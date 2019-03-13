//
//  _mobile_accelerometer.mm
//  hyc
//
//  Created by 李 鹏翔 on 12-5-12.
//  Copyright (c) 2012年 华耀信息科技有限公司. All rights reserved.
//

#import "../preheader.h"
#import "../../include/os/thread.h"
using namespace hyc;
#ifdef HYC_COMPILE_MODULE_MOBILE


#import "../../include/_platform/iOS.h"
#import "_mobile_global_motion.h"
#import "_mobile_global.h"

#define ASSUME_MOTION() if (NULL == g_mobile_motion) g_mobile_motion = new MotionInstance();

namespace hymobile {
    
    //-----------------------------------------------
    MotionAccelerometerDelegate::MotionAccelerometerDelegate()
    {
        ASSUME_MOTION();
        assert(Motion::IsSupportAccelerometer());
        g_mobile_motion->addAccDelegate(this);
    }
    
    //-----------------------------------------------
    MotionAccelerometerDelegate::~MotionAccelerometerDelegate()
    {
        if (g_mobile_motion)
            g_mobile_motion->removeAccDelegate(this);
    }
    
    //-----------------------------------------------
    MotionRotationDelegate::MotionRotationDelegate()
    {
        ASSUME_MOTION();
        assert (Motion::IsSupportRotation());
        g_mobile_motion->addRotDelegate(this);
    }
    
    //-----------------------------------------------
    MotionRotationDelegate::~MotionRotationDelegate()
    {
        if (g_mobile_motion)
            g_mobile_motion->removeRotDelegate(this);
    }

    //-----------------------------------------------
    bool Motion::IsSupportAccelerometer()
    {
        ASSUME_MOTION();
        if (!g_mobile_motion->isEnable())   return false;
        return g_mobile_motion->motionMgr.isAccelerometerAvailable == TRUE;
    }
    
    //-----------------------------------------------
    bool Motion::IsSupportRotation()
    {
        ASSUME_MOTION();
        if (!g_mobile_motion->isEnable())   return false;
        return g_mobile_motion->motionMgr.isGyroAvailable == TRUE;
    }
    
    //-----------------------------------------------
    void Motion::SetUpdateIntervalAccelerometer( u32 interval )
    {
        if (IsSupportAccelerometer())
            g_mobile_motion->motionMgr.accelerometerUpdateInterval = interval / 1000.0f;
    }

    //-----------------------------------------------
    void Motion::SetUpdateIntervalRotation( u32 interval )
    {
        if (IsSupportRotation())
            g_mobile_motion->motionMgr.gyroUpdateInterval = interval / 1000.0f;
    }
    
} // end namespace hymobile

#endif

