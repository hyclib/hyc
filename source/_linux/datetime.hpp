//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// author:       lipengxiang
// description:  
//

#ifndef __BP_DATETIME_HPP_20130503175212__
#define __BP_DATETIME_HPP_20130503175212__

#include <sys/time.h>


namespace hyc
{
    //-----------------------------------------------
    hyc::TimeMicrosecond DateTime::GetSysMicrosecondTime()
    {
        struct timeval tv;
        if (gettimeofday(&tv, NULL))
            return 0;

        TimeMicrosecond r = tv.tv_sec;

        r *= 1000000;
        r += tv.tv_usec;

        return r;
    }

} // end namespace hyc

#endif // end of define __BP_DATETIME_HPP_20130503175212__