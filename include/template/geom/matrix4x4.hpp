// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 4x4 matrix for system
//   3d affine transformation (translation, perspective projection)
//      linear transformation (rotate, scale, orthogonal projection, reflect...)

#ifndef __BP_MATRIX4X4_H_20100317111054__
#define __BP_MATRIX4X4_H_20100317111054__

#include "vector3.hpp"
#include "utils/neon_matrix.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	    4x4 matrix
    //--------------------------------------------------
    template<class T>
    struct Matrix4x4
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        union
        {
            T m[4][4];
            T _m[4*4];
            struct { 
                T  _11, _12, _13, _14;
                T  _21, _22, _23, _24;
                T  _31, _32, _33, _34;
                T  _41, _42, _43, _44;
            }; 
        };

        //--------------------------------------------------
        //*     constructor &　destructor
        //--------------------------------------------------

        Matrix4x4();
        Matrix4x4( const Matrix4x4& M );
        Matrix4x4( 
            const T& _11, const T& _12, const T& _13, const T& _14,
            const T& _21, const T& _22, const T& _23, const T& _24, 
            const T& _31, const T& _32, const T& _33, const T& _34, 
            const T& _41, const T& _42, const T& _43, const T& _44 );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------
 
        Matrix4x4& operator=( const Matrix4x4& mat );
        Matrix4x4 operator-() const;

        Matrix4x4 operator+( const T v ) const;
        Matrix4x4 operator-( const T v ) const;
        Matrix4x4 operator*( const T v ) const;

        Matrix4x4& operator+=( const T v );
        Matrix4x4& operator-=( const T v );
        Matrix4x4& operator*=( const T v );

        Matrix4x4 operator+( const Matrix4x4& rhs ) const;
        Matrix4x4 operator-( const Matrix4x4& rhs ) const;
        Matrix4x4 operator*( const Matrix4x4& rhs ) const;    // matrix transform

        Matrix4x4& operator+=( const Matrix4x4& rhs );
        Matrix4x4& operator-=( const Matrix4x4& rhs );
        Matrix4x4& operator*=( const Matrix4x4& rhs );    // matrix transform

        const T*   operator[]( int i ) const   { return &m[i][0]; }
        T*         operator[]( int i )         { return &m[i][0]; }

        bool operator==( const Matrix4x4& rhs ) const;
        bool operator!=( const Matrix4x4& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Matrix4x4& m, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the matrix whether is zero matrix
        bool isNull() const;

        //* set the matrix to identity matirx
        void identity();

        //* judge the matrix whether is identity matrix
        bool isIdentity() const;

        //* get identity matrix
        Matrix4x4 getIdentity() const;

        //* get zero matrix
        Matrix4x4 getZero() const;

        //* get inverse matrix; this operator is slowly
        //* a inverse matrix can undo the transform for origin matrix
        Matrix4x4 getInverse() const;

        //* mark inverse matrix; this operator is slowly
        //* a inverse matrix can undo the transform for origin matrix
        void inverse();

        //* get transpose matrix; i.e.turnover through the catercorner, do this operator is quickly
        //* !TIPS: when the matrix is orthonormal, the transpose matrix is same as inverse matrix
        Matrix4x4 getTranspose() const;

        //* mark transpose to matrix; (quicker than getTranspose)
        //* !TIPS: when the matrix is orthonormal, the transpose matrix is same as inverse matrix
        void transpose();

        //* get axis x,y,z
        const Vector3<T>& getAxisX() const;
        const Vector3<T>& getAxisY() const;
        const Vector3<T>& getAxisZ() const;
    };

    typedef Matrix4x4<real>  Matrix4x4_f;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>::Matrix4x4()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>::Matrix4x4( const Matrix4x4<T>& mat )
        :_11(mat._11), _12(mat._12), _13(mat._13), _14(mat._14),
        _21(mat._21), _22(mat._22), _23(mat._23), _24(mat._24),
        _31(mat._31), _32(mat._32), _33(mat._33), _34(mat._34),
        _41(mat._41), _42(mat._42), _43(mat._43), _44(mat._44)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>::Matrix4x4(
        const T& _11c, const T& _12c, const T& _13c, const T& _14c,
        const T& _21c, const T& _22c, const T& _23c, const T& _24c, 
        const T& _31c, const T& _32c, const T& _33c, const T& _34c, 
        const T& _41c, const T& _42c, const T& _43c, const T& _44c )
        :_11(_11c), _12(_12c), _13(_13c), _14(_14c),
        _21(_21c), _22(_22c), _23(_23c), _24(_24c),
        _31(_31c), _32(_32c), _33(_33c), _34(_34c),
        _41(_41c), _42(_42c), _43(_43c), _44(_44c)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>& Matrix4x4<T>::operator=( const Matrix4x4<T>& mat )
    {
        _11 = mat._11;
        _12 = mat._12;
        _13 = mat._13;
        _14 = mat._14;

        _21 = mat._21;
        _22 = mat._22;
        _23 = mat._23;
        _24 = mat._24;

        _31 = mat._31;
        _32 = mat._32;
        _33 = mat._33;
        _34 = mat._34;

        _41 = mat._41;
        _42 = mat._42;
        _43 = mat._43;
        _44 = mat._44;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::operator-() const
    {
        return Matrix4x4<T>( 
            -_11, -_12, -_13, -_14,
            -_21, -_22, -_23, -_24,
            -_31, -_32, -_33, -_34,
            -_41, -_42, -_43, -_44 );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::operator+( const T v ) const
    {
        return Matrix4x4<T>( 
            _11 + v, _12 + v, _13 + v, _14 + v,
            _21 + v, _22 + v, _23 + v, _24 + v,
            _31 + v, _32 + v, _33 + v, _34 + v,
            _41 + v, _42 + v, _43 + v, _44 + v );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::operator-( const T v ) const
    {
        return Matrix4x4<T>( 
            _11 - v, _12 - v, _13 - v, _14 - v,
            _21 - v, _22 - v, _23 - v, _24 - v,
            _31 - v, _32 - v, _33 - v, _34 - v,
            _41 - v, _42 - v, _43 - v, _44 - v );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::operator*( const T v ) const
    {
        return Matrix4x4<T>( 
            _11 * v, _12 * v, _13 * v, _14 * v,
            _21 * v, _22 * v, _23 * v, _24 * v,
            _31 * v, _32 * v, _33 * v, _34 * v,
            _41 * v, _42 * v, _43 * v, _44 * v );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>& Matrix4x4<T>::operator+=( const T v )
    {
        _11 += v;  _12 += v;  _13 += v;  _14 += v;
        _21 += v;  _22 += v;  _23 += v;  _24 += v;
        _31 += v;  _32 += v;  _33 += v;  _34 += v;
        _41 += v;  _42 += v;  _43 += v;  _44 += v;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>& Matrix4x4<T>::operator-=( const T v )
    {
        _11 -= v;  _12 -= v;  _13 -= v;  _14 -= v;
        _21 -= v;  _22 -= v;  _23 -= v;  _24 -= v;
        _31 -= v;  _32 -= v;  _33 -= v;  _34 -= v;
        _41 -= v;  _42 -= v;  _43 -= v;  _44 -= v;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>& Matrix4x4<T>::operator*=( const T v )
    {
        _11 *= v;  _12 *= v;  _13 *= v;  _14 *= v;
        _21 *= v;  _22 *= v;  _23 *= v;  _24 *= v;
        _31 *= v;  _32 *= v;  _33 *= v;  _34 *= v;
        _41 *= v;  _42 *= v;  _43 *= v;  _44 *= v;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::operator+( const Matrix4x4<T>& rhs ) const
    {
        return Matrix4x4<T>( 
            _11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
            _21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
            _31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
            _41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44 );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::operator-( const Matrix4x4<T>& rhs ) const
    {
        return Matrix4x4<T>( 
            _11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
            _21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
            _31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
            _41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44 );
    }
    
    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::operator*( const Matrix4x4<T>& rhs ) const
    {
#if defined(__ARM_NEON__) && !defined(__arm64__)
        Matrix4x4<T> out;
        NEON_Matrix4Mul(_m, rhs._m, out._m);
        return out;
#else
        return Matrix4x4<T>(
                         m[0][0]*rhs.m[0][0] + m[0][1]*rhs.m[1][0] + m[0][2]*rhs.m[2][0] + m[0][3]*rhs.m[3][0],
                         m[0][0]*rhs.m[0][1] + m[0][1]*rhs.m[1][1] + m[0][2]*rhs.m[2][1] + m[0][3]*rhs.m[3][1],
                         m[0][0]*rhs.m[0][2] + m[0][1]*rhs.m[1][2] + m[0][2]*rhs.m[2][2] + m[0][3]*rhs.m[3][2],
                         m[0][0]*rhs.m[0][3] + m[0][1]*rhs.m[1][3] + m[0][2]*rhs.m[2][3] + m[0][3]*rhs.m[3][3],
                         m[1][0]*rhs.m[0][0] + m[1][1]*rhs.m[1][0] + m[1][2]*rhs.m[2][0] + m[1][3]*rhs.m[3][0],
                         m[1][0]*rhs.m[0][1] + m[1][1]*rhs.m[1][1] + m[1][2]*rhs.m[2][1] + m[1][3]*rhs.m[3][1],
                         m[1][0]*rhs.m[0][2] + m[1][1]*rhs.m[1][2] + m[1][2]*rhs.m[2][2] + m[1][3]*rhs.m[3][2],
                         m[1][0]*rhs.m[0][3] + m[1][1]*rhs.m[1][3] + m[1][2]*rhs.m[2][3] + m[1][3]*rhs.m[3][3],
                         m[2][0]*rhs.m[0][0] + m[2][1]*rhs.m[1][0] + m[2][2]*rhs.m[2][0] + m[2][3]*rhs.m[3][0],
                         m[2][0]*rhs.m[0][1] + m[2][1]*rhs.m[1][1] + m[2][2]*rhs.m[2][1] + m[2][3]*rhs.m[3][1],
                         m[2][0]*rhs.m[0][2] + m[2][1]*rhs.m[1][2] + m[2][2]*rhs.m[2][2] + m[2][3]*rhs.m[3][2],
                         m[2][0]*rhs.m[0][3] + m[2][1]*rhs.m[1][3] + m[2][2]*rhs.m[2][3] + m[2][3]*rhs.m[3][3],
                         m[3][0]*rhs.m[0][0] + m[3][1]*rhs.m[1][0] + m[3][2]*rhs.m[2][0] + m[3][3]*rhs.m[3][0],
                         m[3][0]*rhs.m[0][1] + m[3][1]*rhs.m[1][1] + m[3][2]*rhs.m[2][1] + m[3][3]*rhs.m[3][1],
                         m[3][0]*rhs.m[0][2] + m[3][1]*rhs.m[1][2] + m[3][2]*rhs.m[2][2] + m[3][3]*rhs.m[3][2],
                         m[3][0]*rhs.m[0][3] + m[3][1]*rhs.m[1][3] + m[3][2]*rhs.m[2][3] + m[3][3]*rhs.m[3][3]
                         );
#endif
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>& Matrix4x4<T>::operator+=( const Matrix4x4<T>& rhs )
    {
        _11 += rhs._11;   _12 += rhs._12;   _13 += rhs._13;   _14 += rhs._14;
        _21 += rhs._21;   _22 += rhs._22;   _23 += rhs._23;   _24 += rhs._24;
        _31 += rhs._31;   _32 += rhs._32;   _33 += rhs._33;   _34 += rhs._34;
        _41 += rhs._41;   _42 += rhs._42;   _43 += rhs._43;   _44 += rhs._44;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>& Matrix4x4<T>::operator-=( const Matrix4x4<T>& rhs )
    {
        _11 -= rhs._11;   _12 -= rhs._12;   _13 -= rhs._13;   _14 -= rhs._14;
        _21 -= rhs._21;   _22 -= rhs._22;   _23 -= rhs._23;   _24 -= rhs._24;
        _31 -= rhs._31;   _32 -= rhs._32;   _33 -= rhs._33;   _34 -= rhs._34;
        _41 -= rhs._41;   _42 -= rhs._42;   _43 -= rhs._43;   _44 -= rhs._44;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T>& Matrix4x4<T>::operator*=( const Matrix4x4<T>& rhs )
    {
        (*this) = (*this) * rhs;
        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix4x4<T>::operator==( const Matrix4x4<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix4x4<T>::operator!=( const Matrix4x4<T>& rhs ) const
    {
        return !( (*this)==rhs );
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix4x4<T>::equals( const Matrix4x4<T>& rhs, const T& tolerance ) const
    {
        for (s32 i = 0; i < 4*4; ++i)
        {
            if (!Math::Equals( _m[i], rhs._m[i], tolerance ))
            {
                return false;
            }
        }
        return true;
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4<T>::identity()
    {
        _11 = 1;    _12 = 0;    _13 = 0;    _14 = 0;
        _21 = 0;    _22 = 1;    _23 = 0;    _24 = 0;
        _31 = 0;    _32 = 0;    _33 = 1;    _34 = 0;
        _41 = 0;    _42 = 0;    _43 = 0;    _44 = 1;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix4x4<T>::isIdentity() const
    {
        return (*this) == getIdentity();
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::getIdentity() const
    {
        return Matrix4x4<T>(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::getZero() const
    {
        return Matrix4x4<T>(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix4x4<T>::isNull() const
    {
        return (*this) == getZero();
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::getInverse() const
    {
        T m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
        T m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
        T m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
        T m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

        T v0 = m20 * m31 - m21 * m30;
        T v1 = m20 * m32 - m22 * m30;
        T v2 = m20 * m33 - m23 * m30;
        T v3 = m21 * m32 - m22 * m31;
        T v4 = m21 * m33 - m23 * m31;
        T v5 = m22 * m33 - m23 * m32;

        T t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
        T t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
        T t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
        T t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

        T invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        T d00 = t00 * invDet;
        T d10 = t10 * invDet;
        T d20 = t20 * invDet;
        T d30 = t30 * invDet;

        T d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        T d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        T d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        T d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        T d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        T d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        T d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        T d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        T d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        T d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        T d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        T d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        return Matrix4x4<T>(
            d00, d01, d02, d03,
            d10, d11, d12, d13,
            d20, d21, d22, d23,
            d30, d31, d32, d33);
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4<T>::inverse()
    {
        *this = this->getInverse();
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix4x4<T>::transpose()
    {
        Math::Swap( m[0][1], m[1][0] );
        Math::Swap( m[0][2], m[2][0] );
        Math::Swap( m[0][3], m[3][0] );

        Math::Swap( m[1][2], m[2][1] );
        Math::Swap( m[1][3], m[3][1] );

        Math::Swap( m[2][3], m[3][2] );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix4x4<T> Matrix4x4<T>::getTranspose() const
    {
        return Matrix4x4<T>(
            m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3]);
    }

    //-----------------------------------------------
    template<class T>
    inline const Vector3<T>& Matrix4x4<T>::getAxisX() const
    {
        return *(Vector3<T>*)(&_11);
    }
    template<class T>
    inline const Vector3<T>& Matrix4x4<T>::getAxisY() const
    {
        return *(Vector3<T>*)(&_21);
    }
    template<class T>
    inline const Vector3<T>& Matrix4x4<T>::getAxisZ() const
    {
        return *(Vector3<T>*)(&_31);
    }



} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_MATRIX4X4_H_20100317111054__