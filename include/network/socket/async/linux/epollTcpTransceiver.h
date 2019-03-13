/*
 * epollTcpTransceiver.h
 *
 *  Created on: Feb 21, 2014
 *      Author: root
 */

//#ifdef HYC_COMPILE_MODULE_NET
//#ifdef CN_NET_EPOLL

#ifndef __BP_EPOLLTCPTRANSCEIVER_H_201402211657_
#define __BP_EPOLLTCPTRANSCEIVER_H_201402211657_

#include "../asyncTransceiver.h"
#include "../../../selector/selector.h"

namespace hyc
{

	/**
	 * @brief startWrite/startRead 中pData可以为NULL.
	 */
	class HYC_API EPOLLTcpTransceiver : public AsyncTransceiver
	{
	public:
		EPOLLTcpTransceiver( Selector& selector, SOCKET socket );
		virtual ~EPOLLTcpTransceiver();

		virtual SocketOperation initialize();
		virtual void			close();

		virtual bool            startWrite( AsyncInfo* pInfo, AsyncData* pData );
		virtual Result          finishWrite( AsyncInfo* pInfo, AsyncData* pData );

		virtual bool            startRead( AsyncInfo* pInfo, AsyncData* pData );
		virtual Result          finishRead( AsyncInfo* pInfo, AsyncData* pData );

		virtual ProtocolType    protocol() const { return Protocol_Tcp; }

	protected:
        Selector&               _rSelector;
	};
}



#endif /* __BP_EPOLLTCPTRANSCEIVER_H_201402211657_ */
//#endif
//#endif
