//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_EPOLLSELECTOR_H_20130321214816__
#define __BP_EPOLLSELECTOR_H_20130321214816__
#ifdef CN_NET_EPOLL

#include "selector.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //! selector implements for epoll
    //! all operator must be in single thread.
    //----------------------------------------------------------------------------------------------------
    class HYC_API EpollSelector : public Selector
    {
    public:
        EpollSelector();
        virtual ~EpollSelector();

        virtual void    destroy();
        virtual void    initialize( SelectorEvent* handler ) {}

        //! remove the operator of handler which is pending to deal
        virtual void    remove( SelectorEvent* handler, AsyncInfo* pRemoveInfo );
        virtual void    enable( SelectorEvent* handler, AsyncInfo* pInfo );
        virtual void    disable( SelectorEvent* handler, AsyncInfo* pInfo );

        //! get the next message, status hint the message' behaviour
        virtual void    select( OUT Array<SelectorEventInfo>* arrFd, const s32& timeout );

    private:
        int	_fd;
    };

} // end namespace hyc

#endif // CN_NET_EPOLL
#endif // end of define __BP_EPOLLSELECTOR_H_20130321214816__
