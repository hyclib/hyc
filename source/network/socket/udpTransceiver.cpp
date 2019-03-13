//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/socket/udpTransceiver.h"
#include "../../../include/network/socket/socketUtils.h"
#include "../../../include/network/socket/netStatus.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	UdpTransceiver
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    UdpTransceiver::UdpTransceiver()
    {
        memset(&_mcastAddr, 0, sizeof(_mcastAddr));
        _mcastAddr.ss_family = AF_UNSPEC; // AF_UNSPEC means not multicast.

        _status = (SocketStatus)-1;
    }

    //-----------------------------------------------
    UdpTransceiver::~UdpTransceiver()
    {
        close();
    }

    //-----------------------------------------------
    hyc::SocketOperation UdpTransceiver::initialize()
    {
        if (_status == (SocketStatus)-1)
        {
            error_all(_T("Error UdpTransceiver::initialize connect failed! doServer/doConnect first."));
            throw ExceptionType_Forbid;
        }

        if(_status == SocketStatus_NeedConnect)
        {
            _status = SocketStatus_ConnectPending;
            return SocketOperation_Connect;
        }
        else if(_status <= SocketStatus_ConnectPending)
        {
            ConnectResult cr = SocketUtils::ConnectFinish(_socket);
            _status = SocketStatus_Connected;

            if (ConnectResult_Ok != cr && ConnectResult_InProgress != cr)
            {
                error_all(_T("Error UdpTransceiver::initialize connect failed!"));
                return SocketOperation_Connect;
            }
        }

        assert(_status >= SocketStatus_Connected);
        return SocketOperation_None;
    }

    //-----------------------------------------------
    bool UdpTransceiver::write( ByteStream* buf )
    {
        if (_status == SocketStatus_NotConnected)
        {
            return writeTo(buf, &_addr);
        }
        
        if (!buf)   return false;
        if (buf->size() <= 0)   return true;

        if (buf->_curPos == NULL)   buf->_curPos = buf->begin();
        assert(buf->_curPos >= buf->begin() && buf->_curPos < buf->end());

        integer bufLen = buf->end() - buf->_curPos;

        assert(_socket != INVALID_SOCKET && _status >= SocketStatus_Connected);

        // 超过一个udp包
        if (bufLen > MAX_PACKET_SIZE)
        {
            error_all(_T("Error in UdpTransceiver::write, the buffer length is too loog!"));
            return false;
        }

        do 
        {
            integer ret;
            if(_status == SocketStatus_Connected)
            {
                ret = ::send(_socket, reinterpret_cast<const char*>(buf->_curPos), bufLen, 0);
            }
            else
            {
                return false;
            }

            if(ret == SOCKET_ERROR)
            {
                s32 err = NetStatus::ErrorCode();

                if(NetStatus::IsInterrupted(err))
                {
                    continue;
                }

                if(NetStatus::IsWouldBlock(err))
                {
                    trace(_T("Error UdpTransceiver::write NetStatus::IsWouldBlock!"));
                    return false;
                }

                trace(_T("Error UdpTransceiver::write: SocketError!"));
                return false;
            } 

            assert(ret == bufLen);
            break;

        } while (true);

        buf->_curPos = buf->end();
        return true;
    }

    //-----------------------------------------------
    hyc::integer UdpTransceiver::readPart( OUT char* pBuf, uinteger size )
    {
        assert(_socket != INVALID_SOCKET);

        integer ret = -1;


        if(_status == SocketStatus_Connected)
        {
            ret = ::recv(_socket, pBuf, size,
                         0
                         );
        }
        else
        {
            return readPartFrom(pBuf, size, NULL);
        }

        return ret;
    }

    //-----------------------------------------------
    bool UdpTransceiver::writeTo( ByteStream* buf, const struct sockaddr_storage* pPeerAddr )
    {
        assert(pPeerAddr);
        if (!buf)   return false;
        if (buf->size() <= 0)   return true;

        if (buf->_curPos == NULL)   buf->_curPos = buf->begin();
        assert(buf->_curPos >= buf->begin() && buf->_curPos < buf->end());

        integer bufLen = buf->end() - buf->_curPos;

        assert(_socket != INVALID_SOCKET && _status >= SocketStatus_Connected);

        // 超过一个udp包
        if (bufLen > MAX_PACKET_SIZE)
        {
            error_all(_T("Error in UdpTransceiver::write, the buffer length is too loog!"));
            return false;
        }

        do 
        {
            integer ret;

            socklen_t len;// = static_cast<socklen_t>(sizeof(_peerAddr));
            if(AF_INET == pPeerAddr->ss_family)
            {
                len = sizeof(sockaddr_in);
            }
            else if(AF_INET6 == pPeerAddr->ss_family)
            {
                len = sizeof(sockaddr_in6);
            }
            else
            {
                error_all(_T("Error UdpTransceiver::write error peerAddr!"));
                return false;
            }

            ret = ::sendto(_socket, reinterpret_cast<const char*>(buf->_curPos), bufLen, 0, 
                (struct sockaddr*)(pPeerAddr), len);

            if(ret == SOCKET_ERROR)
            {
                s32 err = NetStatus::ErrorCode();

                if(NetStatus::IsInterrupted(err))
                {
                    continue;
                }

                if(NetStatus::IsWouldBlock(err))
                {
                    trace(_T("Error UdpTransceiver::write NetStatus::IsWouldBlock!"));
                    return false;
                }

                trace(_T("Error UdpTransceiver::write: SocketError!"));
                return false;
            } 

            assert(ret == bufLen);
            break;

        } while (true);

        buf->_curPos = buf->end();
        return true;
    }

    //-----------------------------------------------
    hyc::integer UdpTransceiver::readPartFrom( OUT char* pBuf, uinteger size, struct sockaddr_storage* pPeerAddr )
    {
        assert(_socket != INVALID_SOCKET);

        integer ret = -1;

        struct sockaddr_storage peerAddr;
        if (!pPeerAddr)
        {
            memset(&peerAddr, 0, sizeof(struct sockaddr_storage));
            pPeerAddr = &peerAddr;
        }

        socklen_t len = static_cast<socklen_t>(sizeof(*pPeerAddr));
        ret = recvfrom(_socket, pBuf, size,
                       0                       ,
            (struct sockaddr*)(pPeerAddr), &len);

        return ret;
    }

    //-----------------------------------------------
    int UdpTransceiver::effectivePort() const
    {
        return SocketUtils::AddrGetPort(_addr);
    }

    //-----------------------------------------------
    bool UdpTransceiver::doServer( const String& host, int port, ProtocolSupport proto, const String& mcastInterface /*= _T("") */ )
    {
        _status = SocketStatus_NotConnected;   // 无需连接

        close();

        _addr = SocketUtils::AddrCreate(host, port, proto, true);
        _addrLen = sizeof(_addr);

        _socket = SocketUtils::CreateSocket(true, _addr.ss_family);
        if (_socket == INVALID_SOCKET)
        {
            error(_T("doServer create socket err"));
            return false;
        }

        //SocketUtils::SetBlock(_socket, false);

        if(SocketUtils::AddrIsMulticast(_addr))
        {
            SocketUtils::SetReuseAddress(_socket, true);
            _mcastAddr = _addr;

#ifdef _WIN32
            //
            // Windows does not allow binding to the mcast address itself
            // so we bind to INADDR_ANY (0.0.0.0) instead. As a result,
            // bi-directional connection won't work because the source 
            // address won't be the multicast address and the client will
            // therefore reject the datagram.
            //
            _addr = 
                SocketUtils::AddrCreate(_T(""), port, _mcastAddr.ss_family == AF_INET ? ProtocolSupport_IPv4 : ProtocolSupport_IPv6, true);
#endif
            if (!SocketUtils::Bind(_socket, _addr))
            {
                //assert(0);
                return false;
            }

            SocketUtils::AddrLocalAddress(_socket, _addr);
            if(0 == SocketUtils::AddrGetPort(_mcastAddr))
            {
                SocketUtils::AddrSetPort(_mcastAddr, SocketUtils::AddrGetPort(_addr));
            }
            SocketUtils::AddMulticastGroup(_socket, _mcastAddr, mcastInterface);
            _mcastInterface = mcastInterface;
        }
        else
        {
#ifndef _WIN32
            //
            // Enable SO_REUSEADDR on Unix platforms to allow re-using
            // the socket even if it's in the TIME_WAIT state. On
            // Windows, this doesn't appear to be necessary and
            // enabling SO_REUSEADDR would actually not be a good
            // thing since it allows a second process to bind to an
            // address even it's already bound by another process.
            //
            // TODO: using SO_EXCLUSIVEADDRUSE on Windows would
            // probably be better but it's only supported by recent
            // Windows versions (XP SP2, Windows Server 2003).
            //
            SocketUtils::SetReuseAddress(_socket, true);
#endif
            if (!SocketUtils::Bind(_socket, _addr))
            {
                return false;
            }
            SocketUtils::AddrLocalAddress(_socket, _addr);

            _mcastAddr.ss_family = AF_UNSPEC; // AF_UNSPEC means not multicast.
        }

        return true;
    }

    //-----------------------------------------------
    void UdpTransceiver::doClient( const String& host, int port, const String& mcastInterface /*= _T("")*/, int mcastTtl /*= -1 */ )
    {
        _status = SocketStatus_NeedConnect;   // 需连接.

        close();

        _addr = SocketUtils::AddrCreateAuto( host, port );
        _addrLen = sizeof(_addr);

        _socket = SocketUtils::CreateSocket(true, _addr.ss_family);
        //SocketUtils::SetBlock(_socket, false);
        if (_socket == INVALID_SOCKET)
        {
            error(_T("doClient create socket err"));
            return;
        }

        // 开启广播设置.
        bool broadcast = false;
        if (host == _T("255.255.255.255"))
        {
            broadcast = true;
            SocketUtils::SetBroadcast(_socket, true);
        }

        //
        // NOTE: setting the multicast interface before performing the
        // connect is important for some OS such as OS X.
        //
        if(SocketUtils::AddrIsMulticast(_addr))
        {
            if(mcastInterface.size() > 0)
            {
                SocketUtils::SetMulticastInterface(_socket, mcastInterface, AF_INET == _addr.ss_family);
            }
            if(mcastTtl != -1)
            {
                SocketUtils::SetMulticastTtl(_socket, mcastTtl, AF_INET == _addr.ss_family);
            }
        }

        //
        // In general, connecting a datagram socket should be non-blocking as this just setups 
        // the default destination address for the socket. However, on some OS, connect sometime
        // returns EWOULDBLOCK. If that's the case, we keep the state as StateNeedConnect. This
        // will make sure the transceiver is notified when the socket is ready for sending (see
        // the initialize() implementation).
        //
        if (broadcast)
        {
            _status = SocketStatus_NotConnected;
        }
        else if(ConnectResult_Ok == SocketUtils::Connect(_socket, _addr))
        {
            _status = SocketStatus_Connected;
        }
    }

    //-----------------------------------------------
    void UdpTransceiver::close()
    {
        if (INVALID_SOCKET == _socket)
        {
            if (_mcastInterface.size() > 0)
            {
                SocketUtils::LeaveMulticastGroup(_socket, _mcastAddr, _mcastInterface);
                _mcastInterface.clear();
                memset(&_mcastAddr, 0, sizeof(_mcastAddr));
                _mcastAddr.ss_family = AF_UNSPEC; // AF_UNSPEC means not multicast.
            }
            Transceiver::close();
        } // if
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
