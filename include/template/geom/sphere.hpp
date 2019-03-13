// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 3d sphere of system

#ifndef __BP_SPHERE_H_2010051795402__
#define __BP_SPHERE_H_2010051795402__

#include "vector3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	3d sphere
    //--------------------------------------------------
    template<class T>
    struct Sphere
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        Vector3<T>  center;
        T           radius; // must >= 0

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Sphere();
        Sphere( const Sphere& rhs );
        Sphere( T radius, const Vector3<T>& center = Vector3<T>(0,0,0) );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Sphere& operator=( const Sphere& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Sphere& rhs ) const;
        bool operator!=( const Sphere& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Sphere& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the Sphere whether is null
        bool isNull() const;

        //* get max circumference of this Sphere
        T getMaxCircumference() const;

        //* get area of this Sphere
        T getArea() const;

        //* get the volume of sphere
        T getVolume() const;

        //* judge the spacify point whether is in the inside of sphere.
        bool isPointIn( const Vector3<T>& point, bool* pIsSurface = NULL ) const;
        bool isPointIn( const T& x, const T& y, const T& z, bool* pIsSurface = NULL ) const { return isPointIn(Vector3<T>(x, y, z), pIsSurface); }

        //* get nearest point from spacify point to this sphere
        Vector3<T> getNearestPoint( const Vector3<T>& point ) const;
    };

    typedef Sphere<real>    Sphere_f;
    typedef Sphere<integer> Sphere_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T> 
    inline Sphere<T>::Sphere()
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Sphere<T>::Sphere( const Sphere<T>& rhs )
        : radius(rhs.radius), center(rhs.center)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Sphere<T>::Sphere( T cradius, const Vector3<T>& ccenter /*= Vector3<T>::ZERO */ )
        : radius(cradius), center(ccenter)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Sphere<T>& Sphere<T>::operator=( const Sphere<T>& rhs )
    {
        radius = rhs.radius;
        center = rhs.center;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Sphere<T>::operator==( const Sphere<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Sphere<T>::operator!=( const Sphere<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Sphere<T>::equals( const Sphere<T>& rhs, const T& tolerance ) const
    {
        return Math::Equals( radius, rhs.radius, tolerance ) && center.equals(rhs.center, tolerance);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Sphere<T>::isNull() const
    {
        return Math::LessEquals(radius, 0);
    }

    //-----------------------------------------------
    template<class T> 
    inline T Sphere<T>::getMaxCircumference() const
    {
        return Math::PI_2 * radius; 
    }

    //-----------------------------------------------
    template<class T> 
    inline T Sphere<T>::getArea() const
    {
        return Math::PI_2 * 2 * radius * radius;

    }

    //-----------------------------------------------
    template<class T>
    inline T Sphere<T>::getVolume() const
    {
        return getArea() * radius / 3;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Sphere<T>::isPointIn( const Vector3<T>& point, bool* pIsSurface ) const
    {
        T v1 = (point-center).getLength2();
        T v2 = radius * radius;
        if (pIsSurface)
            *pIsSurface = (v1 == v2);

        return v1 <= v2;        
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Sphere<T>::getNearestPoint( const Vector3<T>& point ) const
    {
        Vector3<T> q = center - point;
        T    len = q.getLength();

        return point + q * ((len-radius) / len);
    }
     


} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_SPHERE_H_2010051795402__