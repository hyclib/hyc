//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  文件为所有文件的头文件, 不包含任何其他文件, 是文件结构的最低层
//

#ifndef __BP_NETPLATFORM_H_2010121492807__
#define __BP_NETPLATFORM_H_2010121492807__

#ifdef HYC_COMPILE_MODULE_NET

//----------------------------------------------------------------------------------------------------
//*	平台
//----------------------------------------------------------------------------------------------------


// SOCKET
#if CN_PLATFORM != CN_PLATFORM_WINDOWS
#   include <unistd.h>
#   include <errno.h>
#   include <sys/types.h>
#   include <sys/socket.h>
//#   include <sys/sockio.h>
#   include <sys/ioctl.h>
#   include <netinet/tcp.h>
#   include <netdb.h>
#   include <net/if.h>
#   include <arpa/inet.h>
#   include <fcntl.h>

#if CN_PLATFORM == CN_PLATFORM_ANDROID
#   include <linux/sysctl.h>
#else
#   include <sys/sysctl.h>
#endif

#   define SOCKET           s32
#   define SOCKET_ERROR     -1
#   define INVALID_SOCKET   -1
#   define IN_ADDR          in_addr
#endif // CN_PLATFORM != CN_PLATFORM_WINDOWS

// windows
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   define CN_NET_IOCP      // IOCP
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <mswsock.h>
#   pragma comment(lib, "ws2_32.lib")
#   include <Iphlpapi.h>
#   pragma comment(lib, "Iphlpapi.lib")

#   ifdef HYC_COMPILE_MODULE_NET_WLAN
#       include <wlanapi.h>
#       pragma comment(lib, "Wlanapi.lib")
#   endif

// mac
#elif CN_PLATFORM == CN_PLATFORM_IOS || CN_PLATFORM == CN_PLATFORM_MACOSX
#   define CN_NET_KQUEUE    // BSD system
#   include <net/if_dl.h>
#   include <sys/event.h>

// linux
#elif CN_PLATFORM == CN_PLATFORM_LINUX
#   define CN_NET_EPOLL     // EPOLL
#   include <sys/epoll.h>

#else
#   pragma error "net的其他平台未配好"
#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS


//----------------------------------------------------------------------------------------------------
//*	必备的头文件
//---------------------------------------------------------------------------------------------------- 

#include "netType.h"
#include "netAsyncInfo.h"

#endif // HYC_COMPILE_MODULE_NET

#endif // end of define __BP_NETPLATFORM_H_2010121492807__

