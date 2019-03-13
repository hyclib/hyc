//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  trace下的方法全是线程安全的.
//

#ifndef __BP__TRACE_HPP_20110621102901__
#define __BP__TRACE_HPP_20110621102901__


#include "common/cnInterface.h"
#include "common/cnObjectPtr.h"
#include "io/log.h"

namespace hyc
{
    typedef enum {
        TraceType_console,
        TraceType_file,
        TraceType_custom,   // 将使用Trace_customLog自定义日志.
    } TraceType;
    
    /**
     * @brief 初始化并开关日志跟踪.
     */
    extern "C" void HYC_API Trace_init( TraceType type, const tchar* pszFile );
    
    /**
     * @brief 是否启用.
     */
    extern "C" void HYC_API Trace_enable( bool enable );
    
    /**
     * @brief 日志跟踪.
     */
    extern "C" void HYC_API Trace_log( LogLevel ll, const tchar* pMsg, const char* file, s32 line, ... );
    extern "C" void HYC_API Trace_log_lite( LogLevel ll, const tchar* pMsg, ... );
    
    /**
     * @brief 标准printf 带锁.
     */
    extern "C" void HYC_API Trace_printf( const tchar* pFormat, ... );
    extern "C" void HYC_API Trace_scans( OUT String& str );

    /**
     * @brief 自定义日志函数.
     */
    typedef void (*TraceCallback)( LogLevel ll, int lastError, const tchar* pMsg, const char* file, s32 line );
    extern "C" void HYC_API Trace_customLog( TraceCallback callback );
     
} // end namespace hyc



#ifdef HYC_COMPILE_MODULE_LOG

    /**
     * print_debug          : debug状态下输出至控制台.
     * print_all            : 任意状态下输出至控制台.
     * trace / trace_debug  : debug状态下记录.
     * trace_all            : 任意状态下记录完整信息(包括行号文路径).
     * trace_lite           : debug状态下记录简单信息.
     * trace_lite_all       : 任意状态下记录简单信息.
     * error / error_debug  : debug状态下记录错误信息.
     * error_all            : 任意状态下记录完整错误信息(包括行号文路径).
     * error_lite           : debug状态下记录简单错误信息.
     * error_lite_all       : 任意状态下记录简单错误信息.
     */


    #ifdef DEBUG
    #   define print_debug(format, ...)         hyc::Trace_printf(format, ##__VA_ARGS__)
    #   define print_debug_line(format, ...)    print_debug(format _T("\n"), ##__VA_ARGS__)
    #   define print_all(format, ...)           hyc::Trace_printf(format, ##__VA_ARGS__)
    #   define print_all_line(format, ...)      print_all(format _T("\n"), ##__VA_ARGS__)

    // 输出信息.
    #   define trace(format, ...)               hyc::Trace_log(hyc::LogLevel_Debug, (format), __FILE__, __LINE__, ##__VA_ARGS__)
    #   define trace_all(format, ...)           hyc::Trace_log(hyc::LogLevel_Info, (format), __FILE__, __LINE__, ##__VA_ARGS__)
    #   define trace_lite(format, ...)          hyc::Trace_log_lite(hyc::LogLevel_Info, (format), ##__VA_ARGS__)
    #   define trace_lite_all(format, ...)      hyc::Trace_log_lite(hyc::LogLevel_Info, (format), ##__VA_ARGS__)

    // 输出错误消息.
    #   define error(format, ...)               hyc::Trace_log(hyc::LogLevel_Error, (format), __FILE__, __LINE__, ##__VA_ARGS__)
    #   define error_all(format, ...)           hyc::Trace_log(hyc::LogLevel_Error, (format), __FILE__, __LINE__, ##__VA_ARGS__)
    #   define error_lite(format, ...)          hyc::Trace_log_lite(hyc::LogLevel_Error, (format), ##__VA_ARGS__)
    #   define error_lite_all(format, ...)      hyc::Trace_log_lite(hyc::LogLevel_Error, (format), ##__VA_ARGS__)

    #else
    #   define print_debug(format, ...)         (0)
    #   define print_debug_line(format, ...)    (0)
    #   define print_all(format, ...)           hyc::Trace_printf(format, ##__VA_ARGS__)
    #   define print_all_line(format, ...)      print_all(format _T("\n"), ##__VA_ARGS__)

    // 输出信息.
    #   define trace(format, ...)               (0)
    #   define trace_all(format, ...)           hyc::Trace_log(hyc::LogLevel_Info, (format), __FILE__, __LINE__, ##__VA_ARGS__)
    #   define trace_lite(format, ...)          (0)
    #   define trace_lite_all(format, ...)      hyc::Trace_log_lite(hyc::LogLevel_Info, (format), ##__VA_ARGS__)

    // 输出错误消息.
    #   define error(format, ...)               (0)
    #   define error_all(format, ...)           hyc::Trace_log(hyc::LogLevel_Error, (format), __FILE__, __LINE__, ##__VA_ARGS__)
    #   define error_lite(format, ...)          (0)
    #   define error_lite_all(format, ...)      hyc::Trace_log_lite(hyc::LogLevel_Error, (format), ##__VA_ARGS__)

    #endif // DEBUG

#else
    #   define print_debug(format, ...)         (0)
    #   define print_debug_line(format, ...)    (0)
    #   define print_all(format, ...)           (0)
    #   define print_all_line(format, ...)      (0)
    #   define trace(format, ...)               (0)
    #   define trace_all(format, ...)           (0)
    #   define trace_lite(format, ...)          (0)
    #   define trace_lite_all(format, ...)      (0)
    #   define error(format, ...)               (0)
    #   define error_all(format, ...)           (0)
    #   define error_lite(format, ...)          (0)
#endif // HYC_COMPILE_MODULE_LOG

#   define trace_debug                      trace
#   define trace_lite_debug                 trace_lite
#   define error_debug                      error
#   define error_lite_debug                 error_lite


#endif // end of define __BP__TRACE_HPP_20110621102901__
