//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_IOCPSELECTOR_H_20130321151009__
#define __BP_IOCPSELECTOR_H_20130321151009__

#include "selector.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //! selector implements for IOCP
    //----------------------------------------------------------------------------------------------------
    class HYC_API IOCPSelector : public Selector
    {
    public:
        IOCPSelector(); // create IOCP
        virtual ~IOCPSelector();

        virtual void    destroy();
        virtual void    initialize( SelectorEvent* handler ); //! 绑定handler中的socket至IOCP
 
        // 从iocp中删除等待的操作
        virtual void    remove( SelectorEvent* handler, AsyncInfo* pRemoveInfo );

        virtual void    select( OUT Array<SelectorEventInfo>* arrFd, const s32& timeout );
        HANDLE          getIOCPHandler() { return _handle; }
    private:
        HANDLE _handle;
    };

} // end namespace hyc

#endif // end of define __BP_IOCPSELECTOR_H_20130321151009__
