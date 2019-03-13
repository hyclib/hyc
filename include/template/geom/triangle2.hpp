//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 3d triangle of system
// author:       lipengxiang
// description:  
//

#ifndef __BP_TRIANGLE2_H_20120302235626__
#define __BP_TRIANGLE2_H_20120302235626__

#include "vector2.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	2d trangle
    //--------------------------------------------------
    template<class T>
    struct Triangle2
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        Vector2<T>     v1; // in order of clockwise (lefthand coordinate)
        Vector2<T>     v2;
        Vector2<T>     v3;

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Triangle2();
        Triangle2( const Triangle2& rhs );
        Triangle2( const Vector2<T>& v1, const Vector2<T>& v2, const Vector2<T>& v3 );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Triangle2& operator=( const Triangle2& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Triangle2& rhs ) const;
        bool operator!=( const Triangle2& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Triangle2& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the Triangle2 whether is vaild
        bool isVaild() const;

        //* judge the specify point whether is in triangle.
        bool isPointIn( const Vector2<T>& point, bool* pIsSurface = NULL ) const;
        bool isPointIn( const T& x, const T& y, bool* pIsSurface = NULL ) const     { return isPointIn(Vector2<T>(x,y),pIsSurface); }

        //* get area of triangle
        T getArea() const;

        //* get perimeter of triangle
        T getPerimeter() const;

        //* get the edge which is face to v1
        T getEdge1() const;
        T getEdge12() const;     // edge1^2

        //* get the edge which is face to v2
        T getEdge2() const;
        T getEdge22() const;     // edge2^2

        //* get the edge which is face to v3
        T getEdge3() const;
        T getEdge32() const;     // edge3^2

        //* get the angle which is lie on v1
        //* @return angle use in radian
        f32 getAngle1() const;

        //* get the angle which is lie on v2
        //* @return angle use in radian
        f32 getAngle2() const;

        //* get the angle which is lie on v3
        //* @return angle use in radian
        f32 getAngle3() const;

        //* get the center of gravity
        Vector2<T> getCenterGravity() const;

        //* get the center of incircle
        Vector2<T> getIncenter() const;

        //* get the center of circumcircle
        Vector2<T> getCircumcenter() const;

        //* get apothem, i.e. the radius of incircle
        T getApothem() const;

        //* get radius of circumcircle
        T getCircumradius() const;
    };

    typedef Triangle2<real>     Triangle2_f;
    typedef Triangle2<integer>  Triangle2_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------


    //-----------------------------------------------
    template<class T>
    inline Triangle2<T>::Triangle2()
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Triangle2<T>::Triangle2( const Triangle2<T>& rhs )
        : v1(rhs.v1), v2(rhs.v2), v3(rhs.v3)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Triangle2<T>::Triangle2( const Vector2<T>& cv1, const Vector2<T>& cv2, const Vector2<T>& cv3 )
        : v1(cv1), v2(cv2), v3(cv3)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Triangle2<T>& Triangle2<T>::operator=( const Triangle2<T>& rhs )
    {
        v1 = rhs.v1;
        v2 = rhs.v2;
        v3 = rhs.v3;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle2<T>::operator==( const Triangle2<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle2<T>::operator!=( const Triangle2<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle2<T>::equals( const Triangle2<T>& rhs, const T& tolerance ) const
    {
        return v1.equals(rhs.v1, tolerance) && v2.equals(rhs.v2, tolerance) && v3.equals(rhs.v3, tolerance);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle2<T>::isVaild() const
    {
        if (v1 == v2 || v2 == v3 || v3 == v1)
        {
            return false;
        }

        return !Math::Equals( 0, (v1-v2).crossProduct(v2-v3) );
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle2<T>::isPointIn( const Vector2<T>& point, bool* pIsSurface ) const
    {
        Vector2<T> vv0 = v3 - v1;
        Vector2<T> vv1 = v2 - v1;
        Vector2<T> vv2 = point - v1;

        T dot00 = vv0.dotProduct(vv0) ;
        T dot01 = vv0.dotProduct(vv1) ;
        T dot02 = vv0.dotProduct(vv2) ;
        T dot11 = vv1.dotProduct(vv1) ;
        T dot12 = vv1.dotProduct(vv2) ;

        T inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

        T u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
        if (u < 0 || u > 1) // if u out of range, return directly
        {
            return false ;
        }

        T v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
        if (v < 0 || v > 1) // if v out of range, return directly
        {
            return false ;
        }

        u += v;
        if (u <= 1)
        {
            if (pIsSurface) *pIsSurface = (u == 1);
            return true;
        }

        return false;    
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getArea() const
    {
        return Math::Abs((v1-v2).crossProduct(v2-v3));
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getPerimeter() const
    {
        return getEdge1() + getEdge2() + getEdge3();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getEdge1() const
    {
        return (v2-v3).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getEdge2() const
    {
        return (v1-v3).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getEdge3() const
    {
        return (v1-v2).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getEdge12() const
    {
        return (v2-v3).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getEdge22() const
    {
        return (v1-v3).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle2<T>::getEdge32() const
    {
        return (v1-v2).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline f32 Triangle2<T>::getAngle1() const
    {
        Vector2<T> _2 = v3-v1;
        Vector2<T> _3 = v2-v1;
        return 
            Math::Acos( _2.dotProduct(_3) / Math::Sqrt(_2.getLength2() + _3.getLength2()) );
    }

    //-----------------------------------------------
    template<class T> 
    inline f32 Triangle2<T>::getAngle2() const
    {
        Vector2<T> _1 = v3-v2;
        Vector2<T> _3 = v1-v2;
        return
            Math::Acos( _1.dotProduct(_3) / Math::Sqrt(_1.getLength2() + _3.getLength2()) );
    }

    //-----------------------------------------------
    template<class T>
    inline f32 Triangle2<T>::getAngle3() const
    {
        Vector2<T> _1 = v2-v3;
        Vector2<T> _2 = v1-v3;
        return
            Math::Acos( _1.dotProduct(_2) / Math::Sqrt(_1.getLength2() + _2.getLength2()) );
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Triangle2<T>::getCenterGravity() const
    {
        return Vector2<T>(
            (v1.x+v2.x+v3.x) / 3,
            (v1.y+v2.y+v3.y) / 3
            );
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Triangle2<T>::getIncenter() const
    {
        T l1 = getEdge1();
        T l2 = getEdge2();
        T l3 = getEdge3();
        T p = l1 + l2 + l3;

        return Vector2<T>(
            (l1*v1.x + l2*v2.x + l3*v3.x) / p,
            (l1*v1.y + l2*v2.y + l3*v3.y) / p
            );
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Triangle2<T>::getCircumcenter() const
    {
        Vector2<T> e1 = v3-v2;
        Vector2<T> e2 = v1-v3;
        Vector2<T> e3 = v2-v1;

        T d1 = -e2.dotProduct(e3);
        T d2 = -e3.dotProduct(e1);
        T d3 = -e1.dotProduct(e2);

        T c1 = d2*d3;
        T c2 = d3*d1;
        T c3 = d1*d2;
        T _2c = (c1+c2+c3)*2;

        return Vector2<T>(
            ( (c2+c3)*v1.x + (c3+c1)*v2.x + (c1+c2)*v3.x ) / _2c,
            ( (c2+c3)*v1.y + (c3+c1)*v2.y + (c1+c2)*v3.y ) / _2c
            );
    }

    //-----------------------------------------------
    template<class T>
    inline T Triangle2<T>::getCircumradius() const
    {
        Vector2<T> e1 = v3-v2;
        Vector2<T> e2 = v1-v3;
        Vector2<T> e3 = v2-v1;

        T d1 = -e2.dotProduct(e3);
        T d2 = -e3.dotProduct(e1);
        T d3 = -e1.dotProduct(e2);

        T c = (d2*d3 + d3*d1 + d1*d2);

        return Math::Sqrt( (d1+d2) * (d2+d3) * (d3+d1) / c ) / 2;
    }

    //-----------------------------------------------
    template<class T>
    inline T Triangle2<T>::getApothem() const
    {
        return getArea() / getPerimeter();
    }

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_TRIANGLE2_H_20120302235626__