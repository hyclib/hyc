//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET
#ifdef CN_NET_IOCP

#ifndef __BP_IOCPTCPACCEPTOR_H_20110707155657__
#define __BP_IOCPTCPACCEPTOR_H_20110707155657__

#include "../../tcpAcceptor.h"
#include "iocpTcpTransceiver.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	IOCP 异步连接接受器
    //----------------------------------------------------------------------------------------------------
    class HYC_API IOCPTcpAcceptor : public TcpAcceptor
    {
    public:
        //! \brief
        //!     阻塞方式调用acceptBlock, 非阻塞方式调用startAccept,doAccept,finishAccept
        IOCPTcpAcceptor( Selector& selector, bool isBlock );
        //
        //! 使用外部socket来创建 (不由析构自动关闭socket)
        //! \param  bAlreadyBinded  指明socket是否已经绑定了地址,已经绑定的地址必须与将来监听的地址相同
        //
        IOCPTcpAcceptor( Selector& selector, bool isBlock, SOCKET socket, bool bAlreadyBinded );
        virtual ~IOCPTcpAcceptor();

        virtual void            close();

        virtual bool            listen( const String& host, int port, ProtocolSupport protocol );   //! 返回false表示端口被占用or ip错误

        //! 在投递了startAccept之后, 得到响应后调用doAccept得到传输器, 并调用finishAccept完成.
        void                    startAccept(AsyncInfo* pInfo);
        AsyncTransceiverPtr     doAccept();
        void                    finishAccept(AsyncInfo* pInfo);

        //! 阻塞方式的接收.
        AsyncTransceiverPtr      acceptBlock();

    protected:
        virtual TransceiverPtr  accept()    { throw ExceptionType_UnImplement; }

    protected:
        static const u32    _acceptBufSize = sizeof(sockaddr_storage) + 16;
        Selector&           _rSelector;
        SOCKET              _acceptFd;
        integer             _acceptError;
        char                _acceptBuf[_acceptBufSize * 2];
        bool                _block;
    };


} // end namespace hyc

#endif // end of define __BP_IOCPTCPACCEPTOR_H_20110707155657__
#endif // CN_NET_IOCP
#endif // HYC_COMPILE_MODULE_NET

