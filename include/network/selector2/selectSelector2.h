//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_SELECTSELECTOR_H_20140711143917__
#define __BP_SELECTSELECTOR_H_20140711143917__

#include "selector2.h"
#include "../../os/thread.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //! selector implements for select mode.
    //----------------------------------------------------------------------------------------------------
    class HYC_API SelectSelector2 : public Selector2
    {
    public:
        SelectSelector2();
        ~SelectSelector2();

        virtual void    clear();

        virtual void    initialize( TransceiverPtr pTrans );
        virtual void    remove( TransceiverPtr pTrans );

        //! get the next message, arrFd' elements have new message, can read.
        virtual void    select( 
                                INOUT Array<TransceiverPtr>* arrReadFd,
                                INOUT Array<TransceiverPtr>* arrWriteFd,
                                INOUT Array<TransceiverPtr>* arrExceptFd,
                                const s32& timeout );

    private:
        fd_set                     _fdsRead;
        fd_set                     _fdsWrite;
        fd_set                     _fdsExcp;
        ArrayFind<TransceiverPtr>  _transs;
        SyncObject                 _sync;
    };

} // end namespace hyc

#endif // end of define __BP_SELECTSELECTOR_H_20140711143917__