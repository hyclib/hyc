//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET
#ifdef CN_NET_IOCP

#ifndef __BP_IOCPUDPTRANSCEIVER_H_20110701154517__
#define __BP_IOCPUDPTRANSCEIVER_H_20110701154517__

#include "../asyncTransceiver.h"
#include "../../../selector/selector.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	IOCP Udp 异步收发器; IOCP方式只允许为服务端, 为客户端意义不大
    //! doServer/close 操作非线程安全, 其他方法线程安全.
    //----------------------------------------------------------------------------------------------------
    class HYC_API IOCPUdpTransceiver : public AsyncTransceiver, public MemoryAllocObject
    {
    public:
        IOCPUdpTransceiver(Selector& selector);
        virtual ~IOCPUdpTransceiver();

        virtual void            close();

        /**
         * @brief
         *    使用指定的参数开启动udp服务器;
         *     作为udp服务器, 是 [接收->发送] 模型, 即发送之前必须先有接收, 并且, 发送的目标为上次接收的发送方
         * @param mcastInterface
         *    网卡接口.
         * @return
         *    返回false, 表明端口已被占用or ip不正确
         */
        virtual bool            doServer( const String& host, int port, ProtocolSupport proto, const String& mcastInterface = _T("") );

        virtual SocketOperation initialize();

        /**
         * @brief 接收 buf总是为 [_curPos, end)
         *        发送则发送 [pData->pPosWriteAll, end).
         * @param pPeerAddr 数据通信的对方地址将存储在 pPeerAddr上.
         *                  如若不筛选地址则将pPeerAddr置空 memset(pPeerAddr, 0, sizeof(struct sockaddr_storage))
         */
        virtual bool            startWriteTo( AsyncInfo* pInfo, AsyncData* pData, const struct sockaddr_storage* peerAddr );
        virtual Result          finishWrite( AsyncInfo* pInfo, AsyncData* pData );
        virtual bool            startReadFrom( AsyncInfo* pInfo, AsyncData* pData, INOUT struct sockaddr_storage* peerAddr );
        virtual Result          finishRead( AsyncInfo* pInfo, AsyncData* pData );

        virtual ProtocolType    protocol() const    { return Protocol_Udp; }

        //! 绑定的端口.
        int                     effectivePort() const;

    private:
        virtual bool            startWrite( AsyncInfo* pInfo, AsyncData* pData )   { return false; }
        virtual bool            startRead( AsyncInfo* pInfo, AsyncData* pData )   { return false; }

    protected:
        struct sockaddr_storage         _addr;
        struct sockaddr_storage         _mcastAddr;
        String                          _mcastInterface;
    };

} // end namespace hyc

#endif // end of define __BP_IOCPUDPTRANSCEIVER_H_20110701154517__
#endif // CN_NET_IOCP
#endif // HYC_COMPILE_MODULE_NET


