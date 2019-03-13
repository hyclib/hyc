//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  进程处理
//

#ifdef HYC_COMPILE_MODULE_PROCESS

#ifndef __BP_PROCESS_H_20101207152047__
#define __BP_PROCESS_H_20101207152047__

#include "../container/array.h"

namespace hyc
{
    //-----------------------------------
    //! 进程信息
    typedef struct ProcessInfo{
        ProcessInfo() : m_pid(0)   {}
        bool operator==(const ProcessInfo& info) const { return m_pid==info.m_pid && m_name==info.m_name; }
        integer     m_pid;
        String  m_name;
    } ProcessInfo;

    //-----------------------------------
	class HYC_API Process
    {
    public:

        /*---------------------------
          进程信息
        ---------------------------*/
        static void GetCurProcess( OUT ProcessInfo& info );
        static void GetAllProcess( OUT Array<ProcessInfo>* arrInfo );

        // mac os & iOS 平台不支持
#if CN_PLATFORM != CN_PLATFORM_IOS && CN_PLATFORM != CN_PLATFORM_MACOSX
        /*---------------------------
          启动进程;  
        ---------------------------*/
        static bool RunBlock( const tchar* pszExePath, const tchar* pszCmdLine = NULL );    //! 阻塞, 启动的进程结束了才返回（阻塞运行工作目录为当前进程目录）
        static bool Run( const tchar* pszExePath, const tchar* pszCmdLine = NULL, const tchar* pszRunDir = NULL );         //! 非阻塞
#endif
        
        /*---------------------------
          关闭进程
        ---------------------------*/
        static bool Kill( integer pid );
        static bool Shutdown(); // 关机 (iOS 不支持)
        static bool Reboot();   // 重启 (iOS 不支持)
    };

} // end namespace hyc

#endif // end of define __BP_PROCESS_H_20101207152047__
#endif // HYC_COMPILE_MODULE_PROCESS
