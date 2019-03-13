//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#if defined(HYC_COMPILE_MODULE_LOG) || defined(HYC_COMPLIE_SUPPORT_TRACE)

#include "../../include/io/log.h"
#include "../../include/io/logListener.h"
#include "../../include/io/fileStream.h"
#include "../../include/utils/fileUtils.h"
#include "../../include/os/thread.h"
#include "../../include/os/dateTime.h"
#include "../../include/utils/stringUtils.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma warning(disable: 4661)
#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS


#if CN_PLATFORM == CN_PLATFORM_ANDROID
#   include <android/log.h>
#   define PRINTF(fmt, ...)  __android_log_print(ANDROID_LOG_INFO, "hycjni", fmt, ##__VA_ARGS__)
#elif defined(UNICODE) || defined(_UNICODE)
#   define PRINTF(fmt, ...)  wprintf(fmt, ##__VA_ARGS__)
#else
#   define PRINTF(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#endif



namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	LogFileListener::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class LogFileListener::PrivateClass
    {
    public:
        FileStream  m_fs;
        String      m_strLevel;
#ifdef HYC_COMPILE_MODULE_THREAD
        SyncObject  m_syncObj;
#endif
    };
    
    //----------------------------------------------------------------------------------------------------
    //*	LogFileListener
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    LogFileListener::LogFileListener( const tchar* pszFile )
    {
        PRIVATE_CLASS_CREATE;
        PRIVATE_CLASS_GET(p);
        assert(pszFile);

        // 保证文件存在
        FileUtils::FileAssure( pszFile );

        // 创建fstream
        if ( !p->m_fs.open( pszFile, FileStream::FM_ReadWrite|FileStream::FM_Binary ) ) {
            assert( false );
            return; // 保证程序继续运行
        }

        p->m_fs.seek( 0, FileStream::SO_End );
    }
    
    //-----------------------------------------------
    LogFileListener::~LogFileListener()
    {
        PRIVATE_CLASS_GET(p);
        if ( p->m_fs.isOpen() ) {
            p->m_fs.close();
        }

        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    void LogFileListener::onInvoke( const tchar* message, const Log* log )
    {
        PRIVATE_CLASS_GET(p);
        if ( !log || !message ) return;

#ifdef HYC_COMPILE_MODULE_THREAD
        // 同步锁
        SyncTokenPtr token = p->m_syncObj.getToken();
#endif

        static DateTime dt;
        dt.setToSysTime();

        p->m_strLevel.format(_T("%hu-%hu-%hu %hu:%hu:%hu \n"), 
            dt.getYear(), dt.getMonth(), dt.getDay(), dt.getHours(), dt.getMinutes(), dt.getSeconds());

        // log
        switch ( log->getLogLevel() )
        {
        case LogLevel_Debug:
            p->m_strLevel = _T("[DEBUG] ") + p->m_strLevel;
            break;
        case LogLevel_Error:
            p->m_strLevel = _T("[ERROR] ") + p->m_strLevel;
            break;
        case LogLevel_Info:
            p->m_strLevel = _T("[INFO]  ") + p->m_strLevel;
            break;
        }

        p->m_fs.writeByte( &p->m_strLevel[0], p->m_strLevel.size() );
        p->m_fs.writeByte( message, StringUtils::GetByteSize(message) );
        p->m_fs.writeByte( " \n", 2 );
        p->m_fs.flush();
    }

    //----------------------------------------------------------------------------------------------------
    //*	LogConsoleListener
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    LogConsoleListener::LogConsoleListener()
    {
    }
    
    //-----------------------------------------------
    LogConsoleListener::~LogConsoleListener()
    {
    }

    //-----------------------------------------------
    void LogConsoleListener::onInvoke( const tchar* message, const Log* log )
    {
        if ( !log || !message ) return;

        String str;

        DateTime dt;
        dt.setToSysTime();

        str.format(_T("%hu-%hu-%hu %hu:%hu:%hu\n"),
            dt.getYear(), dt.getMonth(), dt.getDay(), dt.getHours(), dt.getMinutes(), dt.getSeconds());

        // log
        switch ( log->getLogLevel() )
        {
        case LogLevel_Debug:
            str = _T("[DEBUG] ") + str;
            break;
        case LogLevel_Error:
            str = _T("[ERROR] ") + str;
            break;
        case LogLevel_Info:
            str = _T("[INFO]  ") + str;
            break;
        }

        str += message;
        
        PRINTF(_T("%s\n"), str.c_str());
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_LOG
