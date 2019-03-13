//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_SELECTOR_H_20130321145855__
#define __BP_SELECTOR_H_20130321145855__

#include "selectorEvent.h"

namespace hyc
{

    /**
     * selector handler info for select.
     */
    struct HYC_API SelectorEventInfo{
        SelectorEvent*      pEvent;
        AsyncInfo*          pInfo;
    };

    /**
     * selector use to transceiver message
     */
    class HYC_API Selector
    {
    public:
        virtual ~Selector() {}

        virtual void    destroy() = 0;

        //! remove the operator of handler which is pending to deal
        //! remove op don't do a callback for pending handler, It's drop the handler immediately,
        //! you must deal the thing e.g. free memory;
        virtual void    initialize( SelectorEvent* handler ) = 0;
        virtual void    remove( SelectorEvent* handler, AsyncInfo* pRemoveInfo ) = 0;
        virtual void    enable( SelectorEvent* handler, AsyncInfo* pInfo )  {}
        virtual void    disable( SelectorEvent* handler, AsyncInfo* pInfo ) {}

        //! get the next message, status hint the message' behaviour
        virtual void    select( OUT Array<SelectorEventInfo>* arrFd, const s32& timeout ) = 0;
    };

} // end namespace hyc

#endif // end of define __BP_SELECTOR_H_20130321145855__