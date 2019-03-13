//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  socket工具
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_SOCKETUTILS_H_20101214143356__
#define __BP_SOCKETUTILS_H_20101214143356__

namespace hyc
{
    //-----------------------------------------------
    // 连接结果
    typedef enum {
        ConnectResult_Ok,
        ConnectResult_InProgress,
        ConnectResult_ConnectRefused,
        ConnectResult_ConnectFailed,
        ConnectResult_Error,
    } ConnectResult;

	//----------------------------------------------------------------------------------------------------
	//*	SocketUtils
	//----------------------------------------------------------------------------------------------------

    class HYC_API SocketUtils
    {
    public:
        /*---------------------------
          网卡工具
        ---------------------------*/

        //! 获得网络适配器个数; windows平台下, pArrNames 无效, linux下 pArrNames 获得网络适配器名称
        static integer      GetAdapterNum( OUT Array<String>* pArrNames = NULL );
        //! 获得网络适配器地址; 错误返回的mac地址为0值
        static MacAddress   GetAdapterAddr( uinteger index );

        /*---------------------------
          地址工具
        ---------------------------*/

        static bool     AddrIsIPv6orIPv4( const wchar_t* pszHost, OUT bool& isIPv4 );   // 返回值为false表示为非法地址.
        static bool     AddrIsIPv6orIPv4( const char* pszHost, OUT bool& isIPv4 );
        static IPv4     AddrHost( const wchar_t* pszHost );   // 从域名得到ip
        static IPv4     AddrHost( const char* pszHost );   // 从域名得到ip
        static IPv4     AddrToIP( const wchar_t* pszIp = L"127.0.0.1" );    // 错误返回 INADDR_NONE.
        static IPv4     AddrToIP( const char* pszIp = "127.0.0.1" );        // 错误返回 INADDR_NONE.
        static String   AddrToStr( IPv4 ip );                               // 错误返回 NULL.

        static String   AddrToStr_v6( IPv6 ip );                            // 错误返回 NULL.
        
        static IPv4     AddrLocal();        // 本机ip
        static IPv4     AddrBroadcast()    { return INADDR_BROADCAST; }    // 255.255.255.255  广播地址/同网段内的全部地址; 可使用 netid.255 代表某个网段的广播地址
        static IPv4     AddrAny()          { return INADDR_ANY; }          // 0.0.0.0          所有地址
        static IPv4     AddrLoopback()     { return INADDR_LOOPBACK; }     // 127.0.0.1        回路地址

        static IPv6     AddrLocal_v6();     // 本机ip
        static IPv6     AddrAny_v6()       { return in6addr_any; }         // 0                所有地址
        static IPv6     AddrLoopback_v6()  { return in6addr_loopback; }    // 0:0:0:0:0:0:0:1  回路地址

        static uinteger AddrHash_jenkins( const IPv4& ip, const PORT& port ); // jenkins hash计算socket (IPv4)

        /*---------------------------
          sockaddr_storage 地址信息工具
        ---------------------------*/

        // 创建将要bind的地址.
        // 0.0.0.0 地址时, host需置空. 如何sockaddr_storage.ss_family == 0 表示地址无效.
        static struct sockaddr_storage
            AddrCreate( const String& host, PORT port, ProtocolSupport protocol, bool server );
        // 根据已经存在的域名或ip自动创建地址; 
        // 0.0.0.0 地址时, host需置空. 如何sockaddr_storage.ss_family == 0 表示地址无效.
        static struct sockaddr_storage
            AddrCreateAuto( const String& host, PORT port );

        static int
            AddrCompare( const struct sockaddr_storage& addr1, const struct sockaddr_storage& addr2 );
        static void
            AddrLocalAddress( SOCKET socket, struct sockaddr_storage& localAddr );
        static bool // 无连接返回false
            AddrRemoteAddress( SOCKET socket, struct sockaddr_storage& remoteAddr );
        static bool
            AddrIsMulticast( const struct sockaddr_storage& addr );
        static String
            AddrGetIp( const struct sockaddr_storage& addr );
        static PORT
            AddrGetPort( const struct sockaddr_storage& addr  );
        static void
            AddrSetPort( struct sockaddr_storage& addr, PORT port  );

        /*---------------------------
          socket基本信息
        ---------------------------*/
        
        static void         GetSocketName( SOCKET socket, OUT IPv4& ip, OUT PORT& port );  // 获得socket自身的地址信息
        static void         GetPeerName( SOCKET socket, OUT IPv4& ip, OUT PORT& port );    // 获得socket链接端的地址信息
        
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        static uinteger     GetConnectTime( SOCKET socket );    // 获得链接时间
#endif
        static uinteger     GetConnectMaxNum(); // 本机的最大连接数

        /*---------------------------
          发送相关
        ---------------------------*/

        static uinteger     GetSendBufSize( SOCKET socket );    // 获得发送缓存大小
        static uinteger     GetSendTimeout( SOCKET socket );    // 获得发送超时
        static uinteger     GetSendUDPMaxMsgSize( SOCKET socket );   // 获得udp最大消息大小
        static bool		    SetSendBufSize( SOCKET socket, uinteger dwSize );    // 设置发送缓存大小
        static bool		    SetSendTimeout( SOCKET socket, uinteger dwTimeout ); // 设置发送超时

        /*---------------------------
          接收相关
        ---------------------------*/

        static uinteger     GetRevBufSize( SOCKET socket );      // 获得接收缓存大小
        static uinteger     GetRevTimeout( SOCKET socket );      // 获得接收超时
        static bool		    SetRevBufSize( SOCKET socket, uinteger dwSize );      // 设置接收缓存大小
        static bool		    SetRevTimeout( SOCKET socket, uinteger dwTimeout );   // 设置接收超时

        /*---------------------------
          连接相关
        ---------------------------*/

        static bool         SetTcpNoDelay( SOCKET socket ); // close TCP Nagle.
        static bool         SetKeepAlive( SOCKET socket );
        static bool         SetBlock( SOCKET socket, bool isBlock );

        //! 允许多个socket监听同一个端口, 当连接无效后, 同一个socket可重复使用.
        static bool         SetReuseAddress( SOCKET socket, bool reuse );

        //! 广播设置 (设定一个socket用于发送广播信息).
        static bool         SetBroadcast( SOCKET socket, bool enable );

        //! 多播设置.
        static bool         SetMulticastInterface( SOCKET socket, const String& interface_, bool IPv4 );    // client add to group.
        static bool         SetMulticastTtl( SOCKET socket, int ttl, bool IPv4 ); // time to live

        // don't call this more then once.
        static bool         AddMulticastGroup( SOCKET socket, const struct sockaddr_storage& group, const String& interface_ );
        static bool         LeaveMulticastGroup( SOCKET socket, const struct sockaddr_storage& group, const String& interface_ );

        /*---------------------------
          socket操作
        ---------------------------*/

        static SOCKET           CreateSocket( bool udp, s32 af_family );    // af_family: AF_INET / AF_INET6
        static bool             CreatePipe( SOCKET fds[2] );    // 管道用于通信(进程间或其他)
        static void             CloseSocket( SOCKET socket );
        static bool             Bind( SOCKET socket, const struct sockaddr_storage& addr );
        static void             Listen( SOCKET socket, s32 backlog );
        static ConnectResult    Connect( SOCKET socket, const struct sockaddr_storage& addr );
        static ConnectResult    ConnectFinish( SOCKET socket );

#ifdef CN_NET_IOCP
        static ConnectResult    ConnectAsync( SOCKET socket, const struct sockaddr_storage& addr, AsyncInfo& info );
        static ConnectResult    ConnectFinishAsync( SOCKET socket, AsyncInfo& info );
#endif // CN_NET_IOCP

        static SOCKET           Accept( SOCKET socket );


    };
	 

} // end namespace hyc

#endif // end of define __BP_SOCKETUTILS_H_20101214143356__

#endif // HYC_COMPILE_MODULE_NET
