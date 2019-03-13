// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 3x3 matrix for system 
//   2d affine transformation (translation, perspective projection)
//      linear transformation (rotate, scale, orthogonal projection, reflect...)

#ifndef __BP_MATRIX3X3_H_20100330135639__
#define __BP_MATRIX3X3_H_20100330135639__

#include "vector3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	    3x3 matrix
    //--------------------------------------------------
    template<class T>
    struct Matrix3x3
    {
        //--------------------------------------------------
        //*     variable
        //--------------------------------------------------

        union {
            T m[3][3];
            T _m[3*3];
            struct { 
                T  _11, _12, _13;
                T  _21, _22, _23;
                T  _31, _32, _33;
            }; 
        };


        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Matrix3x3();
        Matrix3x3( const Matrix3x3& mat );
        Matrix3x3( 
            const T& _11, const T& _12, const T& _13,
            const T& _21, const T& _22, const T& _23,
            const T& _31, const T& _32, const T& _33 );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Matrix3x3& operator=( const Matrix3x3& mat );
        Matrix3x3 operator-() const;

        Matrix3x3 operator+( const T v ) const;
        Matrix3x3 operator-( const T v ) const;
        Matrix3x3 operator*( const T v ) const;

        Matrix3x3& operator+=( const T v );
        Matrix3x3& operator-=( const T v );
        Matrix3x3& operator*=( const T v );

        Matrix3x3 operator+( const Matrix3x3& rhs ) const;
        Matrix3x3 operator-( const Matrix3x3& rhs ) const;
        Matrix3x3 operator*( const Matrix3x3& rhs ) const;    // matrix transform

        Matrix3x3& operator+=( const Matrix3x3& rhs );
        Matrix3x3& operator-=( const Matrix3x3& rhs );
        Matrix3x3& operator*=( const Matrix3x3& rhs );    // matrix transform

        const T*   operator[]( int i ) const   { return &m[i][0]; }
        T*         operator[]( int i )         { return &m[i][0]; }

        // all compare operator use torlerance of MathHelper
        bool operator==( const Matrix3x3& rhs ) const;
        bool operator!=( const Matrix3x3& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Matrix3x3& m, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the matrix whether is zero matrix
        bool isNull() const;

        //* set the matrix to identity matirx
        void identity();

        //* judge the matrix whether is identity matrix
        bool isIdentity() const;

        //* get identity matrix
        Matrix3x3 getIdentity() const;

        //* get zero matrix
        Matrix3x3 getZero() const;

        //* get inverse matrix; this operator is slowly
        //* a inverse matrix can undo the transform for origin matrix
        Matrix3x3 getInverse() const;

        //* mark inverse matrix; this operator is slowly
        //* a inverse matrix can undo the transform for origin matrix
        void inverse();

        //* get transpose matrix; i.e.turnover through the catercorner, do this operator is quickly
        //* !TIPS: when the matrix is orthonormal, the transpose matrix is same as inverse matrix
        Matrix3x3 getTranspose() const;

        //* mark transpose to matrix; (quicker than getTranspose)
        //* !TIPS: when the matrix is orthonormal, the transpose matrix is same as inverse matrix
        void transpose();

        //* get axis x,y,z
        const Vector3<T>& getAxisX() const;
        const Vector3<T>& getAxisY() const;
        const Vector3<T>& getAxisZ() const;
    };

    typedef Matrix3x3<real>  Matrix3x3_f;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>::Matrix3x3()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>::Matrix3x3( const Matrix3x3<T>& mat )
        :_11(mat._11), _12(mat._12), _13(mat._13),
        _21(mat._21), _22(mat._22), _23(mat._23),
        _31(mat._31), _32(mat._32), _33(mat._33)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>::Matrix3x3(
        const T& _11c, const T& _12c, const T& _13c,
        const T& _21c, const T& _22c, const T& _23c,
        const T& _31c, const T& _32c, const T& _33c )
        :_11(_11c), _12(_12c), _13(_13c),
        _21(_21c), _22(_22c), _23(_23c),
        _31(_31c), _32(_32c), _33(_33c)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>& Matrix3x3<T>::operator=( const Matrix3x3<T>& mat )
    {
        _11 = mat._11;
        _12 = mat._12;
        _13 = mat._13;

        _21 = mat._21;
        _22 = mat._22;
        _23 = mat._23;

        _31 = mat._31;
        _32 = mat._32;
        _33 = mat._33;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::operator-() const
    {
        return Matrix3x3<T>( 
            -_11, -_12, -_13,
            -_21, -_22, -_23,
            -_31, -_32, -_33 );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::operator+( const T v ) const
    {
        return Matrix3x3<T>( 
            _11 + v, _12 + v, _13 + v,
            _21 + v, _22 + v, _23 + v,
            _31 + v, _32 + v, _33 + v );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::operator-( const T v ) const
    {
        return Matrix3x3<T>( 
            _11 - v, _12 - v, _13 - v,
            _21 - v, _22 - v, _23 - v,
            _31 - v, _32 - v, _33 - v );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::operator*( const T v ) const
    {
        return Matrix3x3<T>( 
            _11 * v, _12 * v, _13 * v,
            _21 * v, _22 * v, _23 * v,
            _31 * v, _32 * v, _33 * v );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>& Matrix3x3<T>::operator+=( const T v )
    {
        _11 += v;  _12 += v;  _13 += v;
        _21 += v;  _22 += v;  _23 += v;
        _31 += v;  _32 += v;  _33 += v;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>& Matrix3x3<T>::operator-=( const T v )
    {
        _11 -= v;  _12 -= v;  _13 -= v;
        _21 -= v;  _22 -= v;  _23 -= v;
        _31 -= v;  _32 -= v;  _33 -= v;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>& Matrix3x3<T>::operator*=( const T v )
    {
        _11 *= v;  _12 *= v;  _13 *= v;
        _21 *= v;  _22 *= v;  _23 *= v;
        _31 *= v;  _32 *= v;  _33 *= v;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::operator+( const Matrix3x3<T>& rhs ) const
    {
        return Matrix3x3<T>( 
            _11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
            _21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
            _31 + rhs._31, _32 + rhs._32, _33 + rhs._33 );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::operator-( const Matrix3x3<T>& rhs ) const
    {
        return Matrix3x3<T>( 
            _11 - rhs._11, _12 - rhs._12, _13 - rhs._13,
            _21 - rhs._21, _22 - rhs._22, _23 - rhs._23,
            _31 - rhs._31, _32 - rhs._32, _33 - rhs._33 );
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>& Matrix3x3<T>::operator+=( const Matrix3x3<T>& rhs )
    {
        _11 += rhs._11;
        _12 += rhs._12;
        _13 += rhs._13;

        _21 += rhs._21;
        _22 += rhs._22;
        _23 += rhs._23;

        _31 += rhs._31;
        _32 += rhs._32;
        _33 += rhs._33;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>& Matrix3x3<T>::operator-=( const Matrix3x3<T>& rhs )
    {
        _11 -= rhs._11;
        _12 -= rhs._12;
        _13 -= rhs._13;

        _21 -= rhs._21;
        _22 -= rhs._22;
        _23 -= rhs._23;

        _31 -= rhs._31;
        _32 -= rhs._32;
        _33 -= rhs._33;

        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T>& Matrix3x3<T>::operator*=( const Matrix3x3<T>& rhs )
    {
        (*this) = (*this) * rhs;
        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix3x3<T>::operator==( const Matrix3x3<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix3x3<T>::operator!=( const Matrix3x3<T>& rhs ) const
    {
        return !( (*this)==rhs );
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix3x3<T>::equals( const Matrix3x3<T>& rhs, const T& tolerance ) const
    {
        for (s32 i = 0; i < 3*3; ++i)
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
    inline void Matrix3x3<T>::identity()
    {
        _11 = 1;    _12 = 0;    _13 = 0;
        _21 = 0;    _22 = 1;    _23 = 0;
        _31 = 0;    _32 = 0;    _33 = 1;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix3x3<T>::isIdentity() const
    {
        return (*this) == getIdentity();
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::getIdentity() const
    {
        return Matrix3x3<T>(1,0,0, 0,1,0, 0,0,1);
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::getZero() const
    {
        return Matrix3x3<T>(0,0,0, 0,0,0, 0,0,0);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Matrix3x3<T>::isNull() const
    {
        return (*this) == getZero();
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::getInverse() const
    {
        Matrix3x3<T> mat;
        mat._11 = m[1][1]*m[2][2] - m[1][2]*m[2][1];
        mat._12 = m[0][2]*m[2][1] - m[0][1]*m[2][2];
        mat._13 = m[0][1]*m[1][2] - m[0][2]*m[1][1];
        mat._21 = m[1][2]*m[2][0] - m[1][0]*m[2][2];
        mat._22 = m[0][0]*m[2][2] - m[0][2]*m[2][0];
        mat._23 = m[0][2]*m[1][0] - m[0][0]*m[1][2];
        mat._31 = m[1][0]*m[2][1] - m[1][1]*m[2][0];
        mat._32 = m[0][1]*m[2][0] - m[0][0]*m[2][1];
        mat._33 = m[0][0]*m[1][1] - m[0][1]*m[1][0];

        T fDet =
            m[0][0]*mat.m[0][0] +
            m[0][1]*mat.m[1][0] +
            m[0][2]*mat.m[2][0];

        T fInvDet = 1/fDet;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                mat.m[iRow][iCol] *= fInvDet;
        }

        return mat;
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix3x3<T>::inverse()
    {
        *this = this->getInverse();
    }

    //-----------------------------------------------
    template<class T>
    inline Matrix3x3<T> Matrix3x3<T>::getTranspose() const
    {
        return Matrix3x3<T>(
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2] );
    }

    //-----------------------------------------------
    template<class T>
    inline void Matrix3x3<T>::transpose()
    {
        Math::Swap( m[1][0], m[0][1] );
        Math::Swap( m[2][0], m[0][2] );
        Math::Swap( m[2][1], m[1][2] );
    }

    //-----------------------------------------------
    template<class T>
    inline const Vector3<T>& Matrix3x3<T>::getAxisX() const
    {
        return *(Vector3<T>*)(&_11);
    }
    template<class T>
    inline const Vector3<T>& Matrix3x3<T>::getAxisY() const
    {
        return *(Vector3<T>*)(&_21);
    }
    template<class T>
    inline const Vector3<T>& Matrix3x3<T>::getAxisZ() const
    {
        return *(Vector3<T>*)(&_31);
    }


} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_MATRIX3X3_H_20100330135639__