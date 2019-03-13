//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_SIMPLESERVER_H_20140711172519__
#define __BP_SIMPLESERVER_H_20140711172519__

#include "../../event/event.h"
#include "../../os/thread.h"
#include "../../_platform/platform.h"
#include "../socket/tcpTransceiver.h"
#include "../socket/tcpAcceptor.h"
#include "selector2.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	a simple server implement by using selector2.
    //----------------------------------------------------------------------------------------------------
	class SimpleServer_tcp
    {
    public:
        /**
         * @brief use a selector2 to create server.
         * @param pSelector will destory by server.
         * @param workItemNum the count of work thread.
         */
        SimpleServer_tcp( Selector2* pSelector, int recvTimeout = 100, int workItemNum = 0 ) : _pSelector2(pSelector), _recvTimeout(recvTimeout), _workItemNum(workItemNum)    { _acceptor.addRef(); if (_workItemNum <= 0) _workItemNum = (int)Platform::GetProcessorNum(); }
        virtual ~SimpleServer_tcp() { stop(); delete _pSelector2; }

        /**
         * @brief start/stop server.
         */
        virtual bool    go( const String& host, int port, ProtocolSupport protocol );
        virtual void    stop();
        virtual bool    isRunning() const;

        /**
         * @brief close the connection.
         */
        virtual void    disconnect( TransceiverPtr connection );
        
        /**
         * @brief 接收到消息回调.
         *          在回调函数中接收消息.
         * @param ep
         *          发送消息的Endpoint.
         */
        typedef EventSingle<void( TransceiverPtr ep )>    Event_Msg;
        Event_Msg&              event_msg() { return _eventMsg; }

        /**
         * @brief 新连接的回调; 
         * @param newConnection 新连接的客户端.
         * @return false表明不允许继续连接.
         */
        typedef EventSingle< bool( TransceiverPtr newConnection ) >    Event_Connected;
        Event_Connected&        event_connected()   { return _eventConnected; }

        /**
         * @brief 连接断开的回调.
         * @param conn 断开的连接.
         */
        typedef EventSingle< void( TransceiverPtr conn ) >     Event_Disconnect;
        Event_Disconnect&       event_disconnect()  { return _eventDisconnect; }

    protected:
        ThreadPtr                   _pMsgThread;
        TcpAcceptor                 _acceptor;
        Selector2*                  _pSelector2;
        int                         _recvTimeout;
        int                         _workItemNum;
        ArrayFind< ThreadPtr >      _workItems;
        SyncObject                  _syncWorkItemCV;
        ConditionVariable           _workItemCV;
        Event_Msg           _eventMsg;
        Event_Connected     _eventConnected;
        Event_Disconnect    _eventDisconnect;
        
    private:
        /// work thread.
        class WorkItem : public Thread {
            SimpleServer_tcp& _server;
            volatile s32      _mark;
            SyncObject        _sync;
            ConditionVariable _cv;
            TransceiverPtr    _pTrans;
        public:
            WorkItem( SimpleServer_tcp& server ) : _server(server), _mark(0) {}
        
        public:
            bool getToken( TransceiverPtr& ptrans ) { 
                s32 expected = 0;
                if (atomic_compare_exchange(&_mark, &expected, 1)) {
                    _pTrans = ptrans;
                    _server._pSelector2->remove(ptrans);
                    _cv.signal(); 
                    return true;
                } 
                return false;
            }
        protected:
            void releaseToken() { 
                assert(Thread::CurThreadId() == getThreadId());
                s32 expected = 1;
                HY_VERIFY(atomic_compare_exchange(&_mark, &expected, 0)); 
                _server._pSelector2->initialize(_pTrans);
                _server._workItemCV.broadcast();
            }
            bool isToken()      {
                s32 expected = 1; 
                return atomic_compare_exchange(&_mark, &expected, 1);
            }
            
        public:
            virtual bool    onStart(void* lpParam)   { return true; }
            virtual void    onFinish(void* lpParam)   {}
            virtual bool    onInvoke(void* lpParam) {
                if (!isToken()) {
                    _sync.lock();
                    _cv.waitTimeout(_sync, 1000);
                    _sync.unlock();
                    return false;
                }
                
                assert(_pTrans);
                _server.event_msg()(_pTrans);
                releaseToken();
                
                return false;
            }
        };
        
        /// msg thread.
        class MsgThread : public Thread {
            SimpleServer_tcp*       _pServer;
            ArrayFind<TransceiverPtr> _arrFdRead;
            ArrayFind<TransceiverPtr> _arrFdExcept;
        public:
            MsgThread(SimpleServer_tcp* pServer) : _pServer(pServer)   {}
            virtual bool    onStart(void* lpParam) {
                _pServer->_pSelector2->initialize(&_pServer->_acceptor);
                return true;
            }
            virtual bool    onInvoke(void* lpParam)  {
                _pServer->_pSelector2->select(&_arrFdRead, NULL, &_arrFdExcept, _pServer->_recvTimeout);
                
                for (u32 i = 0; i < _arrFdRead.size(); i++) {
                    if (_arrFdRead[i] == &_pServer->_acceptor) {
                        TransceiverPtr pTrans = _pServer->_acceptor.accept();
                        if (pTrans) {
                            if (_pServer->event_connected()(pTrans)) {
                                _pServer->_pSelector2->initialize(pTrans.get());
                            }
                        }
                    } else {
                        
                        int j = 0;
                        while (true) {
                            WorkItem* pItem = (WorkItem*)_pServer->_workItems[j%_pServer->_workItemNum].get();
                            if (pItem->getToken(_arrFdRead[i])) {
                                break;
                            } else {
                                j++;
                                if (j%_pServer->_workItemNum == 0) {
                                    _pServer->_syncWorkItemCV.lock();
                                    _pServer->_workItemCV.waitTimeout(_pServer->_syncWorkItemCV, 50);
                                    _pServer->_syncWorkItemCV.unlock();
                                }
                            }
                        }
                    }
                }
                for (u32 i = 0; i < _arrFdExcept.size(); i++)
                {
                    _pServer->event_disconnect()(_arrFdExcept[i]);
                }
                return false;
            }
        };
    };

    //-----------------------------------------------
    inline bool SimpleServer_tcp::go( const String& host, int port, ProtocolSupport protocol )
    {
        stop();
        if (!_acceptor.listen(host, port, protocol))    
            return false;
        
        for (int i = 0; i < _workItemNum; i++)
        {
            _workItems.push_back(new WorkItem(*this));
            _workItems[i]->start();
        }

        _pMsgThread = new MsgThread(this);
        _pMsgThread->start();
        return true;
    }

    //-----------------------------------------------
    inline void SimpleServer_tcp::stop()
    {

        if (_pMsgThread)
        {
            _pMsgThread->exit();
            _pMsgThread->join();
            _pMsgThread = NULL;
        }
        for (u32 i = 0; i < _workItems.size(); i++) {
            _workItems[i]->exit();
            _workItems[i]->join();
            _workItems[i] = NULL;
        }
        _workItems.clear();

        _pSelector2->clear();
        _acceptor.close();
    }
    
    //-----------------------------------------------
    inline bool SimpleServer_tcp::isRunning() const
    {
        return !_pMsgThread.isEmpty();
    }

    //-----------------------------------------------
    inline void SimpleServer_tcp::disconnect( TransceiverPtr connection )
    {
        _pSelector2->remove(connection.get());
        if (connection)
            connection->close();
    }

} // end namespace hyc

#endif // end of define __BP_SIMPLESERVER_H_20140711172519__