// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define Euler Angles for system

#ifndef __BP_EULERANGLES_H_20100609111340__
#define __BP_EULERANGLES_H_20100609111340__

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	Euler Angles
    //* all angle use in radian.
    //--------------------------------------------------
    struct EulerAngles
    {
        //--------------------------------------------------
        //*     member variable
        //--------------------------------------------------

        real  yaw;
        real  pitch;
        real  roll;

        //--------------------------------------------------
        //*     method
        //--------------------------------------------------

        //* eliminate the Gimbal Lock and nonuniqueness
        inline void canonize();
    };


    //-----------------------------------------------
    inline void EulerAngles::canonize()
    {
        // 限制 pitch 在 -pi/2 ~ pi/2 间

        yaw = Math::MakeInPi(yaw);

        if (pitch < -Math::PI_HALF)
        {
            pitch = -Math::PI - pitch;
            yaw += Math::PI;
            roll += Math::PI;
        }
        else if (pitch > Math::PI_HALF)
        {
            pitch = Math::PI - pitch;
            yaw += Math::PI;
            roll += Math::PI;
        } // if..else

        // 检查gimbal lock
        if ( Math::Equals(Math::Abs(pitch), Math::PI_HALF) )
        {
            yaw += roll;
            roll = 0;
        }
        else
        {
            roll = Math::MakeInPi(roll);

        } // if..else

        yaw = Math::MakeInPi(yaw);
    }


} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_EULERANGLES_H_20100609111340__