//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/socket/tcpTransceiver.h"
#include "../../../include/network/socket/socketUtils.h"
#include "../../../include/network/socket/netStatus.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#define MSG_NOSIGNAL    0
#elif CN_PLATFORM == CN_PLATFORM_IOS
#define MSG_NOSIGNAL    0
#else
#include <signal.h>
#endif

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	TcpTransceiver
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    TcpTransceiver::TcpTransceiver( SOCKET socket, bool connected )
    {
        sockaddr_storage ss;
        SocketUtils::AddrLocalAddress(socket, ss);
//         if (ss.ss_family == AF_INET)
//             _ps = ProtocolSupport_IPv4;
//         else if (ss.ss_family == AF_INET6)
//             _ps = ProtocolSupport_IPv6;
//         else
//         {
//             error_all(_T("can't support this AF: %d"), (int)ss.ss_family);
//         }

        _socket = socket;
        
#if CN_PLATFORM == CN_PLATFORM_IOS
        int flag = 1;
        if(setsockopt(_socket, SOL_SOCKET, SO_NOSIGPIPE, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
        {
            error(_T("ios set SO_NOSIGPIPE error"));
        }
#endif

        _status = connected ? SocketStatus_Connected : SocketStatus_NeedConnect;
        _autoRelease = false;
    }

    //-----------------------------------------------
    TcpTransceiver::TcpTransceiver()
    {
//         _ps = ps;
        _status = SocketStatus_NeedConnect;
        _autoRelease = true;
    }

    //-----------------------------------------------
    TcpTransceiver::~TcpTransceiver()
    {
        if (_autoRelease) close();
    }

    //-----------------------------------------------
    hyc::SocketOperation TcpTransceiver::initialize()
    {
        if(_status == SocketStatus_NeedConnect)
        {
            _status = SocketStatus_ConnectPending;
            return SocketOperation_Connect;
        }
        else if(_status == SocketStatus_ConnectPending)
        {
            ConnectResult cResult = SocketUtils::ConnectFinish(_socket);

            if (cResult != ConnectResult_Ok && cResult != ConnectResult_InProgress)
            {
                trace_all(_T("Error TcpTransceiver::initialize :  tcp connection established!"));
                return SocketOperation_Connect;
            }

            _status = SocketStatus_Connected;
        }
        assert(_status == SocketStatus_Connected);
        return SocketOperation_None;
    }

    //-----------------------------------------------
    bool TcpTransceiver::write( ByteStream* buf )
    {
        if (!buf || _socket == INVALID_SOCKET)   return false;

        // send from begin
        if (NULL == buf->_curPos || buf->end() <= buf->_curPos ) buf->_curPos = buf->begin();

        // Its impossible for the packetSize to be more than an Int.
        integer packetSize = static_cast<integer>(buf->end() - buf->_curPos);

        while(buf->_curPos != buf->end())
        {
            assert(_socket != INVALID_SOCKET);

            integer ret = ::send(_socket, reinterpret_cast<const char*>(&*buf->_curPos), packetSize, MSG_NOSIGNAL);
            if(ret == 0)
            {
                trace(_T("Error TcpTransceiver::write!"));
                return false;
            }

            if(ret == SOCKET_ERROR)
            {
                s32 err = NetStatus::ErrorCode();
                if(NetStatus::IsInterrupted(err))  { continue; }

                if(NetStatus::IsNoBuffers(err) && packetSize > 1024)
                {
                    packetSize /= 2;
                    continue;
                }

                if(NetStatus::IsWouldBlock(err))
                {
                    trace(_T("Error TcpTransceiver::write : NetStatus::IsWouldBlock!"));
                    return false;
                }

                if(NetStatus::IsConnectLost(err))
                {
                    trace(_T("Error TcpTransceiver::write : NetStatus::IsConnectLost!"));
                    return false;
                }

                trace(_T("Error TcpTransceiver::write : socket error!"));
                return false;
            }

            buf->_curPos += ret;

            if(packetSize > buf->end() - buf->_curPos)
            {
                packetSize = static_cast<int>(buf->end() - buf->_curPos);
            }
        } // while

        //buf->_curPos = buf->end();
        return true;
    }

    //-----------------------------------------------
    bool TcpTransceiver::writeAll( ByteStream* buf )
    {
        if (!buf || _socket == INVALID_SOCKET)   return false;

        // Its impossible for the packetSize to be more than an Int.
        integer packetSize = static_cast<integer>(buf->end() - buf->begin());

        Byte* pBegin = buf->begin();

        while(packetSize)
        {
            assert(_socket != INVALID_SOCKET);

            integer ret = ::send(_socket, reinterpret_cast<const char*>(pBegin), packetSize, MSG_NOSIGNAL);
            if(ret == 0)
            {
                trace(_T("Error TcpTransceiver::write!"));
                return false;
            }

            if(ret == SOCKET_ERROR)
            {
                s32 err = NetStatus::ErrorCode();
                if(NetStatus::IsInterrupted(err))  { continue; }

                if(NetStatus::IsNoBuffers(err) && packetSize > 1024)
                {
                    packetSize /= 2;
                    continue;
                }

                if(NetStatus::IsWouldBlock(err))
                {
                    trace(_T("Error TcpTransceiver::write : NetStatus::IsWouldBlock!"));
                    return false;
                }

                if(NetStatus::IsConnectLost(err))
                {
                    trace(_T("Error TcpTransceiver::write : NetStatus::IsConnectLost!"));
                    return false;
                }

                trace(_T("Error TcpTransceiver::write : socket error!"));
                return false;
            }

            pBegin += ret;

            if(packetSize > buf->end() - pBegin)
            {
                packetSize = static_cast<int>(buf->end() - pBegin);
            }
        } // while

        //buf->_curPos = buf->end();
        return true;
    }

    //-----------------------------------------------
    bool TcpTransceiver::read( ByteStream* buf )
    {
        if (!buf || _socket == INVALID_SOCKET)   return false;
        if (NULL == buf->_curPos || buf->end() <= buf->_curPos ) buf->_curPos = buf->begin();

        // Its impossible for the packetSize to be more than an Int.
        int packetSize = static_cast<int>(buf->end() - buf->_curPos);

        while(buf->_curPos != buf->end())
        {
            assert(_socket != INVALID_SOCKET);
            integer ret = ::recv(_socket, reinterpret_cast<char*>(&*buf->_curPos), packetSize, 0);

            if(ret == 0)
            {
                //
                // If the connection is lost when reading data, we shut
                // down the write end of the socket. This helps to unblock
                // threads that are stuck in send() or select() while
                // sending data. Note: I don't really understand why
                // send() or select() sometimes don't detect a connection
                // loss. Therefore this helper to make them detect it.
                //
                //assert(_fd != INVALID_SOCKET);
                //shutdownSocketReadWrite(_fd);
                close();
                trace(_T("Error TcpTransceiver::read : ConnectionLost!"));
                return false;
            }

            if(ret == SOCKET_ERROR)
            {
                s32 err = NetStatus::ErrorCode();

                if(NetStatus::IsInterrupted(err))
                {
                    continue;
                }

                if(NetStatus::IsNoBuffers(err) && packetSize > 1024)
                {
                    packetSize /= 2;
                    continue;
                }

                if(NetStatus::IsWouldBlock(err))
                {
                    if (SocketUtils::GetRevTimeout(socket()) != -1)
                        return false;

                    continue;
                }

                if(NetStatus::IsTimeout(err))
                {
                    return false;
                }
                
                if(NetStatus::IsConnectLost(err))
                {
                    //
                    // See the commment above about shutting down the
                    // socket if the connection is lost while reading
                    // data.
                    //
                    //assert(_fd != INVALID_SOCKET);
                    //shutdownSocketReadWrite(_fd);

                    close();
                    trace(_T("Error TcpTransceiver::read : ConnectionLost2!"));
                    return false;
                }

                close();
                trace(_T("Error TcpTransceiver::read : SocketException!"));
                return false;
            }

            buf->_curPos += ret;

            packetSize = static_cast<int>(buf->end() - buf->_curPos);
        } // while

        //buf->_curPos = buf->end();
        return true;
    }

    //-----------------------------------------------
    integer TcpTransceiver::readPart( OUT char* pBuf, uinteger size )
    {
        if (_socket == INVALID_SOCKET)
            return -1;

        assert(_socket != INVALID_SOCKET);
        return ::recv(_socket, pBuf, size, 0);
    }

    //-----------------------------------------------
    ConnectResult TcpTransceiver::connect( const struct sockaddr_storage& addr )
    {
        if (_socket != INVALID_SOCKET)
        {
            if (_autoRelease) close();
            _socket = INVALID_SOCKET;
            _autoRelease = true;
        }

        if (_socket == INVALID_SOCKET)
        {
            _socket = SocketUtils::CreateSocket(false, addr.ss_family);

            if (_socket == INVALID_SOCKET)
            {
                error(_T("connect create socket err"));
                return ConnectResult_Error;
            }

#if CN_PLATFORM == CN_PLATFORM_IOS
            int flag = 1;
            if(setsockopt(_socket, SOL_SOCKET, SO_NOSIGPIPE, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
            {
                error(_T("ios set SO_NOSIGPIPE error"));
            }
#endif

            _status = SocketStatus_NeedConnect;
        }

        ConnectResult r = ConnectResult_Error;

        r = SocketUtils::Connect(_socket, addr);
        if(ConnectResult_Ok == r)
        {
            _status = SocketStatus_Connected;
        }
        return r;
    }

    //-----------------------------------------------
    ConnectResult TcpTransceiver::connectTimeout( const struct sockaddr_storage& addr, s32 timeout )
    {
        if (_socket != INVALID_SOCKET)
        {
            if (_autoRelease) close();
            _socket = INVALID_SOCKET;
            _autoRelease = true;
        }

        if (_socket == INVALID_SOCKET)
        {
            _socket = SocketUtils::CreateSocket( false, addr.ss_family );
            if (_socket == INVALID_SOCKET)
            {
                error(_T("connect create socket err"));
                return ConnectResult_Error;
            }

#if CN_PLATFORM == CN_PLATFORM_IOS
            int flag = 1;
            if(setsockopt(_socket, SOL_SOCKET, SO_NOSIGPIPE, (char*)&flag, int(sizeof(int))) == SOCKET_ERROR)
            {
                error(_T("ios set SO_NOSIGPIPE error"));
            }
#endif

            _status = SocketStatus_NeedConnect;
        }

        ConnectResult r = ConnectResult_Error;

        SocketUtils::SetBlock( _socket, false );
        // 连接.
        if( ConnectResult_Ok != SocketUtils::Connect(_socket, addr) )
            //::connect(_socket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR ) // in BSD call this will error.
        {
            fd_set fs;
            timeval tm;
            tm.tv_sec  = timeout/1000;  
            tm.tv_usec = timeout%1000*1000;  

            FD_ZERO(&fs);  
            FD_SET(_socket, &fs);
            if( ::select(_socket+1, NULL, &fs, NULL, &tm) > 0)
            {
                r = SocketUtils::ConnectFinish( _socket );
            }
        }
        
        if(ConnectResult_Ok == r)
        {
            _status = SocketStatus_Connected;
        }

        SocketUtils::SetBlock( _socket, true );
        return r;
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
