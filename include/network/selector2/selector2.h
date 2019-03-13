//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_SELECTOR2_H_20140711154558__
#define __BP_SELECTOR2_H_20140711154558__


namespace hyc
{
    /**
     * selector2 simple use transceiver to polling the message.
     * unlike selector, selector2 can't detect the new connect.
     */
    class HYC_API Selector2
    {
    public:
        virtual ~Selector2() {}

        virtual void    clear() = 0;

        virtual void    initialize( TransceiverPtr pTrans ) = 0;
        virtual void    remove( TransceiverPtr pTrans ) = 0;

        //! get the next message, arrFd' elements have new message, can read/write/exception.
        virtual void    select( 
                                INOUT Array<TransceiverPtr>* arrReadFd,
                                INOUT Array<TransceiverPtr>* arrWriteFd,
                                INOUT Array<TransceiverPtr>* arrExceptFd,
                                const s32& timeout ) = 0;
    };


} // end namespace hyc

#endif // end of define __BP_SELECTOR2_H_20140711154558__