//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// base libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_SELECTOREVENT_H_20130321150054__
#define __BP_SELECTOREVENT_H_20130321150054__

#include "../../common/cnObjectPtrSafe.h"

namespace hyc
{
    /**
     * use to deal message, or start a asynchronous operation 
     *  IOCP: startAsync -> finishAsync -> startAsync -> finishAsync ...
     *  EPOLL: startAsync -> finishAsync -> finishAsync ...
     */
    class HYC_API SelectorEvent : public RefCountSafe
    {
    public:
        SelectorEvent()
#ifdef CN_NET_EPOLL
            : _registered(0)
    		, _recvInfo(this)
    		, _writeInfo(this)
#endif
        {}

        virtual ~SelectorEvent()  {}

        //
        //! Get the socket
        //
        virtual SOCKET      socket() = 0;

        //
        //! Call to start a new asynchronous read or write operation.
        //
        virtual bool        startAsync( AsyncInfo* pInfo, AsyncData* pData ) = 0;

        //
        //! Called when the asynchronous operation is finish.
        //
        virtual void        finishAsync( AsyncInfo* pInfo ) = 0;

#ifdef CN_NET_EPOLL
        inline bool 		_isReadInfo( AsyncInfo* pInfo ) const { return pInfo == &_recvInfo; }
        inline bool 		_isWriteInfo( AsyncInfo* pInfo ) const { return pInfo == &_writeInfo; }

        u32                 _registered; // 已经注册的操作.
        AsyncInfo           _recvInfo;
        AsyncInfo			_writeInfo;
        volatile integer    _writeMark;
#endif
    };

    typedef ObjectPtr<SelectorEvent>    SelectorEventPtr;

} // end namespace hyc

#endif // end of define __BP_SELECTOREVENT_H_20130321150054__
