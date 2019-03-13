/*
 * EPOLLTcpTransceiver.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: root
 */

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_EPOLL) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/socket/netStatus.h"
#include "../../../include/network/socket/async/linux/epollTcpTransceiver.h"

namespace hyc
{
	//----------------------------------------------------------------------------------------------------
    //*	EPOLLTcpTransceiver
    //----------------------------------------------------------------------------------------------------

	//-----------------------------------------------
	EPOLLTcpTransceiver::EPOLLTcpTransceiver(Selector& selector, s32 socket)
        : _rSelector(selector)
	{
		_socket = socket;
		SocketUtils::SetBlock(_socket, false);
		//SocketUtils::SetRevBufSize(socket, YUN_CONTENT_MAX_LENGTH);
	}

	//-----------------------------------------------
	EPOLLTcpTransceiver::~EPOLLTcpTransceiver()
	{
	}

	//-----------------------------------------------
	hyc::SocketOperation EPOLLTcpTransceiver::initialize()
	{
		return SocketOperation_None;
	}

	//-----------------------------------------------
	void EPOLLTcpTransceiver::close()
	{
		if (_socket != INVALID_SOCKET)
		{
			// shutdown
			if(shutdown(_socket, SHUT_RDWR) == SOCKET_ERROR)
			{
				if (errno != ENOTCONN)
				{
					trace(_T("close error"));
				}
			}

			//
			Transceiver::close();
		}
	}

	//-----------------------------------------------
	bool EPOLLTcpTransceiver::startWrite(AsyncInfo* pInfo, AsyncData* pData)
	{
		/*if (!pInfo) return false;

		assert(pInfo->__pHandler->_isWriteInfo(pInfo));
		pInfo->inError = 0;
		pInfo->inCountBytes = 0;

		_rSelector.enable(pInfo->__pHandler, pInfo);*/

		assert( pInfo->__pHandler->_registered & (EPOLLET|EPOLLOUT) );

		return true;
	}

	//-----------------------------------------------
	AsyncTransceiver::Result EPOLLTcpTransceiver::finishWrite(AsyncInfo* pInfo, AsyncData* pData)
	{
		if (!pData || !pInfo) return Result_Error;

		ByteStream* buf = pData->pBuf;

        assert(!buf->empty());

        Byte* begin = pData->pPosWriteAll ? pData->pPosWriteAll : buf->_curPos;
        assert(begin >= buf->begin() && begin < buf->end());

        int packetSize = static_cast<int>(buf->end() - begin);

        s32 len = send(_socket, (char*)begin, packetSize, MSG_NOSIGNAL);  // 防止管道破裂信号.
        if (len == SOCKET_ERROR)
        {
            pInfo->inCountBytes = len;
            s32 err = NetStatus::ErrorCode();

            if (NetStatus::IsWouldBlock(err))
            {
            	pInfo->inCountBytes = 0;
                return Result_WouldBlock;
            }
            else
            {
                pInfo->inError = err;
                return Result_Error;
            }
        }
        else
        {
        	pInfo->inCountBytes = len;

        	if (pData->pPosWriteAll)
        		pData->pPosWriteAll += len;
        	else
        		buf->_curPos += len;

        	if (len < packetSize)
        		return Result_WouldBlock;
        	else
        		return Result_moreData;
        }
    }

	//-----------------------------------------------
	bool EPOLLTcpTransceiver::startRead(AsyncInfo* pInfo, AsyncData* pData)
	{
		/*if (!pInfo) return false;

		assert(pInfo->__pHandler->_isReadInfo(pInfo));

		pInfo->inError = 0;
		pInfo->inCountBytes = 0;

		_rSelector.enable(pInfo->__pHandler, pInfo);*/

		assert( pInfo->__pHandler->_registered & (EPOLLET|EPOLLIN) );


		return true;
	}

	//-----------------------------------------------
	AsyncTransceiver::Result EPOLLTcpTransceiver::finishRead(AsyncInfo* pInfo, AsyncData* pData)
	{
		if (!pData || !pInfo) return Result_Error;

		ByteStream* buf = pData->pBuf;

		assert(!buf->empty());

		Byte* begin = pData->pPosWriteAll ? pData->pPosWriteAll : buf->_curPos;
		if (!begin)
		{
			buf->_curPos = buf->begin();
			begin = buf->_curPos;
		}

		assert(begin >= buf->begin() && begin < buf->end());

        s32 bufLen = buf->end()-begin;

        s32 len = recv(_socket, begin, bufLen, 0);
        pInfo->inCountBytes = len;
        if (len > 0)
        {
        	if (pData->pPosWriteAll)
        		pData->pPosWriteAll += len;
        	else
        		buf->_curPos += len;
        }
        else if (len == -1)
        {
        	s32 err = NetStatus::ErrorCode();

        	if (NetStatus::IsWouldBlock(err))
        	{
        		pInfo->inCountBytes = 0;
                return Result_WouldBlock;
        	}
            else
            {
                pInfo->inError = err;
                return Result_Error;
            }
        }

        if (len < bufLen)
            return Result_WouldBlock;
        else
            return Result_moreData;
	}
}


#endif
#endif


