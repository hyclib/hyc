//
//  _mobile_global_connection.h
//  hyc
//
//  Created by brainpoint on 14-3-23.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#ifndef hyc__mobile_global_connection_h
#define hyc__mobile_global_connection_h
#ifdef HYC_COMPILE_MODULE_MOBILE

#import <SystemConfiguration/SCNetworkConfiguration.h>
#import <SystemConfiguration/SCNetworkReachability.h>
#import <Foundation/Foundation.h>

#import "../../include/hyc.h"
#import "../../include/_mobile/connection.h"
#import "../../include/_platform/iOS/Reachability.h"

/**
 * connection instance.
 */
@interface __ConnectionInstance : NSObject{
@private
    Reachability*                               _hostReach;
    ArrayFind<hymobile::ConnectionDelegate*>    _arrDelegates;
    hyc::SyncObject                             _sync;
    hymobile::ConnectionType                    _oldConnectionType;
}

-(id)   init;
-(void) dealloc;

-(void) add: (hymobile::ConnectionDelegate*) delegate;
-(void) remove: (hymobile::ConnectionDelegate*) delegate;

@end


#endif
#endif

