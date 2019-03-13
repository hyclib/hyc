//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"


#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/socket/socketUtils.h"
#include "../../../include/network/socket/netStatus.h"
#include "../../../include/utils/stringUtils.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   include "../../_win32/socketUtils.hpp"

#elif CN_PLATFORM == CN_PLATFORM_MACOSX || CN_PLATFORM == CN_PLATFORM_IOS
#   include <ifaddrs.h>
#   include "../../_iphone/socketUtils.hpp"

#elif CN_PLATFORM == CN_PLATFORM_LINUX
#   include <ifaddrs.h>
#   include "../../_linux/socketUtils.hpp"

#elif CN_PLATFORM == CN_PLATFORM_ANDROID
#   include "../../_android/ifaddrs.h"
#   include "../../_linux/socketUtils.hpp"
#endif

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	internal function
    //----------------------------------------------------------------------------------------------------
     
    //-----------------------------------------------
    static bool
        _notConnected()
    {
#ifdef _WIN32
        return WSAGetLastError() == WSAENOTCONN;
#elif defined(__APPLE__) || defined(__FreeBSD__)
        return errno == ENOTCONN || errno == EINVAL;
#else
        return errno == ENOTCONN;
#endif
    }

    //-----------------------------------------------
    static struct in_addr
        _getInterfaceAddress(const String& name)
    {
        struct in_addr addr;
        addr.s_addr = INADDR_ANY;
#ifdef _WIN32
        IP_ADAPTER_ADDRESSES addrs;
        ULONG buflen = 0;
        if(::GetAdaptersAddresses(AF_INET, 0, 0, &addrs, &buflen) == ERROR_BUFFER_OVERFLOW)
        {
            PIP_ADAPTER_ADDRESSES paddrs;
            char* buf = new char[buflen];
            paddrs = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buf);
            if(::GetAdaptersAddresses(AF_INET, 0, 0, paddrs, &buflen) == NO_ERROR)
            {
                while(paddrs)
                {
#if defined(_UNICODE) || defined(UNICODE)
                    if(paddrs->FriendlyName == name)
#else
                    cString cstr;
                    StringUtils::Wide2narrow(paddrs->FriendlyName, cstr);
                    if(cstr == name)
#endif
                    {
                        struct sockaddr_in addrin;
                        memcpy(&addrin, paddrs->FirstUnicastAddress->Address.lpSockaddr,
                            paddrs->FirstUnicastAddress->Address.iSockaddrLength);
                        addr =  addrin.sin_addr;
                        break;
                    }
                    paddrs = paddrs->Next;
                }
            }
            delete[] buf;
        }
#else
        ifreq if_address;
#if defined(_UNICODE) || defined(UNICODE)
        cString cstr;
        StringUtils::Wide2narrow(name, cstr);
        strcpy(if_address.ifr_name, cstr.c_str());
#else
        strcpy(if_address.ifr_name, name.c_str());
#endif


        SOCKET fd = SocketUtils::CreateSocket(false, AF_INET);
        int rc = ioctl(fd, SIOCGIFADDR, &if_address);
        SocketUtils::CloseSocket(fd);

        if(rc != SOCKET_ERROR)
        {
            addr = reinterpret_cast<struct sockaddr_in*>(&if_address.ifr_addr)->sin_addr;
        }
#endif

        return addr;
    }

    //-----------------------------------------------
    static int
        _getInterfaceIndex(const String& name)
    {
        int index = 0;
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        IP_ADAPTER_ADDRESSES addrs;
        ULONG buflen = 0;
        if(::GetAdaptersAddresses(AF_INET6, 0, 0, &addrs, &buflen) == ERROR_BUFFER_OVERFLOW)
        {
            PIP_ADAPTER_ADDRESSES paddrs;
            char* buf = new char[buflen];
            paddrs = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buf);
            if(::GetAdaptersAddresses(AF_INET6, 0, 0, paddrs, &buflen) == NO_ERROR)
            {
                while(paddrs)
                {
#if defined(_UNICODE) || defined(UNICODE)
                    if(paddrs->FriendlyName == name)
#else
                    cString cStr;
                    StringUtils::Wide2narrow(paddrs->FriendlyName, cStr);
                    if(cStr == name)
#endif
                    {
                        index = paddrs->Ipv6IfIndex;
                        break;
                    }
                    paddrs = paddrs->Next;
                }
            }
            delete[] buf;
        }
#elif !defined(__hpux)
#if defined(_UNICODE) || defined(UNICODE)
        cString cStr;
        StringUtils::Wide2narrow(name, cStr);
        index = if_nametoindex(cStr.c_str());
#else
        index = if_nametoindex(name.c_str());
#endif

#endif
        return index;
    }

    //----------------------------------------------------------------------------------------------------
    //*	socket utils function
    //----------------------------------------------------------------------------------------------------
     

    //-----------------------------------------------
    struct sockaddr_storage SocketUtils::AddrCreate( const String& host, PORT port, ProtocolSupport protocol, bool server )
    {
        struct sockaddr_storage addr;
        memset(&addr, 0, sizeof(struct sockaddr_storage));

        //
        // We don't use getaddrinfo when host is empty as it's not portable (some old Linux
        // versions don't support it).
        //
        if(host.empty())
        {
            if(protocol == ProtocolSupport_IPv6)
            {
                sockaddr_in6* addrin6 = reinterpret_cast<sockaddr_in6*>(&addr);
                addrin6->sin6_family = AF_INET6;
                addrin6->sin6_port = htons(port);
                addrin6->sin6_addr = server ? in6addr_any : in6addr_loopback;
            }
            else
            {
                sockaddr_in* addrin = reinterpret_cast<sockaddr_in*>(&addr);
                addrin->sin_family = AF_INET;
                addrin->sin_port = htons(port);
                addrin->sin_addr.s_addr = server ? htonl(INADDR_ANY) : htonl(INADDR_LOOPBACK);
            }
            return addr;
        }

        struct addrinfo* info = 0;
        int retry = 5;

        struct addrinfo hints = { 0 };

        if(server)
        {
            //
            // If host is empty, getaddrinfo will return the wildcard
            // address instead of the loopack address.
            //
            hints.ai_flags |= AI_PASSIVE;
        }

        if(protocol == ProtocolSupport_IPv4)
        {
            hints.ai_family = PF_INET;
        }
        else if(protocol == ProtocolSupport_IPv6)
        {
            hints.ai_family = PF_INET6;
        }
        else
        {
            hints.ai_family = PF_UNSPEC;
        }

        int rs = 0;
        do
        {
#if defined(_UNICODE) || defined(UNICODE)
            cString cStr;
            StringUtils::Wide2narrow(host, cStr);
            rs = getaddrinfo(cStr.c_str(), 0, &hints, &info);
#else
            rs = getaddrinfo(host.c_str(), 0, &hints, &info);
#endif
        }
        while(info == 0 && rs == EAI_AGAIN && --retry >= 0);

        if(rs != 0)
        {
            trace(_T("Error AddrCreate! %d; host %s, port %d"), rs, host.c_str(), port);
            return addr;
        }
        assert(0 != info);

        memcpy(&addr, info->ai_addr, info->ai_addrlen);
        if(info->ai_family == PF_INET)
        {
            reinterpret_cast<sockaddr_in*>(&addr)->sin_port = htons(port);
        }
        else if(info->ai_family == PF_INET6)
        {
            reinterpret_cast<sockaddr_in6*>(&addr)->sin6_port = htons(port);
        }
        else // Unknown address family.
        {
            freeaddrinfo(info);
            trace(_T("Error _getAddressImpl!"));
            return addr;
        }
        freeaddrinfo(info);
        return addr;
    }

    //-----------------------------------------------
    struct sockaddr_storage SocketUtils::AddrCreateAuto( const String& host, PORT port )
    {
        struct sockaddr_storage addr;
        memset(&addr, 0, sizeof(struct sockaddr_storage));

        //
        // We don't use getaddrinfo when host is empty as it's not portable (some old Linux
        // versions don't support it).
        //
        if (host.empty())
        {
            sockaddr_in* addrin = reinterpret_cast<sockaddr_in*>(&addr);
            addrin->sin_family = AF_INET;
            addrin->sin_port = htons(port);
            addrin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            return addr;
        }

        struct addrinfo* info = 0;
        int retry = 5;
        struct addrinfo hints = { 0 };
        hints.ai_family = AF_UNSPEC;
        //hints.ai_socktype = SOCK_STREAM;

        int rs = 0;
        do
        {
#if defined(_UNICODE) || defined(UNICODE)
            cString cStr;
            StringUtils::Wide2narrow(host, cStr);
            rs = getaddrinfo(cStr.c_str(), 0, &hints, &info);
#else
            rs = getaddrinfo(host.c_str(), 0, &hints, &info);
#endif
        } while (info == 0 && rs == EAI_AGAIN && --retry >= 0);

        if (rs != 0)
        {
            trace(_T("Error AddrCreate! %d; host %s, port %d"), rs, host.c_str(), port);
            return addr;
        }
        assert(0 != info);

        if (info->ai_family == PF_INET)
        {
            memcpy(&addr, info->ai_addr, info->ai_addrlen);
            reinterpret_cast<sockaddr_in*>(&addr)->sin_port = htons(port);
        }
        else if (info->ai_family == PF_INET6)
        {
            *reinterpret_cast<sockaddr_in6*>(&addr) = *((sockaddr_in6*)info->ai_addr);
            reinterpret_cast<sockaddr_in6*>(&addr)->sin6_port = htons(port);
        }
        else // Unknown address family.
        {
            freeaddrinfo(info);
            trace(_T("Error _getAddressImpl!"));
            return addr;
        }
        freeaddrinfo(info);
        return addr;
    }

    //-----------------------------------------------
    int SocketUtils::AddrCompare( const struct sockaddr_storage& addr1, const struct sockaddr_storage& addr2 )
    {
        if(addr1.ss_family < addr2.ss_family)
        {
            return -1;
        }
        else if(addr2.ss_family < addr1.ss_family)
        {
            return 1;
        }

        if(addr1.ss_family == AF_INET)
        {
            const struct sockaddr_in* addr1in = reinterpret_cast<const sockaddr_in*>(&addr1);
            const struct sockaddr_in* addr2in = reinterpret_cast<const sockaddr_in*>(&addr2);

            if(addr1in->sin_port < addr2in->sin_port)
            {
                return -1;
            }
            else if(addr2in->sin_port < addr1in->sin_port)
            {
                return 1;
            }

            if(addr1in->sin_addr.s_addr < addr2in->sin_addr.s_addr)
            {
                return -1;
            }
            else if(addr2in->sin_addr.s_addr < addr1in->sin_addr.s_addr)
            {
                return 1;
            }
        }
        else
        {
            const struct sockaddr_in6* addr1in = reinterpret_cast<const sockaddr_in6*>(&addr1);
            const struct sockaddr_in6* addr2in = reinterpret_cast<const sockaddr_in6*>(&addr2);

            if(addr1in->sin6_port < addr2in->sin6_port)
            {
                return -1;
            }
            else if(addr2in->sin6_port < addr1in->sin6_port)
            {
                return 1;
            }

            int res = memcmp(&addr1in->sin6_addr, &addr2in->sin6_addr, sizeof(struct in6_addr));
            if(res < 0)
            {
                return -1;
            }
            else if(res > 0)
            {
                return 1;
            }
        }

        return 0;
    }

    //-----------------------------------------------
    void SocketUtils::AddrLocalAddress( SOCKET fd, struct sockaddr_storage& localAddr )
    {
        socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_storage));
        if(getsockname(fd, reinterpret_cast<struct sockaddr*>(&localAddr), &len) == SOCKET_ERROR)
        {
            return;
        }
    }

    //-----------------------------------------------
    bool SocketUtils::AddrRemoteAddress( SOCKET fd, struct sockaddr_storage& remoteAddr )
    {
        socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_storage));
        if(getpeername(fd, reinterpret_cast<struct sockaddr*>(&remoteAddr), &len) == SOCKET_ERROR)
        {
        	trace(_T("get remote addr error!"));
            if(_notConnected())
            {
                return false;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::AddrIsMulticast( const struct sockaddr_storage& addr )
    {
        if(addr.ss_family == AF_INET)
        {
            return IN_MULTICAST(ntohl(reinterpret_cast<const struct sockaddr_in*>(&addr)->sin_addr.s_addr));
        }
        else if(addr.ss_family == AF_INET6)
        {
            return 0 != IN6_IS_ADDR_MULTICAST(&reinterpret_cast<const struct sockaddr_in6*>(&addr)->sin6_addr);
        }
        else
        {
            return false;
        }
    }

    //-----------------------------------------------
    hyc::String SocketUtils::AddrGetIp( const struct sockaddr_storage& ss )
    {
        int size = 0;
        if(ss.ss_family == AF_INET)
        {
            size = sizeof(sockaddr_in);
        }
        else if(ss.ss_family == AF_INET6)
        {
            size = sizeof(sockaddr_in6);
        }
        else
        {
            return _T("");
        }

        char namebuf[1024];
        namebuf[0] = '\0';
        getnameinfo(reinterpret_cast<const struct sockaddr *>(&ss), size, namebuf, sizeof(namebuf), 0, 0, NI_NUMERICHOST);

#if defined(_UNICODE) || defined(UNICODE)
        cString s(namebuf);
        wString ws;
        StringUtils::Narrow2wide( s, ws );
        return ws;
#else
        return String(namebuf);
#endif
    }

    //-----------------------------------------------
    hyc::PORT SocketUtils::AddrGetPort( const struct sockaddr_storage& addr )
    {
        if(addr.ss_family == AF_INET)
        {
            return ntohs(reinterpret_cast<const sockaddr_in*>(&addr)->sin_port);
        }
        else if(addr.ss_family == AF_INET6)
        {
            return ntohs(reinterpret_cast<const sockaddr_in6*>(&addr)->sin6_port);
        }
        else
        {
            return -1;
        }
    }

    //-----------------------------------------------
    void SocketUtils::AddrSetPort( struct sockaddr_storage& addr, PORT port )
    {
        if(addr.ss_family == AF_INET)
        {
            reinterpret_cast<sockaddr_in*>(&addr)->sin_port = htons(port);
        }
        else
        {
            assert(addr.ss_family == AF_INET6);
            reinterpret_cast<sockaddr_in6*>(&addr)->sin6_port = htons(port);
        }
    }

    //-----------------------------------------------
    IPv4 SocketUtils::AddrToIP( const wchar_t* pszIp /*= L"127.0.0.1" */ )
    {
        cString str;
        StringUtils::Wide2narrow( pszIp, StringUtils::GetLength(pszIp), str );
        return htonl(inet_addr( str.c_str() ));
    }

    //-----------------------------------------------
    IPv4 SocketUtils::AddrToIP( const char* pszIp /*= "127.0.0.1" */ )
    {
        return htonl(inet_addr( pszIp ));
    }

    //-----------------------------------------------
    hyc::String SocketUtils::AddrToStr( IPv4 ip )
    {
#if defined(_UNICODE) || defined(UNICODE)
        IN_ADDR ia;
        ia.S_un.S_addr = ntohl(ip);
        cString str = inet_ntoa( ia );
        wString wstr;
        StringUtils::Narrow2wide( str.c_str(), str.size(), wstr );
        return wstr;
#else
        IN_ADDR ia;
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        ia.S_un.S_addr = ntohl(ip);
#else
        ia.s_addr = ntohl(ip);
#endif
        return inet_ntoa( ia );
#endif // defined(_UNICODE) || defined(UNICODE)
    }
    
    //-----------------------------------------------
    hyc::String SocketUtils::AddrToStr_v6( IPv6 ip )
    {
        char buf[1024] = {0};
        
#if defined(_UNICODE) || defined(UNICODE)
        if (NULL != inet_ntop(AF_INET6, (void*)&ip, buf, NI_MAXHOST))
        {
            wString ss;
            StringUtils::Narrow2wide(buf, strlen(buf), ss);
            return ss;
        }
#else
        if (NULL != inet_ntop(AF_INET6, (void*)&ip, buf, NI_MAXHOST))
        {
            return buf;
        }
        
#endif // defined(_UNICODE) || defined(UNICODE)
        
        return NULL;
    }

    //-----------------------------------------------
    void SocketUtils::GetSocketName( SOCKET socket, OUT IPv4& ip, OUT PORT& port )
    {
        sockaddr_in service;
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        s32 len =
#else
        socklen_t len =
#endif
            sizeof(service);
        
        ZeroMemory(&service, len);

        ::getsockname( socket, (sockaddr*)&service, &len );

        ip      = htonl(service.sin_addr.s_addr);
        port    = ntohs(service.sin_port);
    }

    //-----------------------------------------------
    void SocketUtils::GetPeerName( SOCKET socket, OUT IPv4& ip, OUT PORT& port )
    {
        sockaddr_in service;
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        s32 len =
#else
        socklen_t len =
#endif
        sizeof(service);
        ZeroMemory(&service, len);

        ::getpeername( socket, (sockaddr*)&service, &len );

        ip      = htonl(service.sin_addr.s_addr);
        port    = ntohs(service.sin_port);
    }

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
    //-----------------------------------------------
    uinteger SocketUtils::GetConnectTime( SOCKET socket )
    {
        unsigned int dwTime = 0;
        int len = sizeof(dwTime);
        if ( ::getsockopt(socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&dwTime, &len) )
        {
            dwTime = 0;
        }

        return dwTime;
    }
#endif

    //-----------------------------------------------
    uinteger SocketUtils::GetConnectMaxNum()
    {
        return SOMAXCONN;
    }

    //-----------------------------------------------
    uinteger SocketUtils::GetSendBufSize( SOCKET socket )
    {
        u32 dwSize = 0;
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        INT len = sizeof(dwSize);
#else
        socklen_t len = sizeof(dwSize);
#endif
        if ( ::getsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char*)&dwSize, &len) )
        {
            dwSize = -1;
        }

        return dwSize;
    }

    //-----------------------------------------------
    uinteger SocketUtils::GetSendTimeout( SOCKET socket )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        u32 dwTime = 0;
        INT len = sizeof(dwTime);
        if (getsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&dwTime, &len))
        {
            dwTime = 0;
        }

        return dwTime;
#else
        struct timeval timeout;
        socklen_t len = sizeof(timeval);
        if (getsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, &len))
        {
            return 0;
        }

        uinteger dwTime;
        dwTime = timeout.tv_sec * 1000 + timeout.tv_usec / 1000;

        return dwTime;
#endif
    }

    //-----------------------------------------------
    uinteger SocketUtils::GetSendUDPMaxMsgSize( SOCKET socket )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        uinteger dwSize = 0;
        INT len = sizeof(dwSize);
        if ( ::getsockopt(socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&dwSize, &len) )
        {
            dwSize = 0;
        }

        return dwSize;
#else
        return 1500;    // sending over Ethernet, the typical MTU is 1500 bytes
#endif
    }

    //-----------------------------------------------
    bool SocketUtils::SetSendBufSize( SOCKET socket, uinteger dwSize )
    {
        if ( ::setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char*)&dwSize, sizeof(dwSize)) )
        {
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::SetSendTimeout( SOCKET socket, uinteger dwTimeout )
    {
        if (dwTimeout == -1)
        {
            dwTimeout = 0;
        }

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        if ( ::setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&dwTimeout, sizeof(dwTimeout)) )
#else
        struct timeval timeout={(long)dwTimeout/1000, (s32)dwTimeout%1000*1000};
        if ( ::setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) )
#endif
        {
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    uinteger SocketUtils::GetRevBufSize( SOCKET socket )
    {
        u32 dwSize = 0;
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        INT len = sizeof(dwSize);
#else
        socklen_t len = sizeof(dwSize);
#endif

        if ( ::getsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char*)&dwSize, &len) )
        {
            dwSize = 0;
        }

        return dwSize;
    }

    //-----------------------------------------------
    uinteger SocketUtils::GetRevTimeout( SOCKET socket )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        u32 dwTime = 0;
        INT len = sizeof(dwTime);
        if (getsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&dwTime, &len))
        {
            dwTime = 0;
        }

        return dwTime;
#else
        struct timeval timeout;
        socklen_t len = sizeof(timeval);
        if (getsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, &len))
        {
            return 0;
        }

        uinteger dwTime;
        dwTime = timeout.tv_sec * 1000 + timeout.tv_usec / 1000;

        return dwTime;
#endif
    }

    //-----------------------------------------------
    bool SocketUtils::SetRevBufSize( SOCKET socket, uinteger dwSize )
    {
        if ( ::setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char*)&dwSize, sizeof(dwSize)) )
        {
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::SetRevTimeout( SOCKET socket, uinteger dwTimeout )
    {
        if (dwTimeout == -1)
        {
            dwTimeout = 0;
        }

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        if ( ::setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&dwTimeout, sizeof(dwTimeout)) )
#else
        struct timeval timeout={(long)dwTimeout/1000, (s32)dwTimeout%1000*1000};
        if ( ::setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) )
#endif
        {
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    IPv4 SocketUtils::AddrLocal()
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        char szName[255] = {0};
        if ( !::gethostname(szName, sizeof(szName)) )
        {
            hostent* host = ::gethostbyname(szName);
            if (host && host->h_addrtype == AF_INET)
            {
                for ( int i = 0; host->h_addr_list[i] != NULL; i++ )
                {
                    IPv4 addr = (*(in_addr*)host->h_addr_list[i]).s_addr;
                    if (addr != INADDR_LOOPBACK)
                    {
                        //print_all(_T("%s"), SocketUtils::AddrToStr(addr).c_str());
                        return htonl(addr);
                    }
                }
            }
        }
        
#else
        char host[NI_MAXHOST] = {0};
        struct ifaddrs *ifaddr, *ifa;
        int family, s;
        struct in_addr linux_in_addr;
        
        if (getifaddrs(&ifaddr) == -1) {
            return INADDR_LOOPBACK;
        }
        
        int idx = 0;
        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr) continue;
            family = ifa->ifa_addr->sa_family;
            
            if (family == AF_INET) {
                s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                if (s != 0) {
                    freeifaddrs(ifaddr);
                    return INADDR_LOOPBACK;
                }
                if (inet_aton(host, &linux_in_addr) == 0) {
                    freeifaddrs(ifaddr);
                    return INADDR_LOOPBACK;
                }
                else {
                    if (INADDR_LOOPBACK != htonl(linux_in_addr.s_addr))
                    {
                        freeifaddrs(ifaddr);
                        return htonl(linux_in_addr.s_addr);
                    }
                }
                idx++;
            }
        }

        freeifaddrs(ifaddr);
#endif

        return INADDR_LOOPBACK;
    }

    //-----------------------------------------------
    hyc::IPv6 SocketUtils::AddrLocal_v6()
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        char szName[255] = { 0 };
        if (!::gethostname(szName, sizeof(szName)))
        {
            struct addrinfo* info = 0;
            struct addrinfo hints = { 0 };
            hints.ai_family = AF_UNSPEC;
            //hints.ai_socktype = SOCK_STREAM;

            if (getaddrinfo(szName, NULL, &hints, &info) != 0)
            {
                return in6addr_loopback;
            }

            in6_addr addr6;
            sockaddr_in6* sockaddr_ip;
            for (struct addrinfo* ptr = info; ptr != NULL; ptr = ptr->ai_next)
            {
                switch (ptr->ai_family) {
                case AF_INET6:
                    sockaddr_ip = (sockaddr_in6*)ptr->ai_addr;
                    addr6 = sockaddr_ip->sin6_addr;
                    freeaddrinfo(info);
                    return addr6;
                    break;
                default:
                    break;
                }
            }
            freeaddrinfo(info);
        }

#else
        struct ifaddrs *ifaddr, *ifa;
        int family;
        struct in6_addr linux_in_addr;

        if (getifaddrs(&ifaddr) == -1) {
            return in6addr_loopback;
        }

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (strcmp("en0", ifa->ifa_name) != 0) continue;
            if (!ifa->ifa_addr) continue;
            if ((ifa->ifa_flags & IFF_UP) == 0) continue;
            
            family = ifa->ifa_addr->sa_family;
            
            if (family == AF_INET6) {
                linux_in_addr = ((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr;
                
                if (IN6_IS_ADDR_MULTICAST(&linux_in_addr))
                    continue;
                if (IN6_IS_ADDR_LOOPBACK(&linux_in_addr))
                    continue;
                if (IN6_IS_ADDR_UNSPECIFIED(&linux_in_addr))
                    continue;
                if (IN6_IS_ADDR_SITELOCAL(&linux_in_addr))
                    continue;
                
                freeifaddrs(ifaddr);
                return linux_in_addr;
            }
        }

        freeifaddrs(ifaddr);
#endif
        
        return in6addr_loopback;
    }

    //-----------------------------------------------
    hyc::IPv4 SocketUtils::AddrHost( const wchar_t* pszHost )
    {
        if ( NULL == pszHost )
        {
            return 0;
        }

        cString strHost;
        StringUtils::Wide2narrow( pszHost, StringUtils::GetLength(pszHost), strHost );
        
        return AddrHost( strHost.c_str() );
    }

    //-----------------------------------------------
    hyc::IPv4 SocketUtils::AddrHost( const char* pszHost )
    {
        if ( NULL == pszHost )
        {
            return 0;
        }

        const char* pHost = NULL;
        struct hostent* pHostent = NULL;
        pHost = pszHost;
        pHostent = gethostbyname( pHost );

        if ( NULL == pHostent || NULL == pHostent->h_addr_list )
        {
            return 0;
        }

        char* pAddr = pHostent->h_addr_list[0];
        return htonl((*(IPv4*)pAddr));
    }

    //-----------------------------------------------
    uinteger SocketUtils::AddrHash_jenkins( const IPv4& ip, const PORT& port )
    {
        char key[9] = {0};
        memcpy( key, &ip, sizeof(IPv4) );
        memcpy( key+4, &port, sizeof(PORT) );

        uinteger hash = 0;
        uinteger i;

        for (i = 0; i < 9; i++) {
            hash += key[i];
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        return hash;
    }


    //-----------------------------------------------
    bool SocketUtils::SetTcpNoDelay( SOCKET fd )
    {
        int flag = 1;
        if(setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
        {
        	return false;
            //CloseSocket(fd);
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::SetKeepAlive( SOCKET fd )
    {
        int flag = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
        {
        	return false;
            //CloseSocket(fd);
        }

        return true;
    }


    //-----------------------------------------------
    bool SocketUtils::SetBlock( SOCKET fd, bool isBlock )
    {
        if(isBlock)
        {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
            unsigned long arg = 0;
            if(ioctlsocket(fd, FIONBIO, &arg) == SOCKET_ERROR)
            {
                //CloseSocket(fd);
                return false;
            }
#else
            int flags = fcntl(fd, F_GETFL);
            flags &= ~O_NONBLOCK;
            if(fcntl(fd, F_SETFL, flags) == SOCKET_ERROR)
            {
                //CloseSocket(fd);
                return false;
            }
#endif
        }
        else
        {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
            unsigned long arg = 1;
            if(ioctlsocket(fd, FIONBIO, &arg) == SOCKET_ERROR)
            {
                //CloseSocket(fd);
                return false;
            }
#else
            int flags = fcntl(fd, F_GETFL);
            flags |= O_NONBLOCK;
            if(fcntl(fd, F_SETFL, flags) == SOCKET_ERROR)
            {
                //CloseSocket(fd);
                return false;
            }
#endif
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::SetReuseAddress( SOCKET fd, bool reuse )
    {
        int flag = reuse ? 1 : 0;
        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
        {
            //CloseSocket(fd);
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::AddMulticastGroup( SOCKET fd, const struct sockaddr_storage& group, const String& interfacea )
    {
        int rc;
        if(group.ss_family == AF_INET)
        {
            struct ip_mreq mreq;
            mreq.imr_multiaddr = reinterpret_cast<const struct sockaddr_in*>(&group)->sin_addr;
            mreq.imr_interface.s_addr = INADDR_ANY;
            if(interfacea.size() > 0)
            {
                //
                // First see if it is the interfacea name. If not check if IP Address.
                //
                mreq.imr_interface = _getInterfaceAddress(interfacea);
                if(mreq.imr_interface.s_addr == INADDR_ANY)
                {
                    struct sockaddr_storage addr = AddrCreate(interfacea, 0, ProtocolSupport_IPv4, true);
                    mreq.imr_interface = reinterpret_cast<const struct sockaddr_in*>(&addr)->sin_addr;
                }
            }
            rc = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, int(sizeof(mreq)));
        }
        else
        {
            struct ipv6_mreq mreq;
            mreq.ipv6mr_multiaddr = reinterpret_cast<const struct sockaddr_in6*>(&group)->sin6_addr;
            mreq.ipv6mr_interface = 0;
            if(interfacea.size() != 0)
            {
                //
                // First check if it is the interfacea name. If not check if index.
                //
                mreq.ipv6mr_interface = _getInterfaceIndex(interfacea);
                if(mreq.ipv6mr_interface == 0)
                {
                    if (interfacea.size() != 2)
                    {
                        trace(_T("Error SetMulticastGroup!"));
                        //CloseSocket(fd);
                        return false;
                    }
                }
            }
            rc = setsockopt(fd, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char*)&mreq, int(sizeof(mreq)));
        }
        if(rc == SOCKET_ERROR)
        {
            trace(_T("Error SetMulticastGroup!"));
            //CloseSocket(fd);
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::LeaveMulticastGroup( SOCKET fd, const struct sockaddr_storage& group, const String& interfacea )
    {
        if (INVALID_SOCKET == fd)   return false;

        int rc;
        if(group.ss_family == AF_INET)
        {
            struct ip_mreq mreq;
            mreq.imr_multiaddr = reinterpret_cast<const struct sockaddr_in*>(&group)->sin_addr;
            mreq.imr_interface.s_addr = INADDR_ANY;
            if(interfacea.size() > 0)
            {
                //
                // First see if it is the interfacea name. If not check if IP Address.
                //
                mreq.imr_interface = _getInterfaceAddress(interfacea);
                if(mreq.imr_interface.s_addr == INADDR_ANY)
                {
                    struct sockaddr_storage addr = AddrCreate(interfacea, 0, ProtocolSupport_IPv4, true);
                    mreq.imr_interface = reinterpret_cast<const struct sockaddr_in*>(&addr)->sin_addr;
                }
            }
            rc = setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, int(sizeof(mreq)));
        }
        else
        {
            struct ipv6_mreq mreq;
            mreq.ipv6mr_multiaddr = reinterpret_cast<const struct sockaddr_in6*>(&group)->sin6_addr;
            mreq.ipv6mr_interface = 0;
            if(interfacea.size() != 0)
            {
                //
                // First check if it is the interfacea name. If not check if index.
                //
                mreq.ipv6mr_interface = _getInterfaceIndex(interfacea);
                if(mreq.ipv6mr_interface == 0)
                {
                    if (interfacea.size() != 2)
                    {
                        trace(_T("Error SetMulticastGroup!"));
                        //CloseSocket(fd);
                        return false;
                    }
                }
            }
            rc = setsockopt(fd, IPPROTO_IPV6, IPV6_LEAVE_GROUP, (char*)&mreq, int(sizeof(mreq)));
        }
        if(rc == SOCKET_ERROR)
        {
            trace(_T("Error SetMulticastGroup!"));
            //CloseSocket(fd);
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::SetMulticastInterface( SOCKET fd, const String& interfacea, bool IPv4 )
    {
        int rc;
        if(IPv4)
        {
            //
            // First see if it is the interfacea name. If not check if IP Address.
            //
            struct in_addr iface = _getInterfaceAddress(interfacea);
            if(iface.s_addr == INADDR_ANY)
            {
                struct sockaddr_storage addr = AddrCreate(interfacea, 0, ProtocolSupport_IPv4, true);
                iface = reinterpret_cast<const struct sockaddr_in*>(&addr)->sin_addr;
            }
            rc = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char*)&iface, int(sizeof(iface)));
        }
        else
        {
            //
            // First check if it is the interfacea name. If not check if index.
            //
            int interfaceNum = _getInterfaceIndex(interfacea);
            if(interfaceNum == 0)
            {
//                 istringstream p(interfacea);
//                 if(!(p >> interfaceNum) || !p.eof())
//                 {
//                     closeSocketNoThrow(fd);
//                     SocketException ex(__FILE__, __LINE__);
//                     ex.error = 0;
//                 }
                if (interfacea.size() != 2)
                {
                    trace(_T("Error SetMulticastInterface!"));
                    //CloseSocket(fd);
                    return false;
                }
            }
            rc = setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char*)&interfaceNum, int(sizeof(int)));
        }
        if(rc == SOCKET_ERROR)
        {
            trace(_T("Error SetMulticastInterface!"));
            //CloseSocket(fd);
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::SetMulticastTtl( SOCKET fd, int ttl, bool IPv4 )
    {
        int rc;
        if(IPv4)
        {
            rc = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, int(sizeof(int)));
        }
        else
        {
            rc = setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, (char*)&ttl, int(sizeof(int)));
        }
        if(rc == SOCKET_ERROR)
        {
            trace(_T("Error SetMulticastTtl!"));
            //CloseSocket(fd);
            return false;
        }

        return true;
    }



    //-----------------------------------------------
    SOCKET SocketUtils::CreateSocket( bool udp, s32 af_family )
    {
        SOCKET fd;

        if(udp)
        {
            fd = socket(af_family, SOCK_DGRAM, IPPROTO_UDP);
        }
        else
        {
            fd = socket(af_family, SOCK_STREAM, IPPROTO_TCP);
        }

        if(fd == INVALID_SOCKET)
        {
            return fd;
        }

        /*if(!udp)
        {
            SetTcpNoDelay(fd);
            SetKeepAlive(fd);
        }*/

        return fd;
    }
    //-----------------------------------------------
    void SocketUtils::CloseSocket( SOCKET fd )
    {
        if (INVALID_SOCKET == fd)
            return;

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        ::shutdown(fd, SD_BOTH);
        ::closesocket(fd);
//         if(::closesocket(fd) == SOCKET_ERROR)
//         {
//             trace(_T("Error in ClostSocket!"));
//             return;
//         }
#else
        ::shutdown(fd, SHUT_RDWR);
        close(fd);

//         int error = errno;
//         if(close(fd) == SOCKET_ERROR)
//         {
//             trace(_T("Error in ClostSocket!"));
//             return;
//         }
//         errno = error;
#endif
    }

    //-----------------------------------------------
    bool SocketUtils::Bind( SOCKET fd, const struct sockaddr_storage& addr )
    {
        int size;
        if(addr.ss_family == AF_INET)
        {
            size = sizeof(sockaddr_in);
        }
        else if(addr.ss_family == AF_INET6)
        {
            size = sizeof(sockaddr_in6);
        }
        else
        {
            assert(false);
            size = 0; // Keep the compiler happy.
        }

        if(bind(fd, reinterpret_cast<const struct sockaddr*>(&addr), size) == SOCKET_ERROR)
        {
            trace(_T("Error Bind!"));
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    void SocketUtils::Listen( SOCKET fd, s32 backlog )
    {
        do 
        {
            if(::listen(fd, backlog) != SOCKET_ERROR)
            {
                return;
            }
        } while (NetStatus::IsInterrupted(NetStatus::ErrorCode()));

        error_all(_T("Error Listen!"));
        return;

    }

    //-----------------------------------------------
    hyc::ConnectResult SocketUtils::Connect( SOCKET fd, const struct sockaddr_storage& addr )
    {
        do 
        {
            int size;
            if(addr.ss_family == AF_INET)
            {
                size = sizeof(sockaddr_in);
            }
            else if(addr.ss_family == AF_INET6)
            {
                size = sizeof(sockaddr_in6);
            }
            else
            {
                //assert(false);
                //size = 0; // Keep the compiler happy.
                return ConnectResult_Error;
            }

            if(::connect(fd, reinterpret_cast<const struct sockaddr*>(&addr), size) == SOCKET_ERROR)
            {
                s32 err = NetStatus::ErrorCode();
                if(NetStatus::IsInterrupted(err))
                {
                    continue;
                }

                if(NetStatus::IsConnectInProgress(err))
                {
                    return ConnectResult_InProgress;
                }

                if(NetStatus::IsConnectRefused(err))
                {
                    return ConnectResult_ConnectRefused;
                }
                else if(NetStatus::IsConnectFailed(err) || NetStatus::IsConnectNetUnReach(err))
                {
                    return ConnectResult_ConnectFailed;
                }
                else
                {
                    trace(_T("Error Connect!"));
                    return ConnectResult_Error;
                }
            }
            break;

        } while (true);

#if defined(__linux)
        //
        // Prevent self connect (self connect happens on Linux when a client tries to connect to
        // a server which was just deactivated if the client socket re-uses the same ephemeral
        // port as the server).
        //
        struct sockaddr_storage localAddr;
        AddrLocalAddress(fd, localAddr);
        if(AddrCompare(addr, localAddr) == 0)
        {
            return ConnectResult_ConnectRefused;
        }
#endif
        return ConnectResult_Ok;
    }

    //-----------------------------------------------
    ConnectResult SocketUtils::ConnectFinish( SOCKET fd )
    {
        //
        // Note: we don't close the socket if there's an exception. It's the responsability
        // of the caller to do so.
        //

        //
        // Strange windows bug: The following call to Sleep() is
        // necessary, otherwise no error is reported through
        // getsockopt.
        //
#ifdef _WIN32
        Sleep(0);
#endif

        int val;
        socklen_t len = static_cast<socklen_t>(sizeof(int));
        if(getsockopt(fd, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&val), &len) == SOCKET_ERROR)
        {
            trace(_T("Error ConnectFinish!"));
            return ConnectResult_Error;
        }

        if(val > 0)
        {
#ifdef _WIN32
            WSASetLastError(val);
#else
            errno = val;
#endif
            if(NetStatus::IsConnectRefused(val))
            {
                return ConnectResult_ConnectRefused;
            }
            else if(NetStatus::IsConnectFailed(val))
            {
                return ConnectResult_ConnectFailed;
            }
            else
            {
                trace(_T("Error ConnectFinish: val!"));
                return ConnectResult_Error;
            }
        }

#if defined(__linux)
        //
        // Prevent self connect (self connect happens on Linux when a client tries to connect to
        // a server which was just deactivated if the client socket re-uses the same ephemeral
        // port as the server).
        //
        struct sockaddr_storage localAddr;
        AddrLocalAddress(fd, localAddr);

        struct sockaddr_storage remoteAddr;
        if(AddrRemoteAddress(fd, remoteAddr) && AddrCompare(remoteAddr, localAddr) == 0)
        {
            return ConnectResult_ConnectRefused;
        }
#endif

        return ConnectResult_Ok;
    }


    //----------------------------------------------------------------------------------------------------
    //*	iocp
    //----------------------------------------------------------------------------------------------------
     
#ifdef CN_NET_IOCP

    //-----------------------------------------------
    hyc::ConnectResult SocketUtils::ConnectAsync( SOCKET fd, const struct sockaddr_storage& addr, AsyncInfo& info )
    {
        //
        // NOTE: It's the caller's responsability to close the socket upon
        // failure to connect. The socket isn't closed by this method.
        //

        struct sockaddr_storage bindAddr;
        memset(&bindAddr, 0, sizeof(bindAddr));

        int size;
        if(addr.ss_family == AF_INET)
        {
            size = sizeof(sockaddr_in);

            struct sockaddr_in* addrin = reinterpret_cast<struct sockaddr_in*>(&bindAddr);
            addrin->sin_family = AF_INET;
            addrin->sin_port = htons(0);
            addrin->sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else if(addr.ss_family == AF_INET6)
        {
            size = sizeof(sockaddr_in6);

            struct sockaddr_in6* addrin = reinterpret_cast<struct sockaddr_in6*>(&bindAddr);
            addrin->sin6_family = AF_INET6;
            addrin->sin6_port = htons(0);
            addrin->sin6_addr = in6addr_any;
        }
        else
        {
            //assert(false);
            //size = 0; // Keep the compiler happy.
            return ConnectResult_Error;
        }

        if(::bind(fd, reinterpret_cast<const struct sockaddr*>(&bindAddr), size) == SOCKET_ERROR)
        {
            trace(_T("Error ConnectAsync!"));
            return ConnectResult_Error;
        }

        LPFN_CONNECTEX ConnectEx = NULL; // a pointer to the 'ConnectEx()' function
        GUID GuidConnectEx = WSAID_CONNECTEX; // The Guid
        DWORD dwBytes;
        if(WSAIoctl(fd, 
            SIO_GET_EXTENSION_FUNCTION_POINTER,
            &GuidConnectEx,
            sizeof(GuidConnectEx),
            &ConnectEx,
            sizeof(ConnectEx),
            &dwBytes,
            NULL, 
            NULL) == SOCKET_ERROR)
        {
            trace(_T("Error ConnectAsync : WSAIoctl!"));
            return ConnectResult_Error;
        }

        if(!ConnectEx(fd, reinterpret_cast<const struct sockaddr*>(&addr), size, 0, 0, 0, 
#if defined(_MSC_VER) && (_MSC_VER < 1300) // COMPILER FIX: VC60
            reinterpret_cast<LPOVERLAPPED>(&info)
#else
            (LPWSAOVERLAPPED)(&info)
#endif
            ))
        {
            s32 err = NetStatus::ErrorCode();
            if(!NetStatus::IsConnectInProgress(err))
            {
                if(NetStatus::IsConnectRefused(err))
                {
                    return ConnectResult_ConnectRefused;
                }
                else if(NetStatus::IsConnectFailed(err))
                {
                    return ConnectResult_ConnectFailed;
                }
                else
                {
                    trace(_T("Error ConnectAsync : ConnectEx!"));
                    return ConnectResult_Error;
                }
            }
        }

        return ConnectResult_Ok;
    }

    //-----------------------------------------------
    hyc::ConnectResult SocketUtils::ConnectFinishAsync( SOCKET fd, AsyncInfo& info )
    {
        //
        // NOTE: It's the caller's responsability to close the socket upon
        // failure to connect. The socket isn't closed by this method.
        //

        if(info.inCountBytes == SOCKET_ERROR)
        {
            WSASetLastError(info.inError);
            if(NetStatus::IsConnectRefused(info.inError))
            {
                return ConnectResult_ConnectRefused;
            }
            else if(NetStatus::IsConnectFailed(info.inError))
            {
                return ConnectResult_ConnectFailed;
            }
            else
            {
                trace(_T("Error ConnectFinishAsync!"));
                return ConnectResult_Error;
            }
        }

        if(setsockopt(fd, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) == SOCKET_ERROR)
        {
            trace(_T("Error ConnectFinishAsync: setsockopt!"));
            return ConnectResult_Error;
        }

        return ConnectResult_Ok;
    }
#endif // CN_NET_IOCP

    //----------------------------------------------------------------------------------------------------
    //*	end of CN_NET_IOCP
    //----------------------------------------------------------------------------------------------------
     

    //-----------------------------------------------
    SOCKET SocketUtils::Accept( SOCKET fd )
    {
        SOCKET ret;

        do 
        {
            if((ret = ::accept(fd, 0, 0)) == INVALID_SOCKET)
            {
                s32 err = NetStatus::ErrorCode();
                if(NetStatus::IsInterruptedAccept(err))
                {
                    continue;
                }

                return SOCKET_ERROR;
            }
            else
                break;
        } while (true);
        
        //SetTcpNoDelay(ret);
        //SetKeepAlive(ret);
        return ret;
    }

    //-----------------------------------------------
    bool SocketUtils::CreatePipe( SOCKET fds[2] )
    {
#ifdef _WIN32

        SOCKET fd = CreateSocket(false, AF_INET);
        SetBlock(fd, true);

        struct sockaddr_storage addr;
        memset(&addr, 0, sizeof(addr));

        struct sockaddr_in* addrin = reinterpret_cast<struct sockaddr_in*>(&addr);
        addrin->sin_family = AF_INET;
        addrin->sin_port = htons(0);
        addrin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        if (!Bind(fd, addr))
        {
            return false;
        }

        AddrLocalAddress( fd, addr );
        Listen(fd, 1);

        try
        {
            fds[0] = CreateSocket(false, AF_INET);
        }
        catch(...)
        {
            ::closesocket(fd);
            return false;
        }

        try
        {
            SetBlock(fds[0], true);
#ifdef _DEBUG
            ConnectResult cr = Connect(fds[0], addr);
            assert(cr == ConnectResult_Ok);
#else
            Connect(fds[0], addr);
#endif
        }
        catch(...)
        {
            ::closesocket(fds[0]);
            ::closesocket(fd);
            return false;
        }

        try
        {
            fds[1] = Accept(fd);
        }
        catch(...)
        {
            ::closesocket(fds[0]);
            ::closesocket(fd);
            return false;
        }

        ::closesocket(fd);

        try
        {
            SetBlock(fds[1], true);
        }
        catch(...)
        {
            ::closesocket(fds[0]);
            ::closesocket(fds[1]);
            return false;
        }

#else

        if(::pipe(fds) != 0)
        {
            return false;

        }

        try
        {
            SetBlock(fds[0], true);
        }
        catch(...)
        {
        	CloseSocket(fds[0]);
            CloseSocket(fds[1]);
            return false;

        }

        try
        {
            SetBlock(fds[1], true);
        }
        catch(...)
        {
            CloseSocket(fds[0]);
            CloseSocket(fds[1]);
            return false;
        }
#endif

        return true;
    }

    //-----------------------------------------------
    bool SocketUtils::SetBroadcast( SOCKET socket, bool enable )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        BOOL e = enable;
#else
        int e = enable ? 1 : 0;
#endif
        return SOCKET_ERROR != ::setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char*)&e, sizeof(e) );
    }

    //-----------------------------------------------
    bool SocketUtils::AddrIsIPv6orIPv4( const wchar_t* pszHost, OUT bool& isIPv4 )
    {
        cString host;
        StringUtils::Wide2narrow(pszHost, StringUtils::GetLength(pszHost), host);
        return AddrIsIPv6orIPv4(host.c_str(), isIPv4);
    }

    //-----------------------------------------------
    bool SocketUtils::AddrIsIPv6orIPv4( const char* pszHost, OUT bool& isIPv4 )
    {
        if (inet_addr(pszHost) != 0xffffffff)
        {
            isIPv4 = true;
            return true;
        }
        else
        {
#if CN_PLATFORM != CN_PLATFORM_WINDOWS
            struct in6_addr s6;
            if (inet_pton(AF_INET6, pszHost, &s6) > 0)
            {
                isIPv4 = false;
                return true;
            }
#else
            isIPv4 = false;
            return true;
#endif
        }

        return false;
    }


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
