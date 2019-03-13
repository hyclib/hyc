/*
 * epollTcpAcceptor.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: root
 */


#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_EPOLL) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/socket/async/linux/epollTcpAcceptor.h"
#include "../../../include/network/socket/async/linux/epollTcpTransceiver.h"

namespace hyc
{
	//----------------------------------------------------------------------------------------------------
    //*	EPOLLTcpAcceptor
    //----------------------------------------------------------------------------------------------------

	//-----------------------------------------------
	EPOLLTcpAcceptor::EPOLLTcpAcceptor(Selector& selector, bool isBlock)
		: _rSelector(selector)
		, _block(isBlock)
	{
	}

	//-----------------------------------------------
	EPOLLTcpAcceptor::EPOLLTcpAcceptor(Selector& selector, bool isBlock, s32 socket, bool bAlreadyBinded)
		: TcpAcceptor(socket, bAlreadyBinded)
		, _rSelector(selector)
		, _block(isBlock)
	{
	}

	//-----------------------------------------------
	EPOLLTcpAcceptor::~EPOLLTcpAcceptor()
	{
	}

	//-----------------------------------------------
	bool EPOLLTcpAcceptor::listen( const String& host, int port, ProtocolSupport protocol )
	{
		if (!TcpAcceptor::listen(host, port, protocol))
			return false;

		if (SOCKET_ERROR != _socket && !_block)
		{
			SocketUtils::SetBlock(_socket, false);
		}
		return true;
	}

	//-----------------------------------------------
	void EPOLLTcpAcceptor::close()
	{
		TcpAcceptor::close();
	}

	//-----------------------------------------------
	void EPOLLTcpAcceptor::startAccept( AsyncInfo* pInfo )
	{
		assert(pInfo->__pHandler->_registered == EPOLLIN | EPOLLET);
	}

	//-----------------------------------------------
	AsyncTransceiverPtr EPOLLTcpAcceptor::doAccept()
	{
		SOCKET fd = SocketUtils::Accept(_socket);
		if (fd == INVALID_SOCKET) return NULL;
		//if (NetStatus::IsWouldBlock(NetStatus::ErrorCode()))
		//	return NULL;

		AsyncTransceiverPtr pTrans = new EPOLLTcpTransceiver(_rSelector, fd);
		if (SocketOperation_None != pTrans->initialize())
		{
			pTrans->close();
			return NULL;
		}
		return pTrans;
	}

	//-----------------------------------------------
	void EPOLLTcpAcceptor::finishAccept( AsyncInfo* pInfo )
	{
	}

	//-----------------------------------------------
	AsyncTransceiverPtr EPOLLTcpAcceptor::acceptBlock()
	{
		SOCKET fd;
		fd = SocketUtils::Accept(_socket);
		if (SOCKET_ERROR == fd)
			return NULL;
		return new EPOLLTcpTransceiver(_rSelector, fd);
	}

}

#endif //
#endif // end HYC_COMPILE_MODULE_NET
