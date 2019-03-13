/*
 * EPOLLUdpTransceiver.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: root
 */

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_EPOLL) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/socket/async/linux/epollUdpTransceiver.h"

namespace hyc
{
	//----------------------------------------------------------------------------------------------------
    //*	EPOLLUdpTransceiver
    //----------------------------------------------------------------------------------------------------

	//-----------------------------------------------
	EPOLLUdpTransceiver::EPOLLUdpTransceiver(Selector& selector)
        : _rSelector(selector)
	{
		memset(&_mcastAddr, 0, sizeof(_mcastAddr));
		_mcastAddr.ss_family = AF_UNSPEC;
	}

	//-----------------------------------------------
	EPOLLUdpTransceiver::~EPOLLUdpTransceiver()
	{
	}

	//-----------------------------------------------
	SocketOperation EPOLLUdpTransceiver::initialize()
	{
		return SocketOperation_None;
	}

	//-----------------------------------------------
	int EPOLLUdpTransceiver::effectivePort() const
	{
		return SocketUtils::AddrGetPort(_addr);
	}

	//-----------------------------------------------
	void EPOLLUdpTransceiver::close()
	{
		if (INVALID_SOCKET == _socket)
		{
			if (_mcastInterface.size() > 0)
			{
				SocketUtils::LeaveMulticastGroup(_socket, _mcastAddr, _mcastInterface);
				_mcastInterface.clear();
				memset(&_mcastAddr, 0, sizeof(_mcastAddr));
				_mcastAddr.ss_family = AF_UNSPEC;
			}
			//
			Transceiver::close();
		}
	}

	//-----------------------------------------------
	bool EPOLLUdpTransceiver::doServer(const String& host, int port, ProtocolSupport proto, const String& mcastInterface)
	{
		close();

		_addr = SocketUtils::AddrCreate(host, port, proto, true);
		_socket = SocketUtils::CreateSocket(true, _addr.ss_family);
		SocketUtils::SetBlock(_socket, false);

		// if send a package to a unreachable host, next recvFrom will case 10054 error. fix this.
		/*DWORD dwBytesReturned = 0;
		        BOOL bNewBehavior = FALSE;
		        DWORD status = WSAIoctl(_socket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
		        if (SOCKET_ERROR == status)
		        {
		            close();
		            return false;
		        }*/


        if(SocketUtils::AddrIsMulticast(_addr))
        {
            SocketUtils::SetReuseAddress(_socket, true);
            _mcastAddr = _addr;

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
	bool EPOLLUdpTransceiver::startWriteTo(AsyncInfo* pInfo, AsyncData* pData, const sockaddr_storage* peerAddr)
	{
		return false;
	}

	//-----------------------------------------------
    AsyncTransceiver::Result EPOLLUdpTransceiver::finishWrite(AsyncInfo* pInfo, AsyncData* pData)
	{
		return Result_Error;
	}

	//-----------------------------------------------
	bool EPOLLUdpTransceiver::startReadFrom(AsyncInfo* pInfo, AsyncData* pData, sockaddr_storage* peerAddr)
	{
		return false;
	}

	//-----------------------------------------------
	AsyncTransceiver::Result EPOLLUdpTransceiver::finishRead(AsyncInfo* pInfo, AsyncData* pData)
	{
		return Result_Error;
	}

}

#endif
#endif

