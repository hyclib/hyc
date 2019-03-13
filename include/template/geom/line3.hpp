// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 3d line of system

#ifndef __BP_LINE3_H_2010050915023__
#define __BP_LINE3_H_2010050915023__

#include "vector3.hpp"
#include "plane3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	3 dimension line
    //* it represent line segment, has length
    //-------------------------------------------------  
    template<class T>
    struct Line3
    {
        //--------------------------------------------------
        //*	    member variable
        //--------------------------------------------------
        Vector3<T>  start;
        Vector3<T>  end;

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Line3();
        Line3( const Line3& rhs );
        Line3( const Vector3<T>& start, const Vector3<T>& end );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Line3& operator=( const Line3& rhs );
        Line3 operator-() const;    // swap start and end

        // all compare operator use torlerance of MathHelper
        bool operator==( const Line3& rhs ) const;
        bool operator!=( const Line3& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Line3& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the line whether is null (start == end)
        bool isNull() const;

        //* judge the point whether is between start and end point
        bool isPointIn( const Vector3<T>& point ) const             { return isPointIn(point.x, point.y, point.z); }
        bool isPointIn( const T& x, const T& y, const T& z ) const;

        //* judge the point whether is on the straight line
        bool isPointOn( const Vector3<T>& point ) const;
        bool isPointOn( const T& x, const T& y, const T& z ) const  { return isPointOn( Vector3<T>(x, y, z) ); }

        //* judge the two lines whether is parallel
        bool isParallel( const Line3& rhs ) const;

        //* get the vector of line
        Vector3<T> getVector() const;

        //* get middle point in line
        Vector3<T> getMiddle() const;

        //* get length of line
        T getLength() const;

        //* get length of line
        //* use quake3 sqrt algorithm, no precision
        T getLength_q() const;

        //* get length square of line
        T getLength2() const;

        //* get the distance from another line
        T getDistance( const Line3& rhs ) const;

        //* get nearest point from spacify point to this line
        Vector3<T> getNearestPoint( const Vector3<T>& point ) const;
    };

    typedef Line3<real>     Line3_f;
    typedef Line3<integer>  Line3_i;

    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Line3<T>::Line3()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Line3<T>::Line3( const Line3<T>& rhs )
        : start(rhs.start), end(rhs.end)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Line3<T>::Line3( const Vector3<T>& cstart, const Vector3<T>& cend )
        : start(cstart), end(cend)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Line3<T>& Line3<T>::operator=( const Line3<T>& rhs )
    {
        start = rhs.start;
        end = rhs.end;
        return *this;
    }

    //-----------------------------------------------
    template<class T>
    inline Line3<T> Line3<T>::operator-() const
    {
        return Line3<T>(end, start);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line3<T>::operator==( const Line3<T>& rhs ) const
    {
        return equals( rhs, Math::TOLERANCE );
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line3<T>::operator!=( const Line3<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line3<T>::equals( const Line3<T>& rhs, const T& tolerance ) const
    {
        return start.equals(rhs.start, tolerance) && end.equals(rhs.end, tolerance);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line3<T>::isNull() const
    {
        return start == end;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line3<T>::isPointIn( const T& x, const T& y, const T& z ) const
    {
        // 线直线上
        if (isPointOn(x, y, z))
        {
            if (x >= Math::Min(start.x, end.x) && x <= Math::Max(start.x, end.x))
            {
                return true;
            }
        } // if

        return false;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line3<T>::isPointOn( const Vector3<T>& point ) const
    {
        // 在端点上
        if (start == point || end == point)
        {
            return true;
        }

        return Math::Equals( Math::Abs((point-start).dotProduct(start-end)), 1);
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Line3<T>::getVector() const
    {
        return end-start;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Line3<T>::getMiddle() const
    {
        return (end+start) / 2;
    }

    //-----------------------------------------------
    template<class T>
    inline T Line3<T>::getLength() const
    {
        return start.getDistance(end);
    }

    //-----------------------------------------------
    template<class T>
    inline T Line3<T>::getLength_q() const
    {
        return start.getDistance_q(end);
    }

    //-----------------------------------------------
    template<class T>
    inline T Line3<T>::getLength2() const
    {
        return start.getDistance2(end);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line3<T>::isParallel( const Line3<T>& rhs ) const
    {
        return Math::Equals( (getVector().crossProduct(rhs.getVector())).getLength2(), 0 );
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Line3<T>::getNearestPoint( const Vector3<T>& point ) const
    {
        Vector3<T> vec = getVector();

        return ( end + (-vec) * ( (point-end).dotProduct(vec) / vec.getLength2() ) );
    }

    //-----------------------------------------------
    template<class T>
    inline T Line3<T>::getDistance( const Line3<T>& rhs ) const
    {
        Vector3<T> vb = rhs.getVector();
        Plane3<T> plane(start, end, vb+start); // this line 所成的平面
        return plane.getDistance(rhs.start);
    }


} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_LINE3_H_2010050915023__