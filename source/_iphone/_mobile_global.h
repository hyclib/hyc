//
//  _mobile_global.h
//  hyc
//
//  Created by brainpoint on 14-3-23.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#ifndef hyc__mobile_global_h
#define hyc__mobile_global_h
#ifdef HYC_COMPILE_MODULE_MOBILE

#import "../../include/hyc.h"
#import "_mobile_global_device.h"
#import "_mobile_global_motion.h"
#import "_mobile_global_connection.h"

namespace hymobile {
    class MotionInstance;
}

extern __OrientationNotification*      g_mobile_ori_notification;
extern __ConnectionInstance*           g_mobile_connection;
extern hymobile::MotionInstance*       g_mobile_motion;

#endif
#endif