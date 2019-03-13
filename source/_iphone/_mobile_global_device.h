//
//  _mobile_global_device.h
//  hyc
//
//  Created by brainpoint on 14-3-23.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#ifndef hyc__mobile_global_device_h
#define hyc__mobile_global_device_h
#ifdef HYC_COMPILE_MODULE_MOBILE

#import <UIKit/UIKit.h>
#import "../../include/hyc.h"
#import "../../include/_mobile/device.h"

using namespace hymobile;

//---------------------------------------
@interface __OrientationNotification : NSObject
{
@public
    ArrayFind<DeviceOrientationDelegate*>    g_arrDeviceOriListener;
    hyc::SyncObject                      g_arrDeviceOriSync;
    f64                                  g_oriChangeDelay;
}

- (void)add: (DeviceOrientationDelegate*) delegate;
- (void)remove: (DeviceOrientationDelegate*) delegate;
- (void)orientationChanged:(NSNotification *)notification;

@end


#endif
#endif
