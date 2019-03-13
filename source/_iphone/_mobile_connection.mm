//
//  _mobile_connection.mm
//  hyc
//
//  Created by brainpoint on 14-3-23.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#import "../preheader.h"
using namespace hyc;
#ifdef HYC_COMPILE_MODULE_MOBILE

#import "_mobile_global.h"

#define ASSUME_CONNECTION() if (nil == g_mobile_connection) g_mobile_connection = [[__ConnectionInstance alloc] init];

namespace hymobile {
    
    //-----------------------------------------------
    ConnectionDelegate::ConnectionDelegate()
    {
        ASSUME_CONNECTION();
        [g_mobile_connection add:this];
    }
    
    //-----------------------------------------------
    ConnectionDelegate::~ConnectionDelegate()
    {
        if (nil != g_mobile_connection)
            [g_mobile_connection remove:this];
    }
    
    
    //-----------------------------------------------
    ConnectionType Connection::GetConnectionType()
    {
        // 创建零地址，0.0.0.0的地址表示查询本机的网络连接状态.
        struct sockaddr_storage zeroAddress;
        
        bzero(&zeroAddress, sizeof(zeroAddress));
        zeroAddress.ss_len = sizeof(zeroAddress);
        zeroAddress.ss_family = AF_INET;
        
        // Recover reachability flags.
        SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
        SCNetworkReachabilityFlags flags;
        
        // 获得连接的标志.
        BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
        CFRelease(defaultRouteReachability);
        
        
        ConnectionType retVal = ConnectionType_NONE;
        
        // 如果不能获取连接标志，则不能连接网络，直接返回.
        if (!didRetrieveFlags)
        {
            return retVal;
        }
        
        // 根据获得的连接标志进行判断.
        if ((flags & kSCNetworkReachabilityFlagsReachable) == 0)
        {
            return retVal;
        }
        
        
        if ((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0)
        {
            // if target host is reachable and no connection is required.
            //  then we'll assume (for now) that your on Wi-Fi.
            retVal = ConnectionType_WIFI;
        }
        
        if ((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand ) != 0) ||
             (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0))
        {
            // ... and the connection is on-demand (or on-traffic) if the.
            //     calling application is using the CFSocketStream or higher APIs.
            if ((flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0)
            {
                // ... and no [user] intervention is needed
                retVal = ConnectionType_WIFI;
            }
        }
        
        if ((flags & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN)
        {
            if((flags & kSCNetworkReachabilityFlagsReachable) == kSCNetworkReachabilityFlagsReachable) {
                if ((flags & kSCNetworkReachabilityFlagsTransientConnection) == kSCNetworkReachabilityFlagsTransientConnection) {
                    retVal = ConnectionType_CELL_3G;
                    if((flags & kSCNetworkReachabilityFlagsConnectionRequired) == kSCNetworkReachabilityFlagsConnectionRequired) {
                        retVal = ConnectionType_CELL_2G;
                    }
                }
            }
        }
        return retVal;
    }
}

#endif