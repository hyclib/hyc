/*
 *  Accelerometer.h
 *  hyc
 *
 *  Created by lipengxiang on 11-10-21.
 *  Copyright 2011 hy. All rights reserved.
 *
 */

#ifndef HYC_MOBILE_ACCELEROMETER_H
#define HYC_MOBILE_ACCELEROMETER_H

#include "device.h"
using namespace hyc;

namespace hymobile
{

    //----------------------------------------------------------------------------------------------------
    //!	Accelerometer Delegate
    //----------------------------------------------------------------------------------------------------
    class HYC_API AccelerometerDelegate
    {
    public:
        AccelerometerDelegate();
        virtual ~AccelerometerDelegate();
        
        /**
         * @brief 加速器回调.
         *        使用左手坐标系，z向外.
         * @param timestamp    逝去的总事件
         * @param orientation  设备的朝向
         */
        virtual void    onAccelerate( const f64& x, const f64& y, const f64& z, const f64& timestamp ) = 0;
    };
    
    //----------------------------------------------------------------------------------------------------
    //!	Captures device motion in the x, y, and z direction.
    //----------------------------------------------------------------------------------------------------
    class HYC_API Accelerometer
    {
    public:
        /**
         * @brief Judgment the device if support the accelerometer
         */
        static bool IsSupport();
        
        /**
         * @brief Set Listener Update Interval
         */
        static void SetUpdateInterval( f64 interval );
    };
    
} // end namespace of hymobile

#endif // HYC_MOBILE_ACCELEROMETER_H
