//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_UDPTRANSCEIVER_H_20110618185617__
#define __BP_UDPTRANSCEIVER_H_20110618185617__


#include "transceiver.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	UdpTransceiver 收发器
    //   socket的关闭由析构来自动关闭
    //   广播消息, 即向 地址:"x.x.x.255"以及一个指定端口发送消息.(IPv6无广播)
    //
    //   服务器接收广播包，在linux下，只能绑定 INADDR_ANY.
    //
    //   接收发送区间为 [buf._curPos, buf.end())
    //----------------------------------------------------------------------------------------------------
    class HYC_API UdpTransceiver : public Transceiver, public MemoryAllocObject
    {
    public:
        //!
        //! The maximum IP datagram size is 65535. Subtract 20 bytes for the IP header and 8 bytes for the UDP header
        //! to get the maximum payload.
        //!
        static const u32                UDP_OVERHEAD = 20 + 8;
        static const u32                MAX_PACKET_SIZE = 65535 - UDP_OVERHEAD;

    public:
        UdpTransceiver();
        virtual ~UdpTransceiver();

        virtual void            close();

        //! \brief
        //!     使用指定的参数开启动udp服务器;
        //!     作为udp服务器, 是 [接收->发送] 模型, 即发送之前必须先有接收, 并且, 发送的目标为上次接收的发送方
        //!     当使用组播时, 服务器的地址必须是:
        //!             224.0.0.0~239.255.255.255之间
        //!              1. 224.0.0.0~224.0.0.255 永久组地址
        //!              2. 224.0.1.0~224.0.1.255 公用的组播地址, 用于internet
        //!              3. 224.0.2.0~238.255.255.255 临时组地址, 全网范围内有效
        //!              4. 239.0.0.0~239.255.255.255 本地组地址, 仅在本地范围内有效
        //!     当使用组播时, 可以收发组播组数据.
        //! \param host
        //!     若想接收广播消息, 需置空.
        //! \param mcastInterface
        //!     网卡接口.
        //! \return
        //!     返回false, 表明端口已被占用or ip不正确.
        virtual bool            doServer( const String& host, int port, ProtocolSupport proto, const String& mcastInterface = _T("") );

        //! \brief
        //!     作为客户端连接至指定的服务器
        //!     连接至指定服务器后, 只能与服务器通信, 不能调用 writeTo, readFrom, readPartFrom
        //!     当使用组播时, 只能向组播组发送数据.
        //! \param host
        //!     指定服务器的地址, 如果需广播, 则为 "255.255.255.255"
        //! \param mcastInterface
        //!     网卡接口.
        //! \param mcastTtl
        //!     time to live, 为-1则取默认值 [0~255]
        //!     当路由转发组播包, ttl都将-1, 到0时将被抛弃;
        virtual void            doClient( const String& host, int port, const String& mcastInterface = _T(""), int mcastTtl = -1 );

        virtual SocketOperation initialize();

        //! 当为服务器时, 无法知道对方信息, 需使用 xxxAddr
        //! (仅为客户端时有效.)
        virtual bool            write( ByteStream* buf );
        virtual bool            writeAll( ByteStream* buf ) { return write(buf); }
        virtual bool            read( ByteStream* buf )     { return false; }
        virtual integer         readPart( OUT char* pBuf, uinteger size );

        //! 从指定的地址接收数据.
        //! \param peerAddr 如若不筛选地址则将pPeerAddr置空 memset(pPeerAddr, 0, sizeof(struct sockaddr_storage))
        //!                  数据通信的对方地址将存储在 pPeerAddr上.
        virtual bool            writeTo( ByteStream* buf, const struct sockaddr_storage* peerAddr );
        virtual integer         readPartFrom( OUT char* pBuf, uinteger size, INOUT struct sockaddr_storage* peerAddr );

        virtual ProtocolType    protocol() const    { return Protocol_Udp; }

        //! 绑定的端口.
        int                     effectivePort() const;

    protected:
        struct sockaddr_storage         _addr;
        int                             _addrLen;
        struct sockaddr_storage         _mcastAddr;
        String                          _mcastInterface;
        SocketStatus                    _status;
    };

} // end namespace hyc


#endif // end of define __BP_UDPTRANSCEIVER_H_20110618185617__
#endif // HYC_COMPILE_MODULE_NET
