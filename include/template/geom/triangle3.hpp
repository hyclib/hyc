// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 3d triangle of system

#ifndef __BP_TRIANGLE3_H_20100517134726__
#define __BP_TRIANGLE3_H_20100517134726__

#include "vector3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	3d trangle
    //--------------------------------------------------
    template<class T>
    struct Triangle3
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        Vector3<T>     v1; // in order of clockwise (lefthand coordinate)
        Vector3<T>     v2;
        Vector3<T>     v3;

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Triangle3();
        Triangle3( const Triangle3& rhs );
        Triangle3( const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3 );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Triangle3& operator=( const Triangle3& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Triangle3& rhs ) const;
        bool operator!=( const Triangle3& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Triangle3& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the Triangle3 whether is vaild
        bool isVaild() const;

        //* judge the specify point is on which face about triangle
        //* \return 0-inside, 1-front, -1-back
        s32 onWhichFace( const Vector3<T>& point ) const;

        //* judge the specify point whether is is triangle.
        bool isPointIn( const Vector3<T>& point, bool* pIsSurface = NULL ) const;
        bool isPointIn( const T& x, const T& y, const T& z, bool* pIsSurface = NULL ) const { return isPointIn(Vector3<T>(x,y,z), pIsSurface); }

        //* get the normal vector of triangle
        Vector3<T> getNormal() const;

        //* get area of triangle
        T getArea() const;
        T getArea2() const;  // area^2

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
        Vector3<T> getCenterGravity() const;

        //* get the center of incircle
        Vector3<T> getIncenter() const;

        //* get the center of circumcircle
        Vector3<T> getCircumcenter() const;

        //* get apothem, i.e. the radius of incircle
        T getApothem() const;

        //* get radius of circumcircle
        T getCircumradius() const;

        //* get the position in Barycentric coordinates of specify point
        //* @remark:
        //*     Barycentric coordinates mean the weight, i.e. vectors's influencing to 
        //*     gravity center;
        //*     e.g. barycentric coordinates of gravity center is (1/3, 1/3, 1/3);
        Vector3<T> getBaryCentric( const Vector3<T>& point ) const;
    };



    typedef Triangle3<real>     Triangle3_f;
    typedef Triangle3<integer>  Triangle3_i;

    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------


    //-----------------------------------------------
    template<class T> 
    inline Triangle3<T>::Triangle3()
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Triangle3<T>::Triangle3( const Triangle3<T>& rhs )
        : v1(rhs.v1), v2(rhs.v2), v3(rhs.v3)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Triangle3<T>::Triangle3( const Vector3<T>& cv1, const Vector3<T>& cv2, const Vector3<T>& cv3 )
        : v1(cv1), v2(cv2), v3(cv3)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Triangle3<T>& Triangle3<T>::operator=( const Triangle3<T>& rhs )
    {
        v1 = rhs.v1;
        v2 = rhs.v2;
        v3 = rhs.v3;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle3<T>::operator==( const Triangle3<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle3<T>::operator!=( const Triangle3<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle3<T>::equals( const Triangle3<T>& rhs, const T& tolerance ) const
    {
        return v1.equals(rhs.v1, tolerance) && v2.equals(rhs.v2, tolerance) && v3.equals(rhs.v3, tolerance);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Triangle3<T>::isVaild() const
    {
        if (v1 == v2 || v2 == v3 || v3 == v1)
        {
            return false;
        }

        Vector3<T> vv1 = v1-v2;
        Vector3<T> vv2 = v2-v3;

        return !Math::Equals( Math::Sqrt(vv1.getLength2()*vv2.getLength2()), vv1.dotProduct(vv2) );
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Triangle3<T>::getNormal() const
    {
        return (v1-v2).crossProduct(v2-v3);
    }

    //-----------------------------------------------
    template<class T> 
    inline s32 Triangle3<T>::onWhichFace( const Vector3<T>& point ) const
    {
        T r = Plane3<T>(v1, v2, v3).getDistance(point);
        if (Math::Equals(0, r))
        {
            return 0;
        }
        else if (r > 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    //-----------------------------------------------
    template<class T>
    inline bool Triangle3<T>::isPointIn( const Vector3<T>& point, bool* pIsSurface ) const
    {
        Vector3<T> vv0 = v3 - v1;
        Vector3<T> vv1 = v2 - v1;
        Vector3<T> vv2 = point - v1;

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
    inline T Triangle3<T>::getArea() const
    {
        return getNormal().getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getArea2() const
    {
        return getNormal().getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getPerimeter() const
    {
        return getEdge1() + getEdge2() + getEdge3();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getEdge1() const
    {
        return (v2-v3).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getEdge2() const
    {
        return (v1-v3).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getEdge3() const
    {
        return (v1-v2).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getEdge12() const
    {
        return (v2-v3).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getEdge22() const
    {
        return (v1-v3).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getEdge32() const
    {
        return (v1-v2).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline f32 Triangle3<T>::getAngle1() const
    {
        Vector3<T> _2 = v3-v1;
        Vector3<T> _3 = v2-v1;
        return
            Math::Acos( _2.dotProduct(_3) / Math::Sqrt(_2.getLength2() + _3.getLength2()) );
    }

    //-----------------------------------------------
    template<class T> 
    inline f32 Triangle3<T>::getAngle2() const
    {
        Vector3<T> _1 = v3-v2;
        Vector3<T> _3 = v1-v2;
        return
            Math::Acos( _1.dotProduct(_3) / Math::Sqrt(_1.getLength2() + _3.getLength2()) );
    }

    //-----------------------------------------------
    template<class T> 
    inline f32 Triangle3<T>::getAngle3() const
    {
        Vector3<T> _1 = v2-v3;
        Vector3<T> _2 = v1-v3;
        return
            Math::Acos( _1.dotProduct(_2) / Math::Sqrt(_1.getLength2() + _2.getLength2()) );
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Triangle3<T>::getCenterGravity() const
    {
        return Vector3<T>(
            (v1.x+v2.x+v3.x) / 3,
            (v1.y+v2.y+v3.y) / 3,
            (v1.z+v2.z+v3.z) / 3
            );
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Triangle3<T>::getIncenter() const
    {
        T l1 = getEdge1();
        T l2 = getEdge2();
        T l3 = getEdge3();
        T p = l1 + l2 + l3;

        return Vector3<T>(
            (l1*v1.x + l2*v2.x + l3*v3.x) / p,
            (l1*v1.y + l2*v2.y + l3*v3.y) / p,
            (l1*v1.z + l2*v2.z + l3*v3.z) / p
            );
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Triangle3<T>::getCircumcenter() const
    {
        Vector3<T> e1 = v3-v2;
        Vector3<T> e2 = v1-v3;
        Vector3<T> e3 = v2-v1;

        T d1 = -e2.dotProduct(e3);
        T d2 = -e3.dotProduct(e1);
        T d3 = -e1.dotProduct(e2);

        T c1 = d2*d3;
        T c2 = d3*d1;
        T c3 = d1*d2;
        T _2c = (c1+c2+c3)*2;

        return Vector3<T>(
            ( (c2+c3)*v1.x + (c3+c1)*v2.x + (c1+c2)*v3.x ) / _2c,
            ( (c2+c3)*v1.y + (c3+c1)*v2.y + (c1+c2)*v3.y ) / _2c,
            ( (c2+c3)*v1.z + (c3+c1)*v2.z + (c1+c2)*v3.z ) / _2c
            );
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getApothem() const
    {
        return getArea() / getPerimeter();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Triangle3<T>::getCircumradius() const
    {
        Vector3<T> e1 = v3-v2;
        Vector3<T> e2 = v1-v3;
        Vector3<T> e3 = v2-v1;

        T d1 = -e2.dotProduct(e3);
        T d2 = -e3.dotProduct(e1);
        T d3 = -e1.dotProduct(e2);

        T c = (d2*d3 + d3*d1 + d1*d2);

        return Math::Sqrt( (d1+d2) * (d2+d3) * (d3+d1) / c ) / 2;
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Triangle3<T>::getBaryCentric( const Vector3<T>& point ) const
    {
        Vector3<T> e1 = v3-v2;
        Vector3<T> e2 = v1-v3;
        Vector3<T> e3 = v2-v1;
        Vector3<T> d1 = point-v1;
        Vector3<T> d2 = point-v2;
        Vector3<T> d3 = point-v3;
        Vector3<T> n = getNormal();
        n.normalize();

        T denominator = e1.crossProduct(e2).dotProduct(n);

        return Vector3<T>(
            e1.crossProduct(d3).dotProduct(n) / denominator,
            e2.crossProduct(d1).dotProduct(n) / denominator,
            e3.crossProduct(d2).dotProduct(n) / denominator
            );
    }
     
	

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_TRIANGLE3_H_20100517134726__