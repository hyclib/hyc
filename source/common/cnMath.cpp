//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#include "../../include/common/cnMath.h"

namespace hyc
{
    const real Math::PI           = real( 4.0 * Math::Atan(1.0f) );
    const real Math::PI_2         = real( 2.0 * Math::PI );
    const real Math::PI_HALF      = real( 0.5 * Math::PI );

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
     real Math::TOLERANCE    = 1e-6f;
    const real Math::TOLERANCE_DEFAULT    = 1e-6f;
#else
     real Math::TOLERANCE    = 1e-14;
    const real Math::TOLERANCE_DEFAULT    = 1e-14;
#endif // CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32


    //-----------------------------------------------
    real Math::Radian2Degree( const real& radian )
    {
        static const real _D2R = 180/PI;
        return radian * _D2R;
    }

    //-----------------------------------------------
    real Math::Degree2Radian( const real& degree )
    {
        static const real _R2D = PI/180;
        return degree * _R2D;
    }

    //-----------------------------------------------
    real Math::MakeIn2Pi( const real& radianValue )
    {
        real v = radianValue;
        v -= Floor( radianValue/PI_2 ) * PI_2;
        if ( v < 0 )    v += PI_2;
        return v;
    }

    //-----------------------------------------------
    real Math::MakeInPi( const real& radianValue )
    {
        real v = radianValue;
        v += PI;
        v -= Floor( v/PI_2 ) * PI_2;
	if ( v < 0 )    v += PI_2;
        v -= PI;
        return v;
    }

    //-----------------------------------------------
    bool Math::Equals( const real& f1, const real& f2, const real& tolerance /*= TOLERANCE */ )
    {
        return (f1 - tolerance <= f2) && (f1 + tolerance >= f2);
    }

    //-----------------------------------------------
    bool Math::LessThan( const real& f1, const real& f2, const real& tolerance /*= TOLERANCE */ )
    {
        if (Equals(f1, f2, tolerance)) { return false; } return f1 < f2;
    }

    //-----------------------------------------------
    bool Math::LessEquals( const real& f1, const real& f2, const real& tolerance /*= TOLERANCE */ )
    {
        if (Equals(f1, f2, tolerance)) { return true; } return f1 <= f2;
    }

    //-----------------------------------------------
    bool Math::MoreThan( const real& f1, const real& f2, const real& tolerance /*= TOLERANCE */ )
    {
        if (Equals(f1, f2, tolerance)) { return false; } return f1 > f2;
    }

    //-----------------------------------------------
    bool Math::MoreEquals( const real& f1, const real& f2, const real& tolerance /*= TOLERANCE */ )
    {
        if (Equals(f1, f2, tolerance)) { return true; } return f1 >= f2;
    }

    //-----------------------------------------------
    f32 Math::Sqrt_q( f32 number )
    {
        integer i;
        f32 x, y;
        //const f32 f = 1.5f;

        x = number * 0.5f;
        y  = number;
        i  = * ( integer * ) &y;
        i  = 0x5f375a86 - ( i >> 1 );        // original value is 0x5f3759df(1.5974630e+009); or use 0x5f375a86 for more precision
        y  = * ( f32 * ) &i;
        y  = y * ( 1.5f - ( x * y * y ) );
        y  = y * ( 1.5f - ( x * y * y ) );
        return number * y;
    }

    //-----------------------------------------------
    hyc::f32 Math::InvSqrt_q( f32 x )
    {
        f32 xhalf = x*0.5f;
        integer i = * ( integer * ) &x;
        i  = 0x5f375a86 - ( i >> 1 );        // original value is 0x5f3759df(1.5974630e+009); or use 0x5f375a86 for more precision
        x  = * ( f32 * ) &i;
        x  = x * ( 1.5f - ( xhalf * x * x ) );
        return x;
    }

} // end namespace hyc
