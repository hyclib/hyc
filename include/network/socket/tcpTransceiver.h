//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_TCPTRANSCEIVER_H_20110617171302__
#define __BP_TCPTRANSCEIVER_H_20110617171302__

#include "transceiver.h"
#include "socketUtils.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	TcpTransceiver 收发器
    //!   socket的关闭不由析构来关闭, 必须由close或外部手动关闭
    //----------------------------------------------------------------------------------------------------
    class HYC_API TcpTransceiver : public Transceiver, public MemoryAllocObject
    {
    public:
        
        //! (内建socket,析构将自动关闭socket)
        TcpTransceiver();
        //! 使用外部socket来创建 (析构不关闭socket)
        //!  - connected:    指明socket释放已经连接
        TcpTransceiver( SOCKET socket, bool connected );
        virtual ~TcpTransceiver();

        ConnectResult           connect( const String& host, PORT port )                        { return connect(SocketUtils::AddrCreateAuto(host, port)); }
        ConnectResult           connect( const struct sockaddr_storage& addr );
        ConnectResult           connectTimeout(const String& host, PORT port, s32 timeout)      { return connectTimeout(SocketUtils::AddrCreateAuto(host, port), timeout); }
        ConnectResult           connectTimeout( const struct sockaddr_storage& addr, s32 timeout );

        virtual SocketOperation initialize();

        //! 从buf的_curPos位置开始发送完buf
        virtual bool            write( ByteStream* buf );
        virtual bool            writeAll( ByteStream* buf );
        //! 从buf的_curPos位置开始接收满buf
        //! @return 返回false时有两种情况:
        //!         1. (NetStatus::IsTimeout(NetStatus::ErrorCode()) || NetStatus::IsWouldBlock(NetStatus::ErrorCode())) == true, 则表明为超时.
        //!         2. 失败, 可断开.
        virtual bool            read( ByteStream* buf );
        //! 接收部分buf
        virtual integer         readPart( OUT char* pBuf, uinteger size );

        virtual ProtocolType    protocol() const    { return Protocol_Tcp; }

    protected:
        SocketStatus    _status;
        bool            _autoRelease;
    };

} // end namespace hyc

#endif // end of define __BP_TCPTRANSCEIVER_H_20110617171302__
#endif // HYC_COMPILE_MODULE_NET
