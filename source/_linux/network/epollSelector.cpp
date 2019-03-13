/*
 * epollSelector.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: root
 */

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_EPOLL) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/selector/epollSelector.h"
#include "../../../include/network/socket/socketUtils.h"

#define EPOLL_MAX_EV 64

namespace hyc
{
	//-----------------------------------------------
	EpollSelector::EpollSelector()
	 : _fd(-1)
	{
		_fd = epoll_create1(0);
		if(_fd < 0)
		{
			 error_all( "Error EpollSelector::setup: epoll_create!" );
			 assert(0);
			 exit(-1);
		}
	}

	//-----------------------------------------------
	EpollSelector::~EpollSelector()
	{
		destroy();
	}

	//-----------------------------------------------
	void EpollSelector::destroy()
	{
		if (_fd > 0)
		{
            SocketUtils::CloseSocket(_fd);
			_fd = -1;
		}
	}

	//-----------------------------------------------
	void EpollSelector::remove(SelectorEvent* handler, AsyncInfo* pRemoveInfo)
	{
		if (_fd <= 0 || !handler || !pRemoveInfo) return;
        assert(pRemoveInfo->__pHandler == handler);

		if (0 == handler->_registered) return;

        struct epoll_event ev;
		//ev.events = 0;
        //ev.data.ptr = handler;
		if(epoll_ctl(_fd, EPOLL_CTL_DEL, handler->socket(), &ev) != 0)
		{
			error_all( "Error EpollSelector::removeOp: epoll_ctl!" );
		}

		handler->_registered = 0;
	}

	//-----------------------------------------------
	void EpollSelector::enable(SelectorEvent* handler, AsyncInfo* pInfo)
	{
		if (_fd <= 0  || !handler || !pInfo) return;
        assert(pInfo->__pHandler == handler);

        u32& mark = handler->_registered;

        bool isMod = mark & EPOLLET;
        mark |= EPOLLET;

        if (handler->_isReadInfo(pInfo))
        {
            if (mark & EPOLLIN)
                return;

            mark |= EPOLLIN;
        }
        else if (handler->_isWriteInfo(pInfo))
        {
            if (mark & EPOLLOUT)
                return;

            mark |= EPOLLOUT;
        }
        else
        {
            error_all("Error EpollSelector::enable: Error pInfo");
            assert(0);
            return;
        }

        struct epoll_event ev;
        ev.events = mark;
        ev.data.ptr = handler;

        if(epoll_ctl(_fd, (isMod ? EPOLL_CTL_MOD : EPOLL_CTL_ADD), handler->socket(), &ev) != 0)
        {
        	if (isMod)
        		mark &= handler->_isReadInfo(pInfo) ? ~EPOLLIN : ~EPOLLOUT;
        	else
        		mark = 0;

            return;
        }
	}

	//-----------------------------------------------
	void EpollSelector::disable(SelectorEvent* handler, AsyncInfo* pInfo)
	{
		if (_fd <= 0  || !handler || !pInfo) return;
        assert(pInfo->__pHandler == handler);

        u32& mark = handler->_registered;
        assert(mark & EPOLLET);

        if (handler->_isReadInfo(pInfo))
        {
            if (!(handler->_registered & EPOLLIN))
                return;

            if (!(handler->_registered & EPOLLOUT))
            {
            	remove(handler, pInfo);
            	return;
            }
            mark &= ~EPOLLIN;
        }
        else if (handler->_isWriteInfo(pInfo))
        {
            if (!(handler->_registered & EPOLLOUT))
                return;

            if (!(handler->_registered & EPOLLIN))
            {
            	remove(handler, pInfo);
            	return;
            }
            mark &= ~EPOLLOUT;
        }
        else
        {
            error_all("Error EpollSelector::disable: Error op");
            assert(0);
            return;
        }

        struct epoll_event ev;
        ev.events = mark;
        ev.data.ptr = handler;

        if(epoll_ctl(_fd, EPOLL_CTL_MOD, handler->socket(), &ev) != 0)
        {
        	if (handler->_isReadInfo(pInfo))
        		mark |= EPOLLIN;
        	else
        		mark |= EPOLLOUT;

            trace( "Error EpollSelector::disable: epoll_ctl!" );
            return;
        }
	}

	//-----------------------------------------------
	void EpollSelector::select(OUT Array<SelectorEventInfo>* arrFd,
			const s32& timeout)
	{
		arrFd->clear();

		SelectorEventInfo sInfo;
		struct epoll_event events[EPOLL_MAX_EV];

		while (true) {
			int ret = epoll_wait(_fd, events, EPOLL_MAX_EV, timeout);
			if (ret == -1) {
				if (errno == EINTR)
					continue;
				else {
					error_all("Error EpollSelector::select: epoll_wait(-1)!");
				}
			} else if (ret == 0) {
				trace("epoll select time out!");
			} else {
				for (int i = 0; i < ret; i++) {
					struct epoll_event& ev = events[i];
					sInfo.pEvent = static_cast<SelectorEvent*>(ev.data.ptr);
					if (ev.events & EPOLLIN) {
						sInfo.pInfo = &sInfo.pEvent->_recvInfo;
						arrFd->push_back(sInfo);
					}
					if (ev.events & EPOLLOUT) {
						sInfo.pInfo = &sInfo.pEvent->_writeInfo;
						arrFd->push_back(sInfo);
					}
				}
			}

			break;

		} // while.
	}
}

#endif
#endif
