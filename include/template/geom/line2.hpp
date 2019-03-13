// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 2d line of system

#ifndef __BP_LINE2_H_2010050915130__
#define __BP_LINE2_H_2010050915130__

#include "vector2.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	2 dimension line
    //* it represent line segment, has length
    //-------------------------------------------------  
    template<class T>
    struct Line2
    {
        //--------------------------------------------------
        //*	    member variable
        //--------------------------------------------------
        Vector2<T>  start;
        Vector2<T>  end;

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Line2();
        Line2( const Line2& rhs );
        Line2( const Vector2<T>& start, const Vector2<T>& end );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Line2& operator=( const Line2& rhs );
        Line2 operator-() const;    // swap start and end

        // all compare operator use torlerance of MathHelper
        bool operator==( const Line2& rhs ) const;
        bool operator!=( const Line2& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Line2& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the line whether is null (start == end)
        bool isNull() const;

        //* judge the point whether is between start and end point
        //* @remark:
        //*     it will return true if the point == start or end
        bool isPointIn( const Vector2<T>& point ) const { return isPointIn(point.x, point.y); }
        bool isPointIn( const T& x, const T& y ) const;

        //* judge the point whether is on the straight line
        bool isPointOn( const Vector2<T>& point ) const { return isPointOn(point.x, point.y); }
        bool isPointOn( const T& x, const T& y ) const;

        //* judge the two line whether is parallel
        bool isParallel( const Line2& rhs ) const;

        //* get the vector of line
        Vector2<T> getVector() const;

        //* get middle point in line
        Vector2<T> getMiddle() const;

        //* get length of line
        T getLength() const;

        //* get length of line
        //* use quake3 sqrt algorithm, no precision
        T getLength_q() const;

        //* get length square of line
        T getLength2() const;

        //* get intersection angle with rhs line; rhs to this
        //* @remark:
        //*     the angle is that from this to rhs; positive direction is anticlockwise (in radian)
        f32 getAngleWith( const Line2& rhs ) const;

        //* get nearest point from spacify point to this line
        Vector2<T> getNearestPoint( const Vector2<T>& point ) const;

        //* get nearest distance form spacify point to this segment.
        f32 getNearestDistanceSegment( const Vector2<T>& point ) const  { return getNearestDistanceSegment(point.x, point.y); }
        f32 getNearestDistanceSegment( const T& x, const T& y ) const;

        //* get nearest distance form spacify point to this line.
        f32 getNearestDistanceLine( const Vector2<T>& point ) const { return getNearestDistanceLine(point.x, point.y); }
        f32 getNearestDistanceLine( const T& x, const T& y ) const;

        //* get the intersect point between two segment
        bool intersectSegment( const Line2& rhs, OUT Vector2<T>& intersectPoint ) const;

        //* get the intersect point between two lines.
        bool intersectLine( const Line2& rhs, OUT Vector2<T>& intersectPoint ) const;
    };

    typedef Line2<real>     Line2_f;
    typedef Line2<integer>  Line2_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Line2<T>::Line2()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Line2<T>::Line2( const Line2<T>& rhs )
        : start(rhs.start), end(rhs.end)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Line2<T>::Line2( const Vector2<T>& cstart, const Vector2<T>& cend )
        : start(cstart), end(cend)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Line2<T>& Line2<T>::operator=( const Line2<T>& rhs )
    {
        start = rhs.start;
        end = rhs.end;
        return *this;
    }

    //-----------------------------------------------
    template<class T>
    inline Line2<T> Line2<T>::operator-() const
    {
        return Line2<T>(end, start);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::operator==( const Line2<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::operator!=( const Line2<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::equals( const Line2<T>& rhs, const T& tolerance ) const
    {
        return start.equals(rhs.start, tolerance) && end.equals(rhs.end, tolerance);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::isNull() const
    {
        return start == end;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::isParallel( const Line2<T>& rhs ) const
    {
        return Math::Equals(getVector().crossProduct(rhs.getVector()), 0);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::isPointIn( const T& x, const T& y ) const
    {
        // 线直线上
        if (isPointOn(x, y))
        {
            if (
                Math::MoreEquals(y, Math::Min(start.y, end.y))
                && Math::LessEquals(y, Math::Max(start.y, end.y))
                && Math::MoreEquals(x, Math::Min(start.x, end.x))
                && Math::LessEquals(x, Math::Max(start.x, end.x))
                )
            {
                return true;
            }
        } // if

        return false;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::isPointOn( const T& x, const T& y ) const
    {
        // 在端点上
        if ( (Math::Equals(start.x, x) && Math::Equals(start.y, y))
            || (Math::Equals(end.x, x) && Math::Equals(end.y, y))
            )
        {
            return true;
        }

        if ( Math::Equals((x - start.x) * (y - end.y), (y - start.y) * (x - end.x)) )
            return true;
        return false;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Line2<T>::getVector() const
    {
        return end-start;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Line2<T>::getMiddle() const
    {
        return (end+start) / 2;
    }

    //-----------------------------------------------
    template<class T>
    inline T Line2<T>::getLength() const
    {
        return start.getDistance(end);
    }

    //-----------------------------------------------
    template<class T>
    inline T Line2<T>::getLength_q() const
    {
        return start.getDistance_q(end);
    }

    //-----------------------------------------------
    template<class T>
    inline T Line2<T>::getLength2() const
    {
        return start.getDistance2(end);
    }

    //-----------------------------------------------
    template<class T>
    inline f32 Line2<T>::getAngleWith( const Line2<T>& rhs ) const
    {
        f32 r1 = getVector().getTheta();
        f32 r2 = rhs.getVector().getTheta();
        if (r1 > Math::PI)
            r1 -= Math::PI;
        if (r2 > Math::PI)
            r2 -= Math::PI;

        return r2-r1;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Line2<T>::getNearestPoint( const Vector2<T>& point ) const
    {
        // 线为一个点
        if (isNull())
            return start;

        // x1 = (b^2*x - a*b*y - a*c) / (a^2+b^2)
        // y1 = (-a*b*x - a^2*y - b*c) / (a^2+b^2)

        T a = start.x - end.x;
        T b = end.y - start.y;
        T c = start.y*end.x - start.x*end.y;
        T a2 = a*a;
        T b2 = b*b;

        return Vector2<T>(
            (b2*point.x - a*b*point.y - a*c) / (a2+b2),
            (-a*b*point.x + a2*point.y - b*c) / (a2+b2)
            );
    }

    //-----------------------------------------------
    template<class T>
    inline f32 Line2<T>::getNearestDistanceSegment( const T& x, const T& y ) const
    {
        // see http://blog.163.com/maosth@126/blog/static/135068435201002111739759/
        f32 a, b, c;
        a = start.getDistance(x, y);
        if(a <= 0.00001f)
        {
            return 0;
        }
        b = end.getDistance(x, y);
        if(b <= 0.00001f)
        {
            return 0;
        }
        //如果start和end坐标相同，则退出函数，并返回距离.
        c = start.getDistance(end);
        if(c <= 0.00001f)
        {
            return a;
        }

        if(a * a >= b*b + c*c)
        {
            return b;
        }
        if(b * b >= a*a + c*c)
        {
            return a;
        }
        f32 l = (a + b + c) / 2; //周长的一半.
        f32 s = sqrt(l * (l -a) * (l - b) * (l - c)); // 海伦公式求面积.
        return 2 * s / c;
    }

    //-----------------------------------------------
    template<class T>
    inline f32 Line2<T>::getNearestDistanceLine( const T& x, const T& y ) const
    {
        T a = start.y-end.y;
        T b = end.x-start.x;
        T c = start.x*end.y-end.x*start.y;

        return Math::Abs(a*x+b*y+c) / Math::Sqrt(a*a+b*b);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Line2<T>::intersectSegment( const Line2<T>& rhs, OUT Vector2<T>& intersectPoint ) const
    {
        const T a1 = start.x - end.x;
        const T b1 = end.y - start.y;
        const T c1 = end.x*start.y - start.x*end.y;

        const T a2 = rhs.start.x - rhs.end.x;
        const T b2 = rhs.end.y - rhs.start.y;
        const T c2 = rhs.end.x*rhs.start.y - rhs.start.x*rhs.end.y;

        const T denominator = b2*a1 - b1*a2;

        // 不存在或两线相同
        if ( Math::Equals(0, denominator) )
        {
            // 相交
            if (isPointIn(rhs.start))
            {
                intersectPoint = rhs.start;
                return true;
            }
            if (isPointIn(rhs.end))
            {
                intersectPoint = rhs.end;
                return true;
            }
            if (rhs.isPointIn(end))
            {
                intersectPoint = end;
                return true;
            }
            if (rhs.isPointIn(start))
            {
                intersectPoint = start;
                return true;
            }
            return false;
        }

        // 判断是否在线段内
        intersectPoint.y = ( b1*c2 - b2*c1 ) / denominator;
        intersectPoint.x = ( a2*c1 - a1*c2 ) / denominator;

        if (
            Math::MoreEquals(intersectPoint.y, Math::Min(start.y, end.y))
            && Math::LessEquals(intersectPoint.y, Math::Max(start.y, end.y))
            && Math::MoreEquals(intersectPoint.x, Math::Min(start.x, end.x))
            && Math::LessEquals(intersectPoint.x, Math::Max(start.x, end.x))

            && Math::MoreEquals(intersectPoint.y, Math::Min(rhs.start.y, rhs.end.y))
            && Math::LessEquals(intersectPoint.y, Math::Max(rhs.start.y, rhs.end.y))
            && Math::MoreEquals(intersectPoint.x, Math::Min(rhs.start.x, rhs.end.x))
            && Math::LessEquals(intersectPoint.x, Math::Max(rhs.start.x, rhs.end.x))
            )
        {
            return true;
        }
        return false;
    }


    //-----------------------------------------------
    template<class T>
    inline bool hyc::geom::Line2<T>::intersectLine( const Line2& rhs, OUT Vector2<T>& intersectPoint ) const
    {
        const T a1 = start.x - end.x;
        const T b1 = end.y - start.y;
        const T c1 = end.x*start.y - start.x*end.y;

        const T a2 = rhs.start.x - rhs.end.x;
        const T b2 = rhs.end.y - rhs.start.y;
        const T c2 = rhs.end.x*rhs.start.y - rhs.start.x*rhs.end.y;

        const T denominator = b2*a1 - b1*a2;

        // 不存在或两线相同
        if ( Math::Equals(0, denominator) )
        {
            // 相交
            if (isPointIn(rhs.start))
            {
                intersectPoint = rhs.start;
                return true;
            }
            if (isPointIn(rhs.end))
            {
                intersectPoint = rhs.end;
                return true;
            }
            if (rhs.isPointIn(end))
            {
                intersectPoint = end;
                return true;
            }
            if (rhs.isPointIn(start))
            {
                intersectPoint = start;
                return true;
            }
            return false;
        }

        // 判断是否在线段内
        intersectPoint.y = ( b1*c2 - b2*c1 ) / denominator;
        intersectPoint.x = ( a2*c1 - a1*c2 ) / denominator;
        return true;
    }

     

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_LINE2_H_2010050915130__