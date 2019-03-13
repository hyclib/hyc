//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyui Graphic Engine
// author:       lipengxiang
// description:  
//

#ifndef __BP_MATRIX3X3UTILS_HPP_20130925232318__
#define __BP_MATRIX3X3UTILS_HPP_20130925232318__

#include "../matrix3x3.hpp"
#include "../quaternion.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	utility of Matrix3x3
    //--------------------------------------------------
    class Matrix3x3Utils
    {
    public:

        //* matrix -> quaternion
        template<class T>
        static void 
            ToQuaternion( const Matrix3x3<T>& mat, OUT Quaternion<T>& q );

        //* 3x3 matirx; get a matrix which is result of m1 x m2 (the speed is quicker than use operator*)
        //* !WARNING: pOut must not be same as m1 or m2
        template<class T>
        static void
            Transform( const Matrix3x3<T>& m1, const Matrix3x3<T>& m2, OUT Matrix3x3<T>& out );

        //* 3x3 matirx; get a matrix for translation
        template<class T, class T2>
        static void 
            MakeTranslation( T2 x, T2 y, OUT Matrix3x3<T>& out );

        //* 3x3 matirx; get a matrix for rotation anticlockwise around origin-point
        template<class T, class T2>
        static void
            MakeRotation( T2 rotInRadian, OUT Matrix3x3<T>& out );

        //* 3x3 matirx; get a matrix for scaling
        template<class T, class T2>
        static void 
            MakeScaling( T2 xScale, T2 yScale, OUT Matrix3x3<T>& out );

        //* 3x3 matirx; get a matrix for scaling in any direction which go through origin
        template<class T, class T2>
        static void 
            MakeScaling( const Vector2<T>& direction, T2 scale, OUT Matrix3x3<T>& out );

        //* 3x3 matirx; get a matrix for reflect about any axis which go through origin
        //* i.e. MakeScaling(axis, -1, pOut)
        template<class T>
        static void 
            MakeReflect( const Vector2<T>& axis, OUT Matrix3x3<T>& out );
    };


    //-----------------------------------------------
    template<class T>
    inline void Matrix3x3Utils::ToQuaternion( const Matrix3x3<T>& kRot, OUT Quaternion<T>& q )
    {
        // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
        // article "Quaternion Calculus and Fast Animation".

        T& w = q.w;
        T& x = q.x;
        T& y = q.y;
        T& z = q.z;

        T fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
        T fRoot;

        if ( fTrace > 0.0 )
        {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = Math::Sqrt(fTrace + 1.0f);  // 2w
            w = 0.5f*fRoot;
            fRoot = 0.5f/fRoot;  // 1/(4w)
            x = (kRot[1][2]-kRot[2][1])*fRoot;
            y = (kRot[2][0]-kRot[0][2])*fRoot;
            z = (kRot[0][1]-kRot[1][0])*fRoot;
        }
        else
        {
            // |w| <= 1/2
            static size_t s_iNext[3] = { 1, 2, 0 };
            size_t i = 0;
            if ( kRot[1][1] > kRot[0][0] )
                i = 1;
            if ( kRot[2][2] > kRot[i][i] )
                i = 2;
            size_t j = s_iNext[i];
            size_t k = s_iNext[j];

            fRoot = Math::Sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
            T* apkQuat[3] = { &x, &y, &z };
            *apkQuat[i] = 0.5f*fRoot;
            fRoot = 0.5f/fRoot;
            w = (kRot[j][k]-kRot[k][j])*fRoot;
            *apkQuat[j] = (kRot[i][j]+kRot[j][i])*fRoot;
            *apkQuat[k] = (kRot[i][k]+kRot[k][i])*fRoot;
        }
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix3x3Utils::Transform( const Matrix3x3<T>& m1, const Matrix3x3<T>& m2, OUT Matrix3x3<T>& out )
    {
        assert(&m1 != &out && &m2 != &out);

        out._11 = m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31;
        out._12 = m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32;
        out._13 = m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33;

        out._21 = m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31;
        out._22 = m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32;
        out._23 = m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33;

        out._31 = m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31;
        out._32 = m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32;
        out._33 = m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix3x3Utils::MakeTranslation( T2 x, T2 y, OUT Matrix3x3<T>& out )
    {
        out._11 = 1;  out._12 = 0;  out._13 = 0;
        out._21 = 0;  out._22 = 1;  out._23 = 0;
        out._31 = x;  out._32 = y;  out._33 = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix3x3Utils::MakeRotation( T2 rotInRadian, OUT Matrix3x3<T>& out )
    {
        T cs = Math::Cos(rotInRadian);
        T sn = Math::Sin(rotInRadian);

        out._11 = cs;     out._12 = sn; out._13 = 0;
        out._21 = -sn;    out._22 = cs; out._23 = 0;
        out._31 = 0;      out._32 = 0;  out._33 = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix3x3Utils::MakeScaling( T2 xScale, T2 yScale, OUT Matrix3x3<T>& out )
    {
        out._11 = xScale; out._12 = 0;      out._13 = 0;
        out._21 = 0;      out._22 = yScale; out._23 = 0;
        out._31 = 0;      out._32 = 0;      out._33 = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix3x3Utils::MakeScaling( const Vector2<T>& direction, T2 scale, OUT Matrix3x3<T>& out )
    {
        /*
        scale(n, k) = [1+(k-1)*nx*nx, (k-1)*nx*ny,
        (k-1)*nx*ny, 1+(k-1)*ny*ny]
        */

        T k1xy = (scale-1) * direction.x * direction.y;

        out._11 = 1 + (scale-1) * direction.x * direction.x;
        out._12 = k1xy;
        out._13 = 0;

        out._21 = k1xy;
        out._22 = 1 + (scale-1) * direction.y * direction.y;
        out._23 = 0;

        out._31 = 0;      out._32 = 0;      out._33 = 1;
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix3x3Utils::MakeReflect( const Vector2<T>& axis, OUT Matrix3x3<T>& out )
    {
        /*
        reflect(n) = scale(n, -1)
        */

        MakeScaling( axis, -1, out );
    }


} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_MATRIX3X3UTILS_HPP_20130925232318__