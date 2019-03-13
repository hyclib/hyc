/*
 * epollUdpTransceiver.h
 *
 *  Created on: Feb 21, 2014
 *      Author: root
 */

//#ifdef HYC_COMPILE_MODULE_NET
//#ifdef CN_NET_EPOLL


#ifndef __BP_EPOLLUDPTRANSCEIVER_H_201402211706_
#define __BP_EPOLLUDPTRANSCEIVER_H_201402211706_

#include "../asyncTransceiver.h"
#include "../../../selector/selector.h"

namespace hyc
{

	/**
	 * @brief startWrite/startRead 中pData可以为NULL.
	 */
	class HYC_API EPOLLUdpTransceiver : public AsyncTransceiver
	{
	public:
		EPOLLUdpTransceiver(Selector& selector);
		virtual ~EPOLLUdpTransceiver();

		virtual SocketOperation initialize();
		virtual void			close();

		virtual bool			doServer( const String& host, int port, ProtocolSupport proto, const String& mcastInterface = _T(""));

		virtual bool            startWriteTo( AsyncInfo* pInfo, AsyncData* pData, const struct sockaddr_storage* peerAddr );
		virtual Result          finishWrite( AsyncInfo* pInfo, AsyncData* pData );
		virtual bool            startReadFrom( AsyncInfo* pInfo, AsyncData* pData, INOUT struct sockaddr_storage* peerAddr );
		virtual Result          finishRead( AsyncInfo* pInfo, AsyncData* pData );

		virtual ProtocolType    protocol() const    { return Protocol_Udp; }

		int						effectivePort() const;

	private:
		virtual bool            startWrite( AsyncInfo* pInfo, AsyncData* pData )   { return false; }
		virtual bool            startRead( AsyncInfo* pInfo, AsyncData* pData )   { return false; }

	protected:
        Selector&                       _rSelector;
		struct sockaddr_storage			_addr;
		struct sockaddr_storage			_mcastAddr;
		String							_mcastInterface;

	};
}



#endif /* __BP_EPOLLUDPTRANSCEIVER_H_201402211706_ */
//#endif
//#endif
