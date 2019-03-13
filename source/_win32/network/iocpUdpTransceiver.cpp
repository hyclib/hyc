//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_IOCP) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/socket/udpTransceiver.h"
#include "../../../include/network/socket/socketUtils.h"
#include "../../../include/network/socket/netStatus.h"

#include "../../../include/network/socket/async/win32/iocpUdpTransceiver.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	IOCPUdpTransceiver
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    IOCPUdpTransceiver::IOCPUdpTransceiver(Selector& selector)
    {
        memset(&_mcastAddr, 0, sizeof(_mcastAddr));
        _mcastAddr.ss_family = AF_UNSPEC;
    }

    //-----------------------------------------------
    IOCPUdpTransceiver::~IOCPUdpTransceiver()
    {
    }

    //-----------------------------------------------
    bool IOCPUdpTransceiver::startWriteTo( AsyncInfo* pInfo, AsyncData* pData, const struct sockaddr_storage* peerAddr )
    {
        if (!pData || !peerAddr || !pInfo)   return false;
        ByteStream* buf = pData->pBuf;

        //assert(buf->_curPos >= buf->begin() && buf->_curPos < buf->end() || buf->_curPos == NULL);

        assert(pData->pPosWriteAll >= buf->begin() && pData->pPosWriteAll < buf->end());

        //if (NULL == buf->_curPos)   buf->_curPos = buf->begin();
        integer bufLen = buf->end() - pData->pPosWriteAll;
        assert(bufLen > 0);

        // 超过一个udp包
        if (bufLen > UdpTransceiver::MAX_PACKET_SIZE)
        {
            error_all(_T("Error in IOCPUdpTransceiver::startWrite, the buffer length is too loog!"));
            return false;
        }

        assert(_socket != INVALID_SOCKET);

        pInfo->__inBuf.len = bufLen;
        pInfo->__inBuf.buf = reinterpret_cast<char*>(pData->pPosWriteAll);
        int err;

        socklen_t len;// = static_cast<socklen_t>(sizeof(_peerAddr));
        if(peerAddr->ss_family == AF_INET)
        {
            len = sizeof(sockaddr_in);
        }
        else if(peerAddr->ss_family == AF_INET6)
        {
            len = sizeof(sockaddr_in6);
        }
        else
        {
            // No peer has sent a datagram yet.
            trace(_T("Error IOCPUdpTransceiver::startWrite: SocketException!"));
            return false;
        }
        err = WSASendTo(_socket, &pInfo->__inBuf, 1, &pInfo->inCountBytes, 0, (struct sockaddr*)(peerAddr), 
            len, (LPWSAOVERLAPPED)pInfo, NULL);

        if(err == SOCKET_ERROR)
        {
            pInfo->inError = NetStatus::ErrorCode();
            if(!NetStatus::IsWouldBlock(pInfo->inError))
            {
                if(NetStatus::IsConnectLost(pInfo->inError))
                {
                    trace(_T("Error IOCPUdpTransceiver::startWrite: ConnectionLostException!"));
                    return false;
                }
                else
                {
                    trace(_T("Error IOCPUdpTransceiver::startWrite: SocketException!"));
                    return false;
                }
            }
        }
        return true;
    }

    //-----------------------------------------------
    AsyncTransceiver::Result IOCPUdpTransceiver::finishWrite( AsyncInfo* pInfo, AsyncData* pData )
    {
        if (!pData || !pInfo)   return Result_Error;

        ByteStream* buf = pData->pBuf;

        if(pInfo->inCountBytes == SOCKET_ERROR)
        {
            pInfo->inError = NetStatus::ErrorCode();

            if(NetStatus::IsConnectLost(pInfo->inError))
            {
                trace(_T("Error IOCPUdpTransceiver::finishWrite: ConnectionLostException!"));
                return Result_Error;
            }
            else
            {
                trace(_T("Error IOCPUdpTransceiver::finishWrite: SocketException!"));
                return Result_Error;
            }
        }

        //assert(p->_write._countBytes == static_cast<integer>(buf->size()));
        //buf->_curPos += pInfo->__inCountBytes;    // 用于保证对buf多次利用时, 避免发送错误.
        pData->pPosWriteAll += pInfo->inCountBytes;
        return Result_WouldBlock;
    }

    //-----------------------------------------------
    bool IOCPUdpTransceiver::startReadFrom( AsyncInfo* pInfo, AsyncData* pData, INOUT struct sockaddr_storage* peerAddr )
    {
        if (!pData || !pInfo || !peerAddr)   return false;

        ByteStream* buf = pData->pBuf;

        assert(buf->_curPos >= buf->begin() && buf->_curPos < buf->end() || buf->_curPos == NULL);

        if (NULL == buf->_curPos)   buf->_curPos = buf->begin();
        integer bufLen = buf->end()-buf->_curPos;

        pInfo->__inBuf.len = bufLen;
        pInfo->__inBuf.buf = reinterpret_cast<char*>(buf->_curPos);
        int err;

        pInfo->__inUdpFromLen = static_cast<socklen_t>(sizeof(*peerAddr));

        err = WSARecvFrom(_socket, &pInfo->__inBuf, 1, &pInfo->inCountBytes, &pInfo->__inFlags, 
            reinterpret_cast<struct sockaddr*>(peerAddr), &(pInfo->__inUdpFromLen), (LPWSAOVERLAPPED)pInfo, NULL);

        if(err == SOCKET_ERROR)
        {
            pInfo->inError = NetStatus::ErrorCode();

            if(NetStatus::IsRecvTruncated(pInfo->inError))
            {
                // Nothing to do.
            }
            else if(!NetStatus::IsWouldBlock(pInfo->inError))
            {
                if(NetStatus::IsConnectLost(pInfo->inError))
                {
                    trace(_T("Error IOCPUdpTransceiver::startRead: ConnectionLostException!"));
                    return false;
                }
                else
                {
                    trace(_T("Error IOCPUdpTransceiver::startRead: SocketException!"));
                    return false;
                }
            } // if..else

        } // if

        return true;
    }

    //-----------------------------------------------
    AsyncTransceiver::Result IOCPUdpTransceiver::finishRead( AsyncInfo* pInfo, AsyncData* pData )
    {
        if (!pData || !pInfo)   return Result_Error;
        ByteStream* buf = pData->pBuf;

        if(pInfo->inCountBytes == SOCKET_ERROR)
        {
            pInfo->inError = NetStatus::ErrorCode();

            if(NetStatus::IsConnectLost(pInfo->inError))
            {
                trace(_T("Error IOCPUdpTransceiver::finishRead: ConnectionLostException!"));
                return Result_Error;
            }
            else
            {
                trace(_T("Error IOCPUdpTransceiver::finishRead: SocketException!"));
                return Result_Error;
            }
        }

        assert(buf->_curPos);

        buf->_curPos += pInfo->inCountBytes;
        return Result_WouldBlock;
    }

    //-----------------------------------------------
    hyc::SocketOperation IOCPUdpTransceiver::initialize()
    {
        return SocketOperation_None;
    }

    //-----------------------------------------------
    int IOCPUdpTransceiver::effectivePort() const
    {
        return SocketUtils::AddrGetPort(_addr);
    }

    //-----------------------------------------------
    bool IOCPUdpTransceiver::doServer( const String& host, int port, ProtocolSupport proto, const String& mcastInterface /*= _T("") */ )
    {
        close();

        _addr = SocketUtils::AddrCreate(host, port, proto, true);

        _socket = SocketUtils::CreateSocket(true, _addr.ss_family);
        SocketUtils::SetBlock(_socket, false);

        // if send a package to a unreachable host, next recvFrom will case 10054 error. fix this.
        DWORD dwBytesReturned = 0;
        BOOL bNewBehavior = FALSE;
        DWORD status = WSAIoctl(_socket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
        if (SOCKET_ERROR == status)
        {
            close();
            return false;
        }


        if(SocketUtils::AddrIsMulticast(_addr))
        {
            SocketUtils::SetReuseAddress(_socket, true);
            _mcastAddr = _addr;

            //
            // Windows does not allow binding to the mcast address itself
            // so we bind to INADDR_ANY (0.0.0.0) instead. As a result,
            // bi-directional connection won't work because the source 
            // address won't be the multicast address and the client will
            // therefore reject the datagram.
            //
            _addr = 
                SocketUtils::AddrCreate(_T(""), port, _mcastAddr.ss_family == AF_INET ? ProtocolSupport_IPv4 : ProtocolSupport_IPv6, true);

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
            if (!SocketUtils::Bind(_socket, _addr))
            {
                //assert(0);
                return false;
            }
            SocketUtils::AddrLocalAddress(_socket, _addr);

            _mcastAddr.ss_family = AF_UNSPEC; // AF_UNSPEC means not multicast.
        }

        return true;
    }

    //-----------------------------------------------
    void IOCPUdpTransceiver::close()
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

#endif // CN_NET_IOCP
#endif // HYC_COMPILE_MODULE_NET
