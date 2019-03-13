//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET
#ifdef CN_NET_IOCP

#ifndef __BP_IOCPTCPTRANSCEIVER_H_20110701155422__
#define __BP_IOCPTCPTRANSCEIVER_H_20110701155422__

#include "../asyncTransceiver.h"
#include "../../../selector/selector.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	IOCP tcp 异步收发器 IOCP方式只允许为服务端, 为客户端意义不大
    //! close 函数非线程安全, 其他函数线程安全.
    //----------------------------------------------------------------------------------------------------
    class HYC_API IOCPTcpTransceiver : public AsyncTransceiver, public MemoryAllocObject
    {
    public:
        //! 需手动关闭socket
        IOCPTcpTransceiver( Selector& selector, SOCKET socket );
        virtual ~IOCPTcpTransceiver();

        virtual SocketOperation initialize();
        virtual void            close();

        /**
         * start: 在调用之后, 可能只会完成部分流的发送(效率原因)
         * finish: 在调用之后, 将会设置buf->_curPos为当前流的位置,
         * 如若buf->_curPos大于流则表示发送完成, 否则需继续发送
         */
        virtual bool            startWrite( AsyncInfo* pInfo, AsyncData* pData );
        virtual Result          finishWrite( AsyncInfo* pInfo, AsyncData* pData );

        virtual bool            startRead( AsyncInfo* pInfo, AsyncData* pData );
        virtual Result          finishRead( AsyncInfo* pInfo, AsyncData* pData );

        virtual ProtocolType    protocol() const    { return Protocol_Tcp; }

    protected:
        struct sockaddr_storage     _connectAddr;
        int                         _maxSendPacketSize;
        int                         _maxReceivePacketSize;
    };

} // end namespace hyc

#endif // end of define __BP_IOCPTCPTRANSCEIVER_H_20110701155422__
#endif // CN_NET_IOCP
#endif // HYC_COMPILE_MODULE_NET


