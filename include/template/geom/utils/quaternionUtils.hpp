//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyui Graphic Engine
// author:       lipengxiang
// description:  
//

#ifndef __BP_QUATERNIONUTILS_HPP_20130925235725__
#define __BP_QUATERNIONUTILS_HPP_20130925235725__


namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	utility of quaternion
    //--------------------------------------------------
    class QuaternionUtils
    {
    public:

        //* Angle and axis -> quaternion
        //* @param angle use in radian.
        template<class T>
        static void 
            MakeAngleAxis( const Vector3<T>& axis, const f32& angle, OUT Quaternion<T>& q );

        //* quaternion -> Euler angles
        template<class T>
        static void 
            ToMatrix( const Quaternion<T>& q, OUT Matrix3x3<T>& mat );
    };

    //-----------------------------------------------
    template<class T>
    inline void QuaternionUtils::ToMatrix( const Quaternion<T>& q, OUT Matrix3x3<T>& kRot )
    {
        T fTx  = q.x+q.x;
        T fTy  = q.y+q.y;
        T fTz  = q.z+q.z;
        T fTwx = fTx*q.w;
        T fTwy = fTy*q.w;
        T fTwz = fTz*q.w;
        T fTxx = fTx*q.x;
        T fTxy = fTy*q.x;
        T fTxz = fTz*q.x;
        T fTyy = fTy*q.y;
        T fTyz = fTz*q.y;
        T fTzz = fTz*q.z;

        kRot[0][0] = 1-(fTyy+fTzz);
        kRot[1][0] = fTxy-fTwz;
        kRot[2][0] = fTxz+fTwy;
        kRot[0][1] = fTxy+fTwz;
        kRot[1][1] = 1-(fTxx+fTzz);
        kRot[2][1] = fTyz-fTwx;
        kRot[0][2] = fTxz-fTwy;
        kRot[1][2] = fTyz+fTwx;
        kRot[2][2] = 1-(fTxx+fTyy);
    }

    //-----------------------------------------------
    template<class T>
    inline void QuaternionUtils::MakeAngleAxis( const Vector3<T>& axis, const f32& angle, OUT Quaternion<T>& q )
    {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        f32 fHalfAngle = ( 0.5*angle );
        T fSin = Math::Sin(fHalfAngle);
        q.w = Math::Cos(fHalfAngle);
        q.x = fSin*axis.x;
        q.y = fSin*axis.y;
        q.z = fSin*axis.z;
    }


} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_QUATERNIONUTILS_HPP_20130925235725__