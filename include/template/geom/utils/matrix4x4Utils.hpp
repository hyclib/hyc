//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyui Graphic Engine
// author:       lipengxiang
// description:  
//

#ifndef __BP_MATRIX4X4UTILS_HPP_20130925232313__
#define __BP_MATRIX4X4UTILS_HPP_20130925232313__

#include "../matrix3x3.hpp"
#include "../matrix4x4.hpp"
#include "matrix3x3Utils.hpp"
#include "quaternionUtils.hpp"
#include "neon_matrix.hpp"

namespace hyc
{
namespace geom
{
	
    //--------------------------------------------------
    //*	utility of Matrix4x4
    //--------------------------------------------------
    class Matrix4x4Utils
    {
    public:

        //* Matrix4x4 -> Matrix3x3
        template<class T>
        static void 
            ToMatrix3x3( const Matrix4x4<T>& m1, OUT Matrix3x3<T> out );

        //* Matrix -> angle and axis
        //* @param angle use in radian.
        template<class T, class T2>
        static void 
            ToAngleAxis( const Matrix4x4<T>& q, OUT T& angle, OUT Vector3<T>& axis );

        //* 4x4 matrix; get a matrix which is result of m1 x m2
        //* !WARNING: pOut must not be same as m1 or m2
        template<class T>
        static void 
            Transform( const Matrix4x4<T>& m1, const Matrix4x4<T>& m2, OUT Matrix4x4<T>& out );

        //* Vector; transform a vector use Matrix
        template<class T>
        static void 
            TransformVector3(  const Matrix4x4<T>& mat, const Vector3<T>& p, OUT Vector3<T>& outP);
        template<class T>
        static void 
            TransformVector2(  const Matrix4x4<T>& mat, const Vector2<T>& p, OUT Vector2<T>& outP);

        //* 4x4 matrix; get a matrix for translation
        template<class T, class T2>
        static void 
            MakeTranslation( T2 x, T2 y, T2 z, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for translation in any direction which go through origin
        template<class T, class T2>
        static void 
            MakeTranslation( const Vector3<T>& direction, T2 distance, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for rotation around axis-x
        //* @param angle use in radian.
        template<class T, class T2>
        static void 
            MakeRotationX( T2 xRot, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for rotation around axis-y
        //* @param angle use in radian.
        template<class T, class T2>
        static void 
            MakeRotationY( T2 yRot, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for rotation around axis-z
        //* @param angle use in radian.
        template<class T, class T2>
        static void 
            MakeRotationZ( T2 zRot, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for rotation around any axis which go through origin
        //* @param angle use in radian.
        template<class T, class T2>
        static void 
            MakeRotation( const Vector3<T>& axis, T2 rot, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for scaling
        template<class T, class T2>
        static void 
            MakeScaling( T2 xScale, T2 yScale, T2 zScale, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for scaling in any direction which go through origin-point
        template<class T, class T2>
        static void 
            MakeScaling( const Vector3<T>& direction, T2 scale, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix for reflect about any axis which go through origin
        //* i.e. MakeScaling(axis, -1, pOut); axis is the normal vector of reflect plane
        template<class T>
        static void 
            MakeReflect( const Vector3<T>& axis, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix from look at
        //! \param eye  the position of camera
        //! \param at   the target of camera look at
        template<class T>
        static void 
            MakeLookAt( const Vector3<T>& eye, const Vector3<T>& at, const Vector3<T>& up, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a matrix from look orientation
        //! \param eye  the position of camera
        //! \param orientation   the orientation of camera look at
        template<class T>
        static void 
            MakeLookOrientation( const Vector3<T>& eye, const Vector3<T>& orientation, const Vector3<T>& up, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a Orthographic matrix
        template<class T, class T2>
        static void 
            MakeOrtho( T2 left, T2 right, T2 top, T2 bottom, T2 nearPos, T2 farPos, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a perspective matrix
        //* @param fovy use in radian.
        template<class T, class T2>
        static void 
            MakePerspective( T2 fovy, T2 aspect, T2 nearPos, T2 farPos, OUT Matrix4x4<T>& out );

        //* 4x4 matrix; get a frustum matrix
        template<class T, class T2>
        static void 
            MakeFrustum( T2 left, T2 right, T2 top, T2 bottom, T2 nearPos, T2 farPos, OUT Matrix4x4<T>& out );

        /** Building a Matrix4 from orientation / scale / position.
        @remarks
        Transform is performed in the order scale, rotate, translation, i.e. translation is independent
        of orientation axes, scale does not affect size of translation, rotation and scaling are always
        centered on the origin.
        */
        template<class T>
        static void 
            Recompose( const Vector3<T>& position, const Vector3<T>& scale, const Quaternion<T>& orientation, OUT Matrix4x4<T>& out );

        //* Decompose a Matrix4 to orientation / scale / position.
        //* must is affine matrix. no projective coefficients.
        template<class T>
        static void 
            Decompose( const Matrix4x4<T>& mat, OUT Vector3<T>& position, OUT Vector3<T>& scale, OUT Quaternion<T>& orientation );
    };

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4Utils::ToMatrix3x3( const Matrix4x4<T>& m1, OUT Matrix3x3<T> out )
    {
        memcpy(out.m[0], m1.m[0], sizeof(T)*3);
        memcpy(out.m[1], m1.m[1], sizeof(T)*3);
        memcpy(out.m[2], m1.m[2], sizeof(T)*3);
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4Utils::Transform( const Matrix4x4<T>& m1, const Matrix4x4<T>& m2, OUT Matrix4x4<T>& out )
    {
        assert(&m1 != &out && &m2 != &out);

#if defined(__ARM_NEON__) && !defined(__arm64__)
        NEON_Matrix4Mul(m1._m, m2._m, out._m);
#else
        out.m[0][0] = m1.m[0][0]*m2.m[0][0] + m1.m[0][1]*m2.m[1][0] + m1.m[0][2]*m2.m[2][0] + m1.m[0][3]*m2.m[3][0];
        out.m[0][1] = m1.m[0][0]*m2.m[0][1] + m1.m[0][1]*m2.m[1][1] + m1.m[0][2]*m2.m[2][1] + m1.m[0][3]*m2.m[3][1];
        out.m[0][2] = m1.m[0][0]*m2.m[0][2] + m1.m[0][1]*m2.m[1][2] + m1.m[0][2]*m2.m[2][2] + m1.m[0][3]*m2.m[3][2];
        out.m[0][3] = m1.m[0][0]*m2.m[0][3] + m1.m[0][1]*m2.m[1][3] + m1.m[0][2]*m2.m[2][3] + m1.m[0][3]*m2.m[3][3];

        out.m[1][0] = m1.m[1][0]*m2.m[0][0] + m1.m[1][1]*m2.m[1][0] + m1.m[1][2]*m2.m[2][0] + m1.m[1][3]*m2.m[3][0];
        out.m[1][1] = m1.m[1][0]*m2.m[0][1] + m1.m[1][1]*m2.m[1][1] + m1.m[1][2]*m2.m[2][1] + m1.m[1][3]*m2.m[3][1];
        out.m[1][2] = m1.m[1][0]*m2.m[0][2] + m1.m[1][1]*m2.m[1][2] + m1.m[1][2]*m2.m[2][2] + m1.m[1][3]*m2.m[3][2];
        out.m[1][3] = m1.m[1][0]*m2.m[0][3] + m1.m[1][1]*m2.m[1][3] + m1.m[1][2]*m2.m[2][3] + m1.m[1][3]*m2.m[3][3];

        out.m[2][0] = m1.m[2][0]*m2.m[0][0] + m1.m[2][1]*m2.m[1][0] + m1.m[2][2]*m2.m[2][0] + m1.m[2][3]*m2.m[3][0];
        out.m[2][1] = m1.m[2][0]*m2.m[0][1] + m1.m[2][1]*m2.m[1][1] + m1.m[2][2]*m2.m[2][1] + m1.m[2][3]*m2.m[3][1];
        out.m[2][2] = m1.m[2][0]*m2.m[0][2] + m1.m[2][1]*m2.m[1][2] + m1.m[2][2]*m2.m[2][2] + m1.m[2][3]*m2.m[3][2];
        out.m[2][3] = m1.m[2][0]*m2.m[0][3] + m1.m[2][1]*m2.m[1][3] + m1.m[2][2]*m2.m[2][3] + m1.m[2][3]*m2.m[3][3];

        out.m[3][0] = m1.m[3][0]*m2.m[0][0] + m1.m[3][1]*m2.m[1][0] + m1.m[3][2]*m2.m[2][0] + m1.m[3][3]*m2.m[3][0];
        out.m[3][1] = m1.m[3][0]*m2.m[0][1] + m1.m[3][1]*m2.m[1][1] + m1.m[3][2]*m2.m[2][1] + m1.m[3][3]*m2.m[3][1];
        out.m[3][2] = m1.m[3][0]*m2.m[0][2] + m1.m[3][1]*m2.m[1][2] + m1.m[3][2]*m2.m[2][2] + m1.m[3][3]*m2.m[3][2];
        out.m[3][3] = m1.m[3][0]*m2.m[0][3] + m1.m[3][1]*m2.m[1][3] + m1.m[3][2]*m2.m[2][3] + m1.m[3][3]*m2.m[3][3];

#endif
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeTranslation( T2 x, T2 y, T2 z, OUT Matrix4x4<T>& out )
    {
        out.m[0][0] = 1;    out.m[0][1] = 0;    out.m[0][2] = 0;    out.m[0][3] = 0;
        out.m[1][0] = 0;    out.m[1][1] = 1;    out.m[1][2] = 0;    out.m[1][3] = 0;
        out.m[2][0] = 0;    out.m[2][1] = 0;    out.m[2][2] = 1;    out.m[2][3] = 0;
        out.m[3][0] = x;    out.m[3][1] = y;    out.m[3][2] = z;    out.m[3][3] = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeTranslation( const Vector3<T>& direction, T2 distance, OUT Matrix4x4<T>& out )
    {
        Vector3<T> d(direction);
        d.normalize();

        out.m[0][0] = 1;    out.m[0][1] = 0;    out.m[0][2] = 0;    out.m[0][3] = 0;
        out.m[1][0] = 0;    out.m[1][1] = 1;    out.m[1][2] = 0;    out.m[1][3] = 0;
        out.m[2][0] = 0;    out.m[2][1] = 0;    out.m[2][2] = 1;    out.m[2][3] = 0;
        out.m[3][0] = d.x*distance;
        out.m[3][1] = d.y*distance;
        out.m[3][2] = d.z*distance;
        out.m[3][3] = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeRotationX( T2 xRot, OUT Matrix4x4<T>& out )
    {
        T cs = (T)Math::Cos(xRot);
        T sn = (T)Math::Sin(xRot);

        out.m[0][0] = 1;   out.m[0][1] = 0;   out.m[0][2] = 0;   out.m[0][3] = 0;
        out.m[1][0] = 0;   out.m[1][1] = cs;  out.m[1][2] = sn;  out.m[1][3] = 0;
        out.m[2][0] = 0;   out.m[2][1] = -sn; out.m[2][2] = cs;  out.m[2][3] = 0;
        out.m[3][0] = 0;   out.m[3][1] = 0;   out.m[3][2] = 0;   out.m[3][3] = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeRotationY( T2 yRot, OUT Matrix4x4<T>& out )
    {
        T cs = (T)Math::Cos(yRot);
        T sn = (T)Math::Sin(yRot);

        out.m[0][0] = cs;  out.m[0][1] = 0;  out.m[0][2] = -sn;   out.m[0][3] = 0;
        out.m[1][0] = 0;   out.m[1][1] = 1;  out.m[1][2] = 0;     out.m[1][3] = 0;
        out.m[2][0] = sn;  out.m[2][1] = 0;  out.m[2][2] = cs;    out.m[2][3] = 0;
        out.m[3][0] = 0;   out.m[3][1] = 0;  out.m[3][2] = 0;     out.m[3][3] = 1;

    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeRotationZ( T2 zRot, OUT Matrix4x4<T>& out )
    {
        T cs = (T)Math::Cos(zRot);
        T sn = (T)Math::Sin(zRot);

        out.m[0][0] = cs;  out.m[0][1] = sn;   out.m[0][2] = 0;   out.m[0][3] = 0;
        out.m[1][0] = -sn; out.m[1][1] = cs;   out.m[1][2] = 0;   out.m[1][3] = 0;
        out.m[2][0] = 0;   out.m[2][1] = 0;    out.m[2][2] = 1;   out.m[2][3] = 0;
        out.m[3][0] = 0;   out.m[3][1] = 0;    out.m[3][2] = 0;   out.m[3][3] = 1;

    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeRotation( const Vector3<T>& axis, T2 rot, OUT Matrix4x4<T>& out )
    {
        /*
        rotate(n, k) = [nx*nx*(1-cosk)+cosk,    nx*ny*(1-cosk)+nz*sink,     nx*nz*(1-cosk)-ny*sink,
        nx*ny*(1-cosk)-nz*sink, ny*ny*(1-cosk)+cosk,        ny*nz*(1-cosk)+nx*sink,
        nx*nz*(1-cosk)+ny*sink, ny*nz*(1-cosk)-nx*sink,     nz*nz*(1-cosk)+cosk]
        */

        T cs = (T)Math::Cos( rot );
        T cs1 = 1 - cs;
        T xy = axis.x * axis.y * cs1;
        T yz = axis.y * axis.z * cs1;
        T xz = axis.x * axis.z * cs1;
        T sn = (T)Math::Sin( rot );
        T xsn = axis.x * sn;
        T ysn = axis.y * sn;
        T zsn = axis.z * sn;

        out._11 = axis.x * axis.x * cs1 + cs;
        out._12 = xy + zsn;
        out._13 = xz - ysn;
        out._14 = 0;

        out._21 = xy - zsn;
        out._22 = axis.y * axis.y * cs1 + cs;
        out._23 = yz + xsn;
        out._24 = 0;

        out._31 = xz + ysn;
        out._32 = yz - xsn;
        out._33 = axis.z * axis.z * cs1 + cs;
        out._34 = 0;

        out._41 = 0;
        out._42 = 0;
        out._43 = 0;
        out._44 = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeScaling( T2 xScale, T2 yScale, T2 zScale, OUT Matrix4x4<T>& out )
    {
        out.m[0][0] = xScale;   out.m[0][1] = 0;      out.m[0][2] = 0;   out.m[0][3] = 0;
        out.m[1][0] = 0;        out.m[1][1] = yScale; out.m[1][2] = 0;   out.m[1][3] = 0;
        out.m[2][0] = 0;        out.m[2][1] = 0;      out.m[2][2] = zScale;  out.m[2][3] = 0;
        out.m[3][0] = 0;        out.m[3][1] = 0;      out.m[3][2] = 0;   out.m[3][3] = 1;
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeScaling( const Vector3<T>& direction, T2 scale, OUT Matrix4x4<T>& out )
    {
        /*
        scale(n, k) = [1+(k-1)*nx*nx, (k-1)*nx*ny, (k-1)*nx*nz,
        (k-1)*nx*ny, 1+(k-1)*ny*ny, (k-1)*ny*nz,
        (k-1)*nx*nz, (k-1)*nz*ny, 1+(k-1)*nz*nz]
        */

        T k1 = scale - 1;
        T xy = direction.x * direction.y * k1;
        T yz = direction.y * direction.z * k1;
        T xz = direction.x * direction.z * k1;

        out._11 = 1 + k1 * direction.x * direction.x;
        out._12 = xy;
        out._13 = xz;
        out._14 = 0;

        out._21 = xy;
        out._22 = 1 + k1 * direction.y * direction.y;
        out._23 = yz;
        out._24 = 0;

        out._31 = xz;
        out._32 = yz;
        out._33 = 1 + k1 * direction.z * direction.z;
        out._34 = 0;

        out._41 = 0;
        out._42 = 0;
        out._43 = 0;
        out._44 = 1;
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4Utils::MakeReflect( const Vector3<T>& axis, OUT Matrix4x4<T>& out )
    {
        /*
        reflect(n) = scale(n, -1)
        */

        MakeScaling( axis, -1, out );
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeOrtho( T2 left, T2 right, T2 top, T2 bottom, T2 nearPos, T2 farPos, OUT Matrix4x4<T>& out )
    {
        assert(left != right && bottom != top && nearPos != farPos);

        out._m[0] = 2.0f / (right - left);
        out._m[1] = 0.0f;
        out._m[2] = 0.0f;
        out._m[3] = 0.0f;

        out._m[4] = 0.0f;
        out._m[5] = 2.0f / (top - bottom);
        out._m[6] = 0.0f;
        out._m[7] = 0.0f;

        out._m[8] = 0.0f;
        out._m[9] = 0.0f;
        out._m[10] = -2.0f / (farPos - nearPos);
        out._m[11] = 0.0f;

        out._m[12] = - (right + left) / (right - left);
        out._m[13] = - (top + bottom) / (top - bottom);
        out._m[14] = - (farPos + nearPos) / (farPos - nearPos);
        out._m[15] = 1.0f; 
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakeFrustum( T2 left, T2 right, T2 top, T2 bottom, T2 nearPos, T2 farPos, OUT Matrix4x4<T>& out )
    {
        assert(left != right && bottom != top && nearPos != farPos);

        out._m[0]  = (2.0 * nearPos) / (right - left);
        out._m[1]  = 0.0;
        out._m[2]  = 0.0;
        out._m[3] = 0.0;

        out._m[4]  = 0.0;
        out._m[5]  = (2.0 * nearPos) / (top - bottom);
        out._m[6]  = 0.0;
        out._m[7] = 0.0;

        out._m[8]  = (right + left) / (right - left);
        out._m[9]  = (top + bottom) / (top - bottom);
        out._m[10] = -(farPos + nearPos) / (farPos - nearPos);
        out._m[11] = -1.0;

        out._m[12]  = 0.0;
        out._m[13]  = 0.0;
        out._m[14] = -(2.0 * farPos * nearPos) / (farPos - nearPos);
        out._m[15] = 0.0;
    }

    //----------------------------------------
    template<class T>
    inline void Matrix4x4Utils::MakeLookOrientation( const Vector3<T>& eye, const Vector3<T>& orientation, const Vector3<T>& up, OUT Matrix4x4<T>& out )
    {
        Vector3<T> f = orientation;       f.normalize();
        Vector3<T> upp = up;               upp.normalize();
        Vector3<T> s = f.crossProduct(upp);//s.normalize();
        Vector3<T> u = s.crossProduct(f);  //u.normalize();

        out._m[0] = s.x;
        out._m[4] = s.y;
        out._m[8] = s.z;
        out._m[12] = 0.0;

        out._m[1] = u.x;
        out._m[5] = u.y;
        out._m[9] = u.z;
        out._m[13] = 0.0;

        out._m[2] = -f.x;
        out._m[6] = -f.y;
        out._m[10] = -f.z;
        out._m[14] = 0.0;

        out._m[3] = out._m[7] = out._m[11] = 0.0;
        out._m[15] = 1.0;

        Matrix4x4<T> translate;
        MakeTranslation(-eye.x, -eye.y, -eye.z, translate);

        out *= translate;
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4Utils::MakeLookAt( const Vector3<T>& eye, const Vector3<T>& at, const Vector3<T>& up, OUT Matrix4x4<T>& out )
    {
        MakeLookOrientation(eye, at-eye, up, out);
    }

    //-----------------------------------------------
    template<class T, class T2>
    inline void Matrix4x4Utils::MakePerspective(T2 fovy, T2 aspect, T2 nearPos, T2 farPos, OUT Matrix4x4<T>& out)
    {
        f32 r = fovy / 2;
        T deltaZ = farPos - nearPos;
        T s = Math::Sin(r);
        T cotangent = 0;

        if (deltaZ == 0 || s == 0 || aspect == 0) {
            assert(0);
            return;
        }

        //cos(r) / sin(r) = cot(r)
        cotangent = Math::Cos(r) / s;

        out._m[0] = cotangent / aspect;
        out._m[1] = 0;
        out._m[2] = 0;
        out._m[3] = 0;

        out._m[4] = 0;
        out._m[5] = cotangent;
        out._m[6] = 0;
        out._m[7] = 0;

        out._m[8] = 0;
        out._m[9] = 0;
        out._m[10] = -(farPos + nearPos) / deltaZ;
        out._m[11] = -1;

        out._m[12] = 0;
        out._m[13] = 0;
        out._m[14] = -2 * nearPos * farPos / deltaZ;
        out._m[15] = 0;
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4Utils::TransformVector3(  const Matrix4x4<T>& mat, const Vector3<T>& p, OUT Vector3<T>& outP)
    {
        assert(&p != &outP);
        /*
        a = (Vx, Vy, Vz, 1)
        b = (a×M)T
        Out = (bx, by, bz)
        */

        outP.x = p.x * mat._m[0] + p.y * mat._m[4] + p.z * mat._m[8] + mat._m[12];
        outP.y = p.x * mat._m[1] + p.y * mat._m[5] + p.z * mat._m[9] + mat._m[13];
        outP.z = p.x * mat._m[2] + p.y * mat._m[6] + p.z * mat._m[10] + mat._m[14];
    }

    //----------------------------------------
    template<class T>
    inline void Matrix4x4Utils::TransformVector2( const Matrix4x4<T>& mat, const Vector2<T>& p, OUT Vector2<T>& outP )
    {
        assert(&p != &outP);

        /*
        a = (Vx, Vy, Vz, 1)
        b = (a×M)T
        Out = (bx, by, bz)
        */

        outP.x = p.x * mat._m[0] + p.y * mat._m[4] + mat._m[12];
        outP.y = p.x * mat._m[1] + p.y * mat._m[5] + mat._m[13];
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4Utils::Recompose( const Vector3<T>& position, const Vector3<T>& scale, const Quaternion<T>& orientation, OUT Matrix4x4<T>& out )
    {
        // Ordering:
        //    1. Scale
        //    2. Rotate
        //    3. Translate

        Matrix3x3<T> rot3x3;
        QuaternionUtils::ToMatrix(orientation, rot3x3);

        // Set up final matrix with scale, rotation and translation
        out.m[0][0] = scale.x * rot3x3[0][0]; out.m[1][0] = scale.y * rot3x3[1][0]; out.m[2][0] = scale.z * rot3x3[2][0]; out.m[3][0] = position.x;
        out.m[0][1] = scale.x * rot3x3[0][1]; out.m[1][1] = scale.y * rot3x3[1][1]; out.m[2][1] = scale.z * rot3x3[2][1]; out.m[3][1] = position.y;
        out.m[0][2] = scale.x * rot3x3[0][2]; out.m[1][2] = scale.y * rot3x3[1][2]; out.m[2][2] = scale.z * rot3x3[2][2]; out.m[3][2] = position.z;

        // No projection term
        out.m[0][3] = 0; out.m[1][3] = 0; out.m[2][3] = 0; out.m[3][3] = 1;
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4Utils::Decompose( const Matrix4x4<T>& mat, OUT Vector3<T>& position, OUT Vector3<T>& scale, OUT Quaternion<T>& orientation )
    {
        //assert(isAffine());

        Matrix3x3<T> kQ;
        //Vector3<T> kU;
        Vector3<T>& kD = scale;

        // QR分解.
        // Factor M = QR = QDU where Q is orthogonal, D is diagonal,
        // and U is upper triangular with ones on its diagonal.  Algorithm uses
        // Gram-Schmidt orthogonalization (the QR algorithm).
        //
        // If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
        //
        //   q0 = m0/|m0|
        //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
        //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
        //
        // where |V| indicates length of vector V and A*B indicates dot
        // product of vectors A and B.  The matrix R has entries
        //
        //   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
        //   r10 = 0      r11 = q1*m1  r12 = q1*m2
        //   r20 = 0      r21 = 0      r22 = q2*m2
        //
        // so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
        // u02 = r02/r00, and u12 = r12/r11.

        // Q = rotation
        // D = scaling
        // U = shear

        // D stores the three diagonal entries r00, r11, r22
        // U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

        // build orthogonal matrix Q
        T fInvLength = Math::InvSqrt(mat.m[0][0]*mat.m[0][0]
        + mat.m[0][1]*mat.m[0][1] +
            mat.m[0][2]*mat.m[0][2]);
        kQ.m[0][0] = mat.m[0][0]*fInvLength;
        kQ.m[0][1] = mat.m[0][1]*fInvLength;
        kQ.m[0][2] = mat.m[0][2]*fInvLength;

        T fDot = kQ.m[0][0]*mat.m[1][0] + kQ.m[0][1]*mat.m[1][1] +
            kQ.m[0][2]*mat.m[1][2];
        kQ.m[1][0] = mat.m[1][0]-fDot*kQ.m[0][0];
        kQ.m[1][1] = mat.m[1][1]-fDot*kQ.m[0][1];
        kQ.m[1][2] = mat.m[1][2]-fDot*kQ.m[0][2];
        fInvLength = Math::InvSqrt(kQ.m[1][0]*kQ.m[1][0] + kQ.m[1][1]*kQ.m[1][1] +
            kQ.m[1][2]*kQ.m[1][2]);
        kQ.m[1][0] *= fInvLength;
        kQ.m[1][1] *= fInvLength;
        kQ.m[1][2] *= fInvLength;

        fDot = kQ.m[0][0]*mat.m[2][0] + kQ.m[0][1]*mat.m[2][1] +
            kQ.m[0][2]*mat.m[2][2];
        kQ.m[2][0] = mat.m[2][0]-fDot*kQ.m[0][0];
        kQ.m[2][1] = mat.m[2][1]-fDot*kQ.m[0][1];
        kQ.m[2][2] = mat.m[2][2]-fDot*kQ.m[0][2];
        fDot = kQ.m[1][0]*mat.m[2][0] + kQ.m[1][1]*mat.m[2][1] +
            kQ.m[1][2]*mat.m[2][2];
        kQ.m[2][0] -= fDot*kQ.m[1][0];
        kQ.m[2][1] -= fDot*kQ.m[1][1];
        kQ.m[2][2] -= fDot*kQ.m[1][2];
        fInvLength = Math::InvSqrt(kQ.m[2][0]*kQ.m[2][0] + kQ.m[2][1]*kQ.m[2][1] +
            kQ.m[2][2]*kQ.m[2][2]);
        kQ.m[2][0] *= fInvLength;
        kQ.m[2][1] *= fInvLength;
        kQ.m[2][2] *= fInvLength;

        // guarantee that orthogonal matrix has determinant 1 (no reflections)
        T fDet = kQ.m[0][0]*kQ.m[1][1]*kQ.m[2][2] + kQ.m[1][0]*kQ.m[2][1]*kQ.m[0][2] +
            kQ.m[2][0]*kQ.m[0][1]*kQ.m[1][2] - kQ.m[2][0]*kQ.m[1][1]*kQ.m[0][2] -
            kQ.m[1][0]*kQ.m[0][1]*kQ.m[2][2] - kQ.m[0][0]*kQ.m[2][1]*kQ.m[1][2];

        if ( fDet < 0.0 )
        {
            for (size_t iRow = 0; iRow < 3; iRow++)
                for (size_t iCol = 0; iCol < 3; iCol++)
                    kQ.m[iCol][iRow] = -kQ.m[iCol][iRow];
        }

        // build "right" matrix R
        Matrix3x3<T> kR;
        kR.m[0][0] = kQ.m[0][0]*mat.m[0][0] + kQ.m[0][1]*mat.m[0][1] +
            kQ.m[0][2]*mat.m[0][2];
        kR.m[1][0] = kQ.m[0][0]*mat.m[1][0] + kQ.m[0][1]*mat.m[1][1] +
            kQ.m[0][2]*mat.m[1][2];
        kR.m[1][1] = kQ.m[1][0]*mat.m[1][0] + kQ.m[1][1]*mat.m[1][1] +
            kQ.m[1][2]*mat.m[1][2];
        kR.m[2][0] = kQ.m[0][0]*mat.m[2][0] + kQ.m[0][1]*mat.m[2][1] +
            kQ.m[0][2]*mat.m[2][2];
        kR.m[2][1] = kQ.m[1][0]*mat.m[2][0] + kQ.m[1][1]*mat.m[2][1] +
            kQ.m[1][2]*mat.m[2][2];
        kR.m[2][2] = kQ.m[2][0]*mat.m[2][0] + kQ.m[2][1]*mat.m[2][1] +
            kQ.m[2][2]*mat.m[2][2];

        // the scaling component
        kD.x = kR.m[0][0];
        kD.y = kR.m[1][1];
        kD.z = kR.m[2][2];

        //         // the shear component
        //         T fInvD0 = 1.0f/kD[0];
        //         kU[0] = kR.m[0][1]*fInvD0;
        //         kU[1] = kR.m[0][2]*fInvD0;
        //         kU[2] = kR.m[1][2]/kD[1];

        Matrix3x3Utils::ToQuaternion(kQ, orientation);
        position.x = mat.m[3][0];
        position.y = mat.m[3][1];
        position.z = mat.m[3][2];
    }


} // end namespace geom
} // end namespace hyc



#endif // end of define __BP_MATRIX4X4UTILS_HPP_20130925232313__