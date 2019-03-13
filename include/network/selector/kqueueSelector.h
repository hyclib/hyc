//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_KQUEUESELECTOR_H_20130321220212__
#define __BP_KQUEUESELECTOR_H_20130321220212__
#ifdef CN_NET_KQUEUE

#include "selector.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //! selector implements for kqueue
    //----------------------------------------------------------------------------------------------------
    class HYC_API KqueueSelector : public Selector
    {
    public:
        KqueueSelector();
        ~KqueueSelector();

        virtual void    destroy();
        virtual void    initialize( SelectorEvent* handler );

        //! remove the operator of handler which is pending to deal
        virtual void    remove( SelectorEvent* handler, AsyncInfo* pRemoveInfo );
        virtual void    enable( SelectorEvent* handler, AsyncInfo* pInfo );
        virtual void    disable( SelectorEvent* handler, AsyncInfo* pInfo );

        //! get the next message, status hint the message' behaviour
        virtual void    select( OUT Array<SelectorEventInfo>* arrFd, int timeout );

    private:
        Array<struct kevent> _events;
        Array<struct kevent> _changes;
        bool                 _selecting;
    };

} // end namespace hyc

#endif // CN_NET_KQUEUE
#endif // end of define __BP_KQUEUESELECTOR_H_20130321220212__