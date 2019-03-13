//
//  utils_string.hpp
//
//  Created by brainpoint on 6/16/15.
//  Copyright (c) 2015 citong. All rights reserved.
//

#ifndef implement_ctUtilsString_hpp
#define implement_ctUtilsString_hpp

namespace hyc {
    
    
    /**
     * 判断字符串是否为空.
     */
    inline bool utils_string_isEmpty( NSString* str )  { return !str || str.length == 0; }
    
    /**
     * 获得指定时间与现在的距离.
     * @param time ms
     */
    inline NSString* utils_string_time( u32 time );
    
    /**
     * 获取指定数值的字符串格式化形式.
     * @return 10W, 10K, 及准确数值
     */
    inline NSString* utils_string_number( u32 num );
}


namespace hyc {
    
    inline NSString* utils_string_time( hyc::u32 time )
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
    
    
    inline NSString* utils_string_number( hyc::u32 num )
    {
        if (num > 10000) {
            NSString* str = [NSString stringWithFormat:@"%u", num/10000];
            if (num%10000>=1000) {
                str = [str stringByAppendingString:[NSString stringWithFormat:@".%u", num%10000/1000]];
            }
            str = [str stringByAppendingString:NSLocalizedString(@"W", nil)];
            return str;
        }
        else if (num > 1000) {
            NSString* str = [NSString stringWithFormat:@"%u", num/1000];
            if (num%1000>=100) {
                str = [str stringByAppendingString:[NSString stringWithFormat:@".%u", num%1000/100]];
            }
            str = [str stringByAppendingString:NSLocalizedString(@"K", nil)];
            return str;
        }
        
        return [NSString stringWithFormat:@"%u", num];
    }
}

#endif
