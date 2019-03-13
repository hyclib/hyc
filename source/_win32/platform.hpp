//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#if CN_PLATFORM == CN_PLATFORM_WINDOWS


namespace hyc
{

    //-----------------------------------------------
    uinteger Platform::GetProcessorNum()
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        return sysInfo.dwNumberOfProcessors;
    }


    //-----------------------------------------------
    hyc::s32 Platform::GetLastError()
    {
        return ::GetLastError();
    }


    //-----------------------------------------------
    String Platform::GetLastErrorMsg(s32 errCode)
    {
        LPTSTR lpBuffer;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_IGNORE_INSERTS
            | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errCode, // 错误代码.
            LANG_NEUTRAL,
            (LPTSTR)& lpBuffer,
            0,
            NULL);
        String str = lpBuffer;
        LocalFree(lpBuffer);
        return str;
    }


    //-----------------------------------------------
    void Platform::GetMemoryStatus(OUT MemoryStatus& s)
    {
        MEMORYSTATUS status;
        ZeroMemory(&status, sizeof(status));
        status.dwLength = sizeof(MEMORYSTATUS);
        ::GlobalMemoryStatus(&status);

        s.totalVirtual = status.dwTotalVirtual;
        s.availVirtual = status.dwAvailVirtual;
        s.totalPhy = status.dwTotalPhys;
        s.availPhy = status.dwAvailPhys;
    }


    //-----------------------------------------------
    void Platform::ShowTray(bool show)
    {
        ::ShowWindow(::FindWindow(_T("Shell_TrayWnd"), NULL), show ? SW_SHOW : SW_HIDE);
        ::EnableWindow(::FindWindow(_T("Shell_TrayWnd"), NULL), show ? TRUE : FALSE);
    }

    //-----------------------------------------------
    bool Platform::IsShowTray()
    {
        return TRUE == ::IsWindowVisible(::FindWindow(_T("Shell_TrayWnd"), NULL));
    }

    //-----------------------------------------------
    bool Platform::IsShowDesktop()
    {
        return TRUE == ::IsWindowVisible(::FindWindow(_T("Progman"), NULL));
    }

    //-----------------------------------------------
    void Platform::ShowDesktop(bool show)
    {
        ::ShowWindow(::FindWindow(_T("Progman"), NULL), show ? SW_SHOW : SW_HIDE);
        ::EnableWindow(::FindWindow(_T("Progman"), NULL), show ? TRUE : FALSE);
    }

//     int check_nic()
//     {
//         int fd;
//         struct ifreq ifr;
//         D_INT32 ret = 0;
// 
//         if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) <= 0)
//         {
// 
//             return 0;
//         }
//         memset(&ifr, 0, sizeof(ifr));
//         strncpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name) - 1);
//         if (ioctl(fd, SIOCGIFFLAGS, (char *)&ifr) < 0)
//         {
//             close(fd);
//             return 0;
//         }
// 
//         if ((ifr.ifr_flags & IFF_RUNNING) == 0)
//         {
//             ret = 0;
//         }
//         else
//         {
//             ret = 1;
//         }
// 
//         close(fd);
//         return ret;
//     }

} // end namespace hyc

#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS
