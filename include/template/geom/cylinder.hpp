// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//  <Geometric> define 3d vector of system


#ifndef __BP_CYLINDER_H_20100717183756__
#define __BP_CYLINDER_H_20100717183756__

#include "vector3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	cylinder
    //--------------------------------------------------
    template<class T>
    struct Cylinder
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        Vector3<T>  center; // 圆柱的中心
        Vector3<T>  up;     // 从中心延轴心向上到顶的向量
        T           radius; // 底半径 must >= 0

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Cylinder();
        Cylinder( const Cylinder& rhs );

        // center:  圆柱的中心
        // up:  从中心延轴心向上到顶的向量
        // radius:  底半径 must >= 0
        Cylinder( const Vector3<T>& center, const Vector3<T>& up, T radius );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Cylinder& operator=( const Cylinder& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Cylinder& rhs ) const;
        bool operator!=( const Cylinder& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Cylinder& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the Cylinder whether is null
        bool isNull() const;

        //* get the height of cylinder
        T getHeight() const;

        //* get area of bottom circle
        T getAreaBottom() const;

        //* get area of side
        T getAreaSide() const;

        //* get area of Cylinder
        T getAreaAll() const;

        //* get the volume of Cylinder
        T getVolume() const;

        //* judge the spacify point whether is in the inside of Cylinder.
        bool isPointIn( const T& x, const T& y, const T& z, bool* pIsSurface = NULL ) const     { return isPointIn(Vector3<T>(x,y,z), pIsSurface); }
        bool isPointIn( const Vector3<T>& point, bool* pIsSurface = NULL ) const;
        bool isPointIn( const Vector3<T>& point, T halfHeight2, bool* pIsSurface = NULL ) const;   // 连续多次调用使用这个函数; halfHeight2 = (h/2)^2 或 up^2

        //* get nearest point from spacify point to this Cylinder
//         Vector3<T> getNearestPoint( const Vector3<T>& point ) const;

    private:
        T _isPointIn( const Vector3<T>& point, bool& r ) const;
    };

    typedef Cylinder<real>      Cylinder_f;
    typedef Cylinder<integer>   Cylinder_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Cylinder<T>::Cylinder()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Cylinder<T>::Cylinder( const Cylinder<T>& rhs )
        : center(rhs.center), up(rhs.up), radius(rhs.radius)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Cylinder<T>::Cylinder( const Vector3<T>& center, const Vector3<T>& up, T radius )
        : center(center), up(up), radius(radius)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Cylinder<T>& Cylinder<T>::operator=( const Cylinder<T>& rhs )
    {
        center = rhs.center;
        up = rhs.up;
        radius = rhs.radius;
        return *this;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Cylinder<T>::operator==( const Cylinder<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Cylinder<T>::operator!=( const Cylinder<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Cylinder<T>::equals( const Cylinder<T>& rhs, const T& tolerance ) const
    {
        return center.equals(rhs.center, tolerance)
            && up.equals(rhs.up, tolerance)
            && Math::Equals(radius, rhs.radius, tolerance);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Cylinder<T>::isNull() const
    {
        return Math::Equals(0, radius) || up.isNull(); 
    }

    //-----------------------------------------------
    template<class T>
    inline T Cylinder<T>::getHeight() const
    {
        return up.getLength() * 2;        
    }

    //-----------------------------------------------
    template<class T>
    inline T Cylinder<T>::getAreaBottom() const
    {
        return radius * radius * Math::PI;
    }

    //-----------------------------------------------
    template<class T>
    inline T Cylinder<T>::getAreaSide() const
    {
        return 2 * radius * Math::PI * getHeight();
    }

    //-----------------------------------------------
    template<class T>
    inline T Cylinder<T>::getAreaAll() const
    {
        return 2 * getAreaBottom() + getAreaSide();
    }

    //-----------------------------------------------
    template<class T>
    inline T Cylinder<T>::getVolume() const
    {
        return getAreaBottom() * getHeight();
    }

    //-----------------------------------------------
    template<class T>
    inline bool Cylinder<T>::isPointIn( const Vector3<T>& point, bool* pIsSurface ) const
    {
        return isPointIn(point, up.getLength2(), pIsSurface);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Cylinder<T>::isPointIn( const Vector3<T>& point, T halfHeight2, bool* pIsSurface ) const
    {
        bool r;
        T v = _isPointIn(point,r );
        if (r) {
            if (pIsSurface) *pIsSurface = (v == halfHeight2);
            return v <= halfHeight2;
        }

        return false;
    }

    //-----------------------------------------------
    template<class T>
    inline T Cylinder<T>::_isPointIn( const Vector3<T>& point, bool& r ) const
    {
        r = true;

        // 到中心轴的距离
        Vector3<T> interactPoint = center + (-up) * ( (point-center).dotProduct(up) / up.getLength2() );   // 到轴线的最近点
        interactPoint = point - interactPoint;
        T distance = interactPoint.getLength();

        if (distance > radius)
        {
            r = false;
            return 0;
        }

        return ((point-center) - interactPoint).getLength2();
    }

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_CYLINDER_H_20100717183756__