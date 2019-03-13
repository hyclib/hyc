//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// author:       lipengxiang
// description:  
//

#ifndef __BP_DATETIME_HPP_20130503175216__
#define __BP_DATETIME_HPP_20130503175216__


namespace hyc
{
    //-----------------------------------------------
    hyc::TimeMicrosecond DateTime::GetSysMicrosecondTime()
    {
        FILETIME ft;
#if defined(_WIN32_WCE)
        GetSystemTimeAsFileTimeWithMillisecondResolution(&ft);
#else
        GetSystemTimeAsFileTime(&ft);   // in 100ns.
#endif

        ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
        epoch.LowPart  = 0xD53E8000;
        epoch.HighPart = 0x019DB1DE;

        ULARGE_INTEGER ts;
        ts.LowPart  = ft.dwLowDateTime;
        ts.HighPart = ft.dwHighDateTime;
        ts.QuadPart -= epoch.QuadPart;
        return ts.QuadPart/10;
    }

} // end namespace hyc

#endif // end of define __BP_DATETIME_HPP_20130503175216__