//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  日志监听者
//

#if defined(HYC_COMPILE_MODULE_LOG) || defined(HYC_COMPLIE_SUPPORT_TRACE)

#ifndef __BP_LOGLISTENER_H_20101214101107__
#define __BP_LOGLISTENER_H_20101214101107__


namespace hyc
{
    class Log;

	//----------------------------------------------------------------------------------------------------
	//*	LogListener; Derive from this class
	//----------------------------------------------------------------------------------------------------

    class HYC_API LogListener : public INoncopyable
    {
    public:
        virtual ~LogListener() {}

        // when message log, invoke will run
        virtual void    onInvoke( const tchar* message, const Log* log ) = 0;
    };

    
    //----------------------------------------------------------------------------------------------------
    //*	LogFileListener; 文件日志监听者, 能够将日志信息记录至文件
    //----------------------------------------------------------------------------------------------------
    class HYC_API LogFileListener : public LogListener
    {
        PRIVATE_CLASS_DECLARE;
    public:
        LogFileListener( const tchar* pszFile );
        ~LogFileListener();

    public:
        // when message log, invoke will run
        virtual void    onInvoke( const tchar* message, const Log* log );
    };

        
    //----------------------------------------------------------------------------------------------------
    //*	LogConsoleListener; 控制台日志监听者, 能够将日志信息记录至控制台
    //----------------------------------------------------------------------------------------------------
    class HYC_API LogConsoleListener : public LogListener
    {
    public:
        LogConsoleListener();
        ~LogConsoleListener();

    public:
        // when message log, invoke will run
        virtual void    onInvoke( const tchar* message, const Log* log );
    };
     


} // end namespace hyc

#endif // end of define __BP_LOGLISTENER_H_20101214101107__

#endif // HYC_COMPILE_MODULE_LOG
