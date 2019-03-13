//
//  identifierUtils.mm
//  hyc
//
//  Created by brainpoint on 14-3-22.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#include "../preheader.h"

#ifdef HYC_COMPILE_MODULE_IDENTIFIER

#include "../../include/utils/identifierUtils.h"
#include "../../include/_platform/iOS.h"

namespace hyc
{
    String IdentifierUtils::generateUUID()
    {
        CFUUIDRef puuid = CFUUIDCreate( nil );
        CFStringRef uuidString = CFUUIDCreateString( nil, puuid );
        NSString * result = (__bridge NSString *)CFStringCreateCopy( NULL, uuidString);
        CFRelease(puuid);
        CFRelease(uuidString);
        
        String s = PlatformIOS::ConvertToString(result);
        //[result release];
        return s;
    }
}

#endif