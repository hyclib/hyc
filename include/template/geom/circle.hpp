// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 2d circle of system

#ifndef __BP_CIRCLE_H_20100514175658__
#define __BP_CIRCLE_H_20100514175658__

#include "vector2.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	2d circle
    //--------------------------------------------------
    template<class T>
    struct Circle
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        Vector2<T>  center;
        T           radius; // must >= 0

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Circle();
        Circle( const Circle& rhs );
        Circle( T radius, const Vector2<T>& center = Vector2<T>(0,0) );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Circle& operator=( const Circle& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Circle& rhs ) const;
        bool operator!=( const Circle& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Circle& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the Circle whether is null
        bool isNull() const;

        //* judge the spacify point whether is in the inside of circle.
        bool isPointIn( const T& x, const T& y, bool* pIsSurface = NULL ) const    { return isPointIn(Vector2<T>(x, y), pIsSurface); }
        bool isPointIn( const Vector2<T>& point, bool* pIsSurface = NULL ) const;

        //* get circumference of this circle
        T getCircumference() const;

        //* get area of this circle
        T getArea() const;

        //* get nearest point from spacify point to this circle
        Vector2<T> getNearestPoint( const Vector2<T>& point ) const;
    };

    typedef Circle<real>    Circle_f;
    typedef Circle<integer> Circle_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------


    //-----------------------------------------------
    template<class T>
    inline Circle<T>::Circle()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Circle<T>::Circle( const Circle<T>& rhs )
        : center(rhs.center), radius(rhs.radius)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Circle<T>::Circle( T cradius, const Vector2<T>& ccenter/* = Vector2<T>::ZERO*/ )
        : center(ccenter), radius(cradius)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Circle<T>& Circle<T>::operator=( const Circle<T>& rhs )
    {
        center = rhs.center;
        radius = rhs.radius;
        return *this;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Circle<T>::operator==( const Circle<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Circle<T>::operator!=( const Circle<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Circle<T>::equals( const Circle<T>& rhs, const T& tolerance ) const
    {
        return center.equals(rhs.center, tolerance) && Math::Equals(radius, rhs.radius, tolerance);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Circle<T>::isNull() const
    {
        return Math::MoreEquals( 0, radius );
    }

    //-----------------------------------------------
    template<class T>
    inline T Circle<T>::getCircumference() const
    {
        return Math::PI_2 * radius;
    }

    //-----------------------------------------------
    template<class T>
    inline T Circle<T>::getArea() const
    {
        return Math::PI * radius * radius;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Circle<T>::isPointIn( const Vector2<T>& point, bool* pIsSurface ) const
    {
        T v1 = (point-center).getLength2();
        T v2 = radius * radius;
        if (pIsSurface) *pIsSurface = (v1 == v2);
        return v1 <= v2;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Circle<T>::getNearestPoint( const Vector2<T>& point ) const
    {
        Vector2<T> q = center - point;
        T    len = q.getLength();

        return point + q * (len-radius) / len;
    }
     
     
	

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_CIRCLE_H_20100514175658__