//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  日志
//

#if defined(HYC_COMPILE_MODULE_LOG) || defined(HYC_COMPLIE_SUPPORT_TRACE)

#ifndef __BP_LOG_H_2010121495710__
#define __BP_LOG_H_2010121495710__

#include "logListener.h"

namespace hyc
{
    class LogListener;

    //-----------------------------------------------
    typedef enum {
        LogLevel_Info,    // normal infomation; (default)
        LogLevel_Error,
        LogLevel_Debug,   // only can be log in debug mode
    } LogLevel;

    //----------------------------------------------------------------------------------------------------
    //*	Log
    //----------------------------------------------------------------------------------------------------
     
    class HYC_API Log : public INoncopyable, public RefCount
    {
        PRIVATE_CLASS_DECLARE;
    public:
        Log();
        ~Log();

    public:
        /*---------------------------
          listener 管理; 将管理listener的生命期
        ---------------------------*/
        void            addListener( LogListener* listener );
        void            removeListener( LogListener* listener );
        u32             getListenerNum() const;
        LogListener*    getListener( u32 index );

        /*---------------------------
          logLevel 管理
        ---------------------------*/
        const LogLevel& getLogLevel() const;
        void            setLogLevel( const LogLevel& level );

        /*---------------------------
          日志
        ---------------------------*/
        u32             getMsgMaxLength() const;
        void            setMsgMaxLength( u32 length );
        void            log( const tchar* message );
        void            logf( const tchar* format, ... );
        void            vlogf( const tchar* format, va_list _ArgList );

    };
    typedef ObjectPtr<Log>  LogPtr;

	

} // end namespace hyc

#endif // end of define __BP_LOG_H_2010121495710__

#endif // HYC_COMPILE_MODULE_LOG
