//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/socket/netStatus.h"

namespace hyc
{
    //-----------------------------------------------
    hyc::s32 NetStatus::ErrorCode()
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::WSAGetLastError();
#else
        return errno;
#endif        
    }

    //-----------------------------------------------
    bool NetStatus::IsTimeout( const s32& errorCode )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return errorCode == WSAETIMEDOUT;
#else
        return errorCode == ETIMEDOUT;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsInterrupted( const s32& errorCode )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return errorCode == WSAEINTR;
#else
#   ifdef EPROTO
        return errorCode == EINTR || errorCode == EPROTO;
#   else
        return errorCode == EINTR;
#   endif
#endif    
    }

    //-----------------------------------------------
    bool NetStatus::IsConnectFailed( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAECONNREFUSED ||
            error == WSAETIMEDOUT ||
            error == WSAENETUNREACH ||
            error == WSAEHOSTUNREACH ||
            error == WSAECONNRESET ||
            error == WSAESHUTDOWN ||
            error == WSAECONNABORTED;
#else
        return error == ECONNREFUSED ||
            error == ETIMEDOUT ||
            error == ENETUNREACH ||
            error == EHOSTUNREACH ||
            error == ECONNRESET ||
            error == ESHUTDOWN ||
            error == ECONNABORTED;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsConnectRefused( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAECONNREFUSED || error == ERROR_CONNECTION_REFUSED;
#else
        return error == ECONNREFUSED;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsConnectInProgress( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAEWOULDBLOCK || error == WSA_IO_PENDING || error == ERROR_IO_PENDING;
#else
        return error == EINPROGRESS;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsConnectNetUnReach( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAENETUNREACH  || error == WSAEHOSTUNREACH || error == WSAHOST_NOT_FOUND || error == WSATRY_AGAIN;
#else
        return error == ENETUNREACH;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsConnectLost( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAECONNRESET ||
            error == WSAESHUTDOWN ||
            error == WSAENOTCONN ||
            error == ERROR_NETNAME_DELETED ||
            error == WSAECONNABORTED;
#else
        return error == ECONNRESET ||
            error == ENOTCONN ||
            error == ESHUTDOWN ||
            error == ECONNABORTED ||
            error == EPIPE;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsRecvTruncated( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return  error == WSAEMSGSIZE || error == ERROR_MORE_DATA;
#else
        // We don't get an error under Linux if a datagram is truncated.
        return false;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsInterruptedAccept( const s32& error )
    {
        if(IsInterrupted(error))
        {
            return true;
        }

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAECONNABORTED ||
            error == WSAECONNRESET ||
            error == WSAETIMEDOUT;
#else
        return error == ECONNABORTED ||
            error == ECONNRESET ||
            error == ETIMEDOUT;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsNoBuffers( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAENOBUFS ||
            error == WSAEFAULT;
#else
        return error == ENOBUFS;
#endif
    }

    //-----------------------------------------------
    bool NetStatus::IsWouldBlock( const s32& error )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return error == WSAEWOULDBLOCK || error == WSA_IO_PENDING || error == ERROR_IO_PENDING;
#else
        return error == EAGAIN || error == EWOULDBLOCK;
#endif
    }
} // end namespace smbll

#endif // HYC_COMPILE_MODULE_NET
