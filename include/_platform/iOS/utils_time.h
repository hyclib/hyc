//
//  utils_time.h
//  eleven
//
//  Created by brainpoint on 11/8/14.
//  Copyright (c) 2014 hy. All rights reserved.
//

#ifndef eleven_utils_time_h
#define eleven_utils_time_h

#include "../../os/dateTime.h"

namespace hyc {
    
    
    /**
     *! 获得指定时间与现在的距离.
     */
    NSString* utils_time_string( u32 time );
    
    
} // namespace hyc.





namespace hyc {

    inline NSString* utils_time_string( hyc::u32 time )
    {
        hyc::DateTime dt1;
        dt1.setToSysTime();
        
        if (dt1.getTime() > time)
        {
            u32 s = (u32)dt1.getTime() - time;
            
            u32 year = s / (60*60*24*365);
            s %= (60*60*24*365);
            u32 month = s / (60*60*24*30);
            s %= (60*60*24*30);
            u32 day = s / (60*60*24);
            s %= (60*60*24);
            u32 hour = s / (60*60);
            s %= (60*60);
            u32 min = s / 60;
            s %= 60;
            
            if (year > 0)
            {
                if (year > 1)
                    return [NSString stringWithFormat:@"%u%@", year, NSLocalizedString(@"years", nil)];
                else
                    return [NSString stringWithFormat:@"%u%@", year, NSLocalizedString(@"year", nil)];
            }
            else if (month > 0)
            {
                if (month > 1)
                    return [NSString stringWithFormat:@"%u%@", month, NSLocalizedString(@"months", nil)];
                else
                    return [NSString stringWithFormat:@"%u%@", month, NSLocalizedString(@"month", nil)];
            }
            else if (day > 0)
            {
                if (day > 1)
                    return [NSString stringWithFormat:@"%u%@", day, NSLocalizedString(@"days", nil)];
                else
                    return [NSString stringWithFormat:@"%u%@", day, NSLocalizedString(@"day", nil)];
            }
            else if (hour > 0)
            {
                if (hour > 1)
                    return [NSString stringWithFormat:@"%u%@", hour, NSLocalizedString(@"hours", nil)];
                else
                    return [NSString stringWithFormat:@"%u%@", hour, NSLocalizedString(@"hour", nil)];
            }
            else if (min > 0)
            {
                if (min > 1)
                    return [NSString stringWithFormat:@"%u%@", min, NSLocalizedString(@"mins", nil)];
                else
                    return [NSString stringWithFormat:@"%u%@", min, NSLocalizedString(@"min", nil)];
            }
            else if (s > 0)
            {
                if (s > 1)
                    return [NSString stringWithFormat:@"%u%@", s, NSLocalizedString(@"s", nil)];
                else
                    return [NSString stringWithFormat:@"%u%@", s, NSLocalizedString(@"s", nil)];
            }
        }
        
        return NSLocalizedString(@"now", nil);
    }
    
}

#endif
