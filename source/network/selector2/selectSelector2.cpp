//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
//#if defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/socket/transceiver.h"
#include "../../../include/network/selector2/selectSelector2.h"

namespace hyc
{


    //-----------------------------------------------
    SelectSelector2::SelectSelector2()
    {
    }

    //-----------------------------------------------
    SelectSelector2::~SelectSelector2()
    {

    }

    //-----------------------------------------------
    void SelectSelector2::clear()
    {
        _sync.lock();
        _transs.clear();
        FD_ZERO(&_fdsRead);
        FD_ZERO(&_fdsWrite);
        FD_ZERO(&_fdsExcp);
        _sync.unlock();
    }

    //-----------------------------------------------
    void SelectSelector2::initialize( TransceiverPtr pTrans )
    {
        _sync.lock();
        if (_transs.find(pTrans) < 0)
        {
            _transs.push_back(pTrans);
        }
        _sync.unlock();
    }

    //-----------------------------------------------
    void SelectSelector2::remove( TransceiverPtr pTrans )
    {
        _sync.lock();
        s32 i = _transs.find(pTrans);
        if (i >= 0)
        {
            _transs.eraseByIndex(i);
        }
        _sync.unlock();
    }

    //-----------------------------------------------
    void SelectSelector2::select(  
                                INOUT Array<TransceiverPtr>* arrReadFd,
                                INOUT Array<TransceiverPtr>* arrWriteFd,
                                INOUT Array<TransceiverPtr>* arrExceptFd,
                                const s32& timeout  )
    {
        if (arrReadFd)
            arrReadFd->clear();
        if (arrWriteFd)
            arrWriteFd->clear();
        if (arrExceptFd)
            arrExceptFd->clear();

        struct timeval to;
        struct timeval* pto = &to;
        if (timeout < 0)
        {
            pto = NULL;
        }
        else
        {
            to.tv_sec = timeout/1000;
            to.tv_usec = (timeout%1000)*1000;
        }

        SyncTokenPtr token = _sync.getToken();
        {

            FD_ZERO(&_fdsRead);
            FD_ZERO(&_fdsWrite);
            FD_ZERO(&_fdsExcp);

            fd_set* pSetRead = arrReadFd ? &_fdsRead : NULL;
            fd_set* pSetWrite = arrWriteFd ? &_fdsWrite : NULL;
            fd_set* pSetExcp = arrExceptFd ? &_fdsExcp : NULL;

            int maxfd = 0;

            // closed socket set.
            for (u32 i = 0; i < _transs.size(); i++)
            {
                if (_transs[i]->isClosed())
                {
                    if (arrExceptFd)
                    {
                        arrExceptFd->push_back(_transs[i]);
                    }
                    _transs.eraseByIndex(i);
                    i--;
                }
            }

            // live socket set.
            if (!arrExceptFd || arrExceptFd->empty())
            {
                for (u32 i = 0; i < _transs.size(); i++)
                {

#if CN_PLATFORM != CN_PLATFORM_WINDOWS
                    maxfd = Math::Max((int)_transs[i]->socket(), maxfd);
#endif
                    if (pSetRead)
                        FD_SET(_transs[i]->socket(), &_fdsRead);
                    if (pSetWrite)
                        FD_SET(_transs[i]->socket(), &_fdsWrite);
                    if (pSetExcp)
                        FD_SET(_transs[i]->socket(), &_fdsExcp);
                }

                if (!_transs.empty())
                {
                    int num = ::select(maxfd+1, pSetRead, pSetWrite, pSetExcp, pto);
                    switch (num)
                    {
                    case 0:
                        return;
                    case SOCKET_ERROR:
                        error_all(_T("select error"));
                        return;
                    default:
                        {
                            for (u32 i = 0; i < _transs.size(); i++)
                            {
                                if (arrReadFd && FD_ISSET(_transs[i]->socket(), &_fdsRead))
                                {
                                    arrReadFd->push_back(_transs[i]);
                                }
                                if (arrWriteFd && FD_ISSET(_transs[i]->socket(), &_fdsWrite))
                                {
                                    arrWriteFd->push_back(_transs[i]);
                                }
                                if (FD_ISSET(_transs[i]->socket(), &_fdsExcp))
                                {
                                    TransceiverPtr pTrans = _transs[i];
                                    pTrans->close();

                                    if (arrExceptFd)
                                    {                                
                                        arrExceptFd->push_back(pTrans);
                                    }
                                    _transs.eraseByIndex(i);
                                    i--;
                                }
                            }
                        } break;
                    }
                }
                else
                {
                    Thread::Sleep(1);
                }
            } // if.
        }
    }


} // end namespace hyc


//#endif
#endif
