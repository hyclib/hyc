//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// common libaray
// author:       lipengxiang
// description:  
//

#include "preheader.h"

#include "../include/io/log.h"
#include "../include/io/logListener.h"
#include "../include/os/dateTime.h"
#include "../include/os/thread.h"
#include "_trace.h"
#include "globalVar.hpp"

#if defined(HYC_COMPLIE_SUPPORT_TRACE)
#include "../include/utils/stringUtils.h"
#include "../include/common/cnString.h"
static hyc::TraceCallback    g_traceCustomCallback = NULL;
#endif

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma warning(disable: 4996)
#endif

#if CN_PLATFORM == CN_PLATFORM_ANDROID
#   include <android/log.h>
#   define VPRINTF(fmt, arg)  __android_log_vprint(ANDROID_LOG_INFO, "hycjni", fmt, arg)
#elif defined(UNICODE) || defined(_UNICODE)
#   define VPRINTF(fmt, arg)  vwprintf(fmt, arg)
#else
#   define VPRINTF(fmt, arg)  vprintf(fmt, arg)
#endif

namespace hyc
{
#if defined(HYC_COMPLIE_SUPPORT_TRACE)
    static TraceType    g_traceType = TraceType_console;
    static bool         g_traceEnable = true;
    static integer      g_traceThreadId = 0;
#endif // HYC_COMPLIE_SUPPORT_TRACE

    //-----------------------------------------------
    void HYC_API Trace_log_lite( LogLevel ll, const tchar* pMsg, ... )
    {
    	if (!g_pTraceLog && TraceType_custom != g_traceType)   return;

        int err = Platform::GetLastError();

#if defined(HYC_COMPLIE_SUPPORT_TRACE)
        if (!g_traceEnable) return;

        String str2;
        va_list arg_ptr;
        va_start(arg_ptr, pMsg);
        str2.vformat(pMsg, arg_ptr);
        va_end(arg_ptr);

        if (TraceType_custom == g_traceType)
        {
            if (g_traceCustomCallback)
                g_traceCustomCallback(ll, err, str2.c_str(), "", 0);
            return;
        }

        cString tempStr;
        if (LogLevel_Info != ll)
            tempStr.format("  Code: (%d)%s \n", err, Platform::GetLastErrorMsg(err).c_str());
        tempStr += "  Msg: ";

#if defined(_UNICODE) || defined(UNICODE)
        String str;
        StringUtils::Narrow2wide(tempStr, str);
#else
        String& str = tempStr;
#endif

        str += str2;
        str += _T(" \n");

        g_pTraceSync->lock();
        assert(g_traceThreadId == 0 || g_traceThreadId == Thread::CurThreadId());
        g_traceThreadId = Thread::CurThreadId();
        g_pTraceLog->setLogLevel(ll);
        g_pTraceLog->log(str.c_str());
        g_traceThreadId = 0;
        g_pTraceSync->unlock();

#endif // HYC_COMPLIE_SUPPORT_TRACE
    }

    //-----------------------------------------------
    void Trace_log( LogLevel ll, const tchar* pMsg, const char* file, s32 line, ... )
    {
    	if (!g_pTraceLog && TraceType_custom != g_traceType)   return;

        int err = Platform::GetLastError();

#if defined(HYC_COMPLIE_SUPPORT_TRACE)
        if (!g_traceEnable) return;

        String str2;
        va_list arg_ptr;
        va_start(arg_ptr, line);
        str2.vformat(pMsg, arg_ptr);
        va_end(arg_ptr);

        if (TraceType_custom == g_traceType)
        {
            if (g_traceCustomCallback)
                g_traceCustomCallback(ll, err, str2.c_str(), file, line);
            return;
        }

        cString tempStr;
        if (LogLevel_Info == ll)
        {
            tempStr.format("  File: (%d) %s;\n",
                line, file);
        }
        else
        {
            tempStr.format("  Code: (%d)%s \n  File: (%d) %s;\n",
                err, Platform::GetLastErrorMsg(err).c_str(),
                line, file);
        }
        tempStr += "  Msg: ";

#if defined(_UNICODE) || defined(UNICODE)
        String str;
        StringUtils::Narrow2wide(tempStr, str);
#else
        String& str = tempStr;
#endif

        str += str2;
        str += _T(" \n");
 
        g_pTraceSync->lock();
        assert(g_traceThreadId == 0 || g_traceThreadId == Thread::CurThreadId());
        g_traceThreadId = Thread::CurThreadId();
        g_pTraceLog->setLogLevel(ll);
        g_pTraceLog->log(str.c_str());
        g_traceThreadId = 0;
        g_pTraceSync->unlock();

#endif // HYC_COMPLIE_SUPPORT_TRACE
    }

    //-----------------------------------------------
    void Trace_init( TraceType type, const tchar* pszFile )
    {
#ifdef HYC_COMPLIE_SUPPORT_TRACE
        g_traceType = type;
        if (TraceType_file == type)
        {
            SAFE_DELETE(g_pTraceLog);
            g_pTraceLog = new Log();
            g_pTraceLog->addListener(new LogFileListener(pszFile));
        }
        else if (TraceType_console == type)
        {
            SAFE_DELETE(g_pTraceLog);
            g_pTraceLog = new Log();
            g_pTraceLog->addListener(new LogConsoleListener());
        }
        else
        {
            SAFE_DELETE(g_pTraceLog);
        }
#endif // HYC_COMPLIE_SUPPORT_TRACE
    }

    //-----------------------------------------------
    void HYC_API Trace_enable( bool enable )
    {
#ifdef HYC_COMPLIE_SUPPORT_TRACE
        g_traceEnable = enable;
#endif
    }

    //-----------------------------------------------
    void HYC_API Trace_printf( const tchar* pFormat, ... )
    {
#ifdef HYC_COMPLIE_SUPPORT_TRACE

        va_list arg_ptr;
        va_start(arg_ptr, pFormat);
        
        g_pTraceSync->lock();
        assert(g_traceThreadId == 0 || g_traceThreadId == Thread::CurThreadId());
        g_traceThreadId = Thread::CurThreadId();
        VPRINTF(pFormat, arg_ptr);  // printf 只是信号安全, 实际操作FILE 还是不安全的.
        g_traceThreadId = 0;
        g_pTraceSync->unlock();

        va_end(arg_ptr);

#endif
    }

    //-----------------------------------------------
    void HYC_API Trace_scans( OUT String& str )
    {
#ifdef HYC_COMPLIE_SUPPORT_TRACE

#if defined(_UNICODE) || defined(UNICODE)
        wchar_t buf[512] = {0};
        wscanf(L"%s", buf);
#else
        char buf[512] = {0};
        scanf("%s", buf);
#endif 
        //g_pTraceSync->lock();
        //g_pTraceSync->unlock();

        str = buf;

#endif
    }

#if defined(HYC_COMPLIE_SUPPORT_TRACE)
    void Trace_release(integer threadId)
    {
        if (g_traceThreadId == threadId)
        {
            g_traceThreadId = 0;
            g_pTraceSync->unlock();
        }
    }

#endif

    //-----------------------------------------------
    extern "C" void HYC_API Trace_customLog( TraceCallback callback )
    {
#ifdef HYC_COMPLIE_SUPPORT_TRACE
        g_traceCustomCallback = callback;
#endif
    }


} // end namespace hyc
