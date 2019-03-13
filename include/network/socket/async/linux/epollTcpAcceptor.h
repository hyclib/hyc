/*
 * epollTcpAcceptor.h
 *
 *  Created on: Feb 21, 2014
 *      Author: root
 */


//#ifdef HYC_COMPILE_MODULE_NET
//#ifdef CN_NET_EPOLL

#include "../../../../hycPlatform.h"
#include "../../../netPlatform.h"

#ifndef __BP_EPOLLTCPACCEPTOR_H_201402211457_
#define __BP_EPOLLTCPACCEPTOR_H_201402211457_

#include "../../tcpAcceptor.h"
#include "epollTcpTransceiver.h"

namespace hyc
{
	//----------------------------------------------------------------------------------------------------
    //!	EPOLL 异步连接接受器
    //----------------------------------------------------------------------------------------------------
	class HYC_API EPOLLTcpAcceptor : public TcpAcceptor
	{
	public:
		EPOLLTcpAcceptor( Selector& selector, bool isBlock );

		EPOLLTcpAcceptor( Selector& selector, bool isBlock, SOCKET socket, bool bAlreadyBinded );

		virtual ~EPOLLTcpAcceptor();

		virtual void		close();
		virtual bool		listen( const String& host, int port, ProtocolSupport protocol );

		void				startAccept(AsyncInfo* pInfo);
		AsyncTransceiverPtr	doAccept();
		void				finishAccept(AsyncInfo* pInfo);

		//
		AsyncTransceiverPtr	acceptBlock();

	protected:
		virtual TransceiverPtr	accept()	{ throw "no implements"; }

	protected:
		Selector& 			_rSelector;
		bool				_block;
	};
}

#endif /* __BP_EPOLLTCPACCEPTOR_H_201402211457_ */
//#endif
//#endif
