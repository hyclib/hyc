//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  Process的win32平台实现
//

#ifndef __BP_PROCESS_HPP_20101207152643__
#define __BP_PROCESS_HPP_20101207152643__

#include <process.h>
#include <tlhelp32.h>    // 快照函数头文件

#include "../../include/os/process.h"
#include "../../include/utils/fileUtils.h"

namespace hyc
{
	

    //-----------------------------------------------
    void Process::GetCurProcess( OUT ProcessInfo& info )
    {
        info.m_pid = _getpid();

        static String strProcessName;
        if (strProcessName.empty())
        {
            tchar pn[MAX_PATH] = {0};
            ::GetModuleFileName(NULL, pn, MAX_PATH);
            strProcessName = pn;
        }
        
        info.m_name =strProcessName;
    }

    //-----------------------------------------------
    void Process::GetAllProcess( OUT Array<ProcessInfo>* parrInfo )
    {
        Array<ProcessInfo>& arrInfo = *parrInfo;
        arrInfo.clear();
        ProcessInfo info;

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(pe32);  // 使用这个结构前，先设置它的大小
        HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 给系统内所有进程拍一个快照
        if ( hProcessSnap == INVALID_HANDLE_VALUE )
        {
            return;
        }

        BOOL bMore = ::Process32First( hProcessSnap, &pe32 );  // 遍历进程快照，轮流显示每个进程的信息
        while (bMore)
        {
            info.m_name = pe32.szExeFile;
            info.m_pid  = pe32.th32ProcessID;
            arrInfo.push_back(info);
            bMore = ::Process32Next(hProcessSnap, &pe32);  // 得到下一个进程的快照
        }

        ::CloseHandle(hProcessSnap);       // 关闭快照对象
    }

    //-----------------------------------------------
    bool Process::RunBlock( const tchar* pszExepath, const tchar* pszCmdLine /*= NULL*/ )
    {
        if (!FileUtils::FileIsExist(pszExepath))
            return false;

        String param = pszCmdLine;
        String exe = pszExepath;
        if (FileUtils::PathIsRelative(pszExepath))
            exe = FileUtils::DirGetCurrent()+pszExepath;

        exe.replace( 0, exe.size()-1, _T("/"), _T("\\") );

        SHELLEXECUTEINFO exeInfo;
        exeInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
        exeInfo.fMask   = (SEE_MASK_DOENVSUBST|SEE_MASK_FLAG_DDEWAIT|0x04000000|SEE_MASK_NOCLOSEPROCESS|SEE_MASK_NO_CONSOLE);
        exeInfo.hwnd    = NULL;
        exeInfo.lpVerb  = _T("open");
        exeInfo.lpFile  = exe.c_str();
        exeInfo.lpParameters    = param.c_str();
        exeInfo.lpDirectory     = 0;
        exeInfo.nShow           = SW_SHOWNORMAL;
        exeInfo.hInstApp        = NULL;
        exeInfo.lpIDList        = NULL;
        exeInfo.lpClass         = NULL;
        exeInfo.hkeyClass       = NULL;
        exeInfo.dwHotKey        = NULL;
        exeInfo.hMonitor        = NULL;
        exeInfo.hIcon           = NULL;
        exeInfo.hProcess        = NULL;

        BOOL r = ::ShellExecuteEx(&exeInfo);
        if (r)
            WaitForSingleObject(exeInfo.hProcess, INFINITE);
        return 0 != r;
    }

    //-----------------------------------------------
    bool Process::Run( const tchar* pszExePath, const tchar* pszCmdLine/* = NULL*/, const tchar* pszRunDir/* = NULL*/ )
    {
//         PROCESS_INFORMATION pi;
//         return 0 != ::CreateProcess( pszExePath, pszCmdLine, 
//             NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, NULL, &pi);

        String exe = pszExePath;
        if (FileUtils::PathIsRelative(pszExePath))
            exe = FileUtils::DirGetCurrent()+pszExePath;

        exe.replace( 0, exe.size()-1, _T("/"), _T("\\") );

        String runDir = pszRunDir;
        if (FileUtils::PathIsRelative(pszRunDir))
            runDir = FileUtils::DirGetCurrent()+pszRunDir;
        runDir.replace( 0, runDir.size()-1, _T("/"), _T("\\") );

        return (HINSTANCE)32 < ::ShellExecute( NULL, _T("open"), exe.c_str(), pszCmdLine, runDir.c_str(), SW_SHOW );
    }

    //-----------------------------------------------
    bool Process::Kill( integer pid )
    {
        // 获得权限
//         TOKEN_PRIVILEGES tp;
//         ZeroMemory(&tp, sizeof(TOKEN_PRIVILEGES));
//         tp.PrivilegeCount = 1;
//         tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
//         ::LookupPrivilegeValue( NULL, SE_TAKE_OWNERSHIP_NAME, &tp.Privileges[0].Luid );
//         HANDLE hToken = NULL;
//         ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );
//         ::AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL);

        HANDLE hProcess = ::OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid );
        if ( !hProcess )
        {
            return false;
        }

        BOOL r = ::TerminateProcess( hProcess, 0 );
        ::CloseHandle( hProcess );

//        ::CloseHandle( hToken );

        return 0 != r;               
    }

    //-----------------------------------------------
    bool Process::Shutdown()
    {
        // 获得权限
        TOKEN_PRIVILEGES tp;
        ZeroMemory(&tp, sizeof(TOKEN_PRIVILEGES));
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        ::LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid );

        HANDLE hToken = NULL;
        ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );

        ::AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL);

        // 关机
        return TRUE == ExitWindowsEx(EWX_FORCE|EWX_POWEROFF, 0);
    }

    //-----------------------------------------------
    bool Process::Reboot()
    {
        // 获得权限
        TOKEN_PRIVILEGES tp;
        ZeroMemory(&tp, sizeof(TOKEN_PRIVILEGES));
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        ::LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid );

        HANDLE hToken = NULL;
        ::OpenProcessToken( ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );

        ::AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL);

        // 关机
        return TRUE == ExitWindowsEx(EWX_REBOOT, 0);
    }

} // end namespace hyc

#endif // end of define __BP_PROCESS_HPP_20101207152643__
