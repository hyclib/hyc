//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  网络通信相关的数据类型
//

#ifdef HYC_COMPILE_MODULE_NET
#ifndef __BP_NETTYPE_H_20101214164035__
#define __BP_NETTYPE_H_20101214164035__

#include "../hycPlatform.h"

namespace hyc
{
    //-----------------------------------------------
    //!  protocol support
    typedef enum {
        ProtocolSupport_IPv4,
        ProtocolSupport_IPv6,
        ProtocolSupport_Both,
    } ProtocolSupport;

    //-----------------------------------------------
    //!  protocol
    typedef enum {
        Protocol_Tcp,
        Protocol_Udp,
    } ProtocolType;

    //-----------------------------------------------
    //  IP/PORT
    typedef unsigned int    IPv4;
    typedef in6_addr        IPv6;
    typedef unsigned int    PORT;

    //-----------------------------------------------
    //!  MAC地址结构
    typedef struct {
        Byte    addr[6];
    } MacAddress;


    //-----------------------------------------------
    //!  socket operation enum
    typedef enum  {
        SocketOperation_None = 0,
        SocketOperation_Read = 1,
        SocketOperation_Write = 2,
        SocketOperation_Connect = 2
    } SocketOperation;

    //-----------------------------------------------
    //! socket status enum
    typedef enum {
        SocketStatus_NeedConnect    = 0,
        SocketStatus_ConnectPending = 1,
        SocketStatus_Connected      = 2,
        SocketStatus_NotConnected   = 3
    } SocketStatus;

} // end namespace hyc

#endif // end of define __BP_NETTYPE_H_20101214164035__
#endif // HYC_COMPILE_MODULE_NET
