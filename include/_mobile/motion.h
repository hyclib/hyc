/*
 *  Accelerometer.h
 *  hyc
 *
 *  Created by lipengxiang on 11-10-21.
 *  Copyright 2011 hy. All rights reserved.
 *
 */

#ifndef HYC_MOBILE_MOTION_H
#define HYC_MOBILE_MOTION_H
#ifdef HYC_COMPILE_MODULE_MOBILE

namespace hymobile
{
    /**
     * Accelerometer Delegate.
     */
    class HYC_API MotionAccelerometerDelegate
    {
    public:
        MotionAccelerometerDelegate();
        virtual ~MotionAccelerometerDelegate();
        
        /**
         * @brief 当发生重力加速变化时回调.
         *        使用右手坐标系，z向外，y向上，x向右.
         * @param timestamp 单位为millisecond.
         */
        virtual void    onUpdate( const f64& x, const f64& y, const f64& z, const f64& timestamp ) = 0;
        
        virtual void    onError( const String& msg, long code ) = 0;
    };
    
    /**
     * Rotation Delegate.
     */
    class HYC_API MotionRotationDelegate
    {
    public:
        MotionRotationDelegate();
        virtual ~MotionRotationDelegate();
        
        /**
         * @brief 当发生重力加速变化时回调.
         *        使用右手坐标系，z向外，y向上，x向右.
         * @param timestamp 单位为millisecond.
         */
        virtual void    onUpdate( const f64& x, const f64& y, const f64& z, const f64& timestamp ) = 0;
        
        virtual void    onError( const String& msg, long code ) = 0;
    };
    
    /**
     * device motion, accelermeter.
     */
    class HYC_API Motion
    {
    public:
        /**
         * @brief Judgment the device if support the feature.
         */
        static bool IsSupportAccelerometer();
        static bool IsSupportRotation();
        
        /**
         * @brief Set delegate Update Interval
         */
        static void SetUpdateIntervalAccelerometer( u32 interval );
        static void SetUpdateIntervalRotation( u32 interval );
    };
    
} // end namespace of hymobile

#endif
#endif // HYC_MOBILE_MOTION_H
