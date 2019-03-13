//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/socket/tcpTransceiver.h"
#include "../../../include/network/socket/tcpAcceptor.h"
#include "../../../include/network/socket/socketUtils.h"
#include "../../../include/network/socket/netStatus.h"
#include "../../../include/container/array.h"


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	TcpAcceptor
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    TcpAcceptor::TcpAcceptor()
    {
        _socket = INVALID_SOCKET;
        _extSocket = INVALID_SOCKET;
        _bAlreadyBinded = false;

#ifdef SOMAXCONN
        _backlog = SOMAXCONN;
#else
        _backlog = 511;
#endif // SOMAXCONN

        //SocketUtils::AddrLocalAddress( _socket, const_cast<struct sockaddr_storage&>(p->_addr) );
    }

    //-----------------------------------------------
    TcpAcceptor::TcpAcceptor( SOCKET socket, bool bAlreadyBinded )
    {
        _socket = INVALID_SOCKET;
        _extSocket = socket;
        _bAlreadyBinded = bAlreadyBinded;

#ifdef SOMAXCONN
        _backlog = SOMAXCONN;
#else
        _backlog = 511;
#endif // SOMAXCONN
        //SocketUtils::AddrLocalAddress( _socket, const_cast<struct sockaddr_storage&>(p->_addr) );
    }

    //-----------------------------------------------
    TcpAcceptor::~TcpAcceptor()
    {
        if (_extSocket == INVALID_SOCKET)
        {
            close();
            assert(_socket == INVALID_SOCKET);
        }
    }

    //-----------------------------------------------
    void TcpAcceptor::_initalize(SOCKET extS)
    {
        if (INVALID_SOCKET == extS)
        {
            _socket = SocketUtils::CreateSocket(false, _addr.ss_family);
            _extSocket = extS;
        }
        else
        {
            _socket = extS;
            _extSocket = extS;
        }

#ifndef _WIN32
        //
        // Enable SO_REUSEADDR on Unix platforms to allow re-using the
        // socket even if it's in the TIME_WAIT state. On Windows,
        // this doesn't appear to be necessary and enabling
        // SO_REUSEADDR would actually not be a good thing since it
        // allows a second process to bind to an address even it's
        // already bound by another process.
        //
        // TODO: using SO_EXCLUSIVEADDRUSE on Windows would probably
        // be better but it's only supported by recent Windows
        // versions (XP SP2, Windows Server 2003).
        //
        SocketUtils::SetReuseAddress(_socket, true);
#endif

        //SocketUtils::SetBlock(_socket, false);
        //setTcpBufSize(_fd, _instance->initializationData().properties, _logger);
    }

    //-----------------------------------------------
    bool TcpAcceptor::listen( const String& host, int port, ProtocolSupport protocol )
    {
        close();

        if (host.empty())
        {
            _addr = SocketUtils::AddrCreate(SocketUtils::AddrToStr(SocketUtils::AddrLocal()), port, protocol, true);
        }
        else
        {
            _addr = SocketUtils::AddrCreate(host, port, protocol, true);
        }

        _initalize(_extSocket);

        // 重用.
        SocketUtils::SetReuseAddress(_socket, true);

        if (!_bAlreadyBinded && !SocketUtils::Bind(_socket, _addr))
        {
            return false;
        }

        struct sockaddr_storage addr;
        SocketUtils::AddrLocalAddress( _socket, addr );
        if (0 != SocketUtils::AddrCompare( addr, _addr ))
        {
            trace( _T("listen: 地址不同") );
            return false;
        }

        SocketUtils::AddrLocalAddress( _socket, _addr );

        SocketUtils::Listen(_socket, _backlog);
        return true;
    }

    //-----------------------------------------------
    TransceiverPtr TcpAcceptor::accept()
    {
        SOCKET fd;
        fd = SocketUtils::Accept(_socket);
        if (SOCKET_ERROR == fd)
        {
            return NULL;
        }
        return new TcpTransceiver(fd, true);
    }

    //-----------------------------------------------
    int TcpAcceptor::effectivePort() const
    {
        return SocketUtils::AddrGetPort(_addr);
    }


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
