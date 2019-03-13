// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define axially aligned bounding box of the system

#ifndef __BP_AABBOX_H_2010051403039__
#define __BP_AABBOX_H_2010051403039__

#include "vector3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	axially aligned bounding box
    //--------------------------------------------------
    template<class T>
    struct AABBox
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        Vector3<T>     min;
        Vector3<T>    max;    // must >= min

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        AABBox();
        AABBox( const AABBox& rhs );
        AABBox( const Vector3<T> min, const Vector3<T>& max );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        AABBox& operator=( const AABBox& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const AABBox& rhs ) const;
        bool operator!=( const AABBox& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const AABBox& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the AABBox whether is null
        bool isNull() const;

        //* check the min < max
        bool isVaild() const;

        //* repair the box if min > max
        void repair();

        //* move box
        void move( const Vector3<T>& direction );

        //* get center point
        Vector3<T> getCenter() const;

        //* get size vector of box
        //* @return:
        //*     vector from max to min
        Vector3<T> getSize() const;

        //* get volume of box
        T getVolume() const;

        //* get the axis-x border length
        T getLengthX() const;

        //* get the axis-y border length
        T getLengthY() const;

        //* get the axis-z border length
        T getLengthZ() const;

        //* judge the specify point whether is inside of aabbox.
        bool isPointIn( const T& x, const T& y, const T& z ) const;
        bool isPointIn( const T& x, const T& y, const T& z, bool* pIsSurface ) const;
        bool isPointIn( const Vector3<T>& point ) const                                 { return isPointIn(point.x, point.y, point.z); }
        bool isPointIn( const Vector3<T>& point, bool* pIsSurface ) const               { return isPointIn(point.x, point.y, point.z, pIsSurface); }

        //* get nearest point from spacify point to this aabb
        //* @remark:
        //*     if specify point is inside of aabbox, return specify point
        Vector3<T> getNearestPoint( const Vector3<T>& point ) const;
    };

    typedef AABBox<real>    AABBox_f;
    typedef AABBox<integer> AABBox_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------
     


    //-----------------------------------------------
    template<class T>
    inline AABBox<T>::AABBox()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline AABBox<T>::AABBox( const AABBox<T>& rhs )
    {
        min = rhs.min;
        max = rhs.max;
    }

    //-----------------------------------------------
    template<class T>
    inline AABBox<T>::AABBox( const Vector3<T> cmin, const Vector3<T>& cmax )
    {
        min = cmin;
        max = cmax;
    }

    //-----------------------------------------------
    template<class T>
    inline AABBox<T>& AABBox<T>::operator=( const AABBox<T>& rhs )
    {
        min = rhs.min;
        max = rhs.max;
        return *this;
    }

    //-----------------------------------------------
    template<class T>
    inline bool AABBox<T>::operator==( const AABBox<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T>
    inline bool AABBox<T>::operator!=( const AABBox<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T>
    inline bool AABBox<T>::equals( const AABBox<T>& rhs, const T& tolerance ) const
    {
        return min.equals(rhs.min, tolerance) && max.equals(rhs.max, tolerance);
    }

    //-----------------------------------------------
    template<class T>
    inline bool AABBox<T>::isNull() const
    {
        return Math::MoreEquals(min.x, max.x)
            || Math::MoreEquals(min.y, max.y)
            || Math::MoreEquals(min.z, max.z);
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> AABBox<T>::getCenter() const
    {
        return (min + max) / 2;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> AABBox<T>::getSize() const
    {
        return max-min;
    }

    //-----------------------------------------------
    template<class T>
    inline T AABBox<T>::getVolume() const
    {
        return getLengthX() * getLengthY() * getLengthZ();        
    }

    //-----------------------------------------------
    template<class T>
    inline void AABBox<T>::move( const Vector3<T>& direction )
    {
        min += direction;
        max += direction;
    }

    //-----------------------------------------------
    template<class T>
    inline void AABBox<T>::repair()
    {
        if (max.x < min.x)  Math::Swap(max.x, min.x);
        if (max.y < min.y)  Math::Swap(max.y, min.y);
        if (max.z < min.z)  Math::Swap(max.z, min.z);
    }

    //-----------------------------------------------
    template<class T>
    inline bool AABBox<T>::isVaild() const
    {
        return !isNull();
    }

    //-----------------------------------------------
    template<class T>
    inline T AABBox<T>::getLengthX() const
    {
        return max.x - min.x;
    }

    //-----------------------------------------------
    template<class T>
    inline T AABBox<T>::getLengthY() const
    {
        return max.y - min.y;
    }

    //-----------------------------------------------
    template<class T>
    inline T AABBox<T>::getLengthZ() const
    {
        return max.z - max.z;
    }

    //-----------------------------------------------
    template<class T>
    inline bool AABBox<T>::isPointIn( const T& x, const T& y, const T& z ) const
    {
        return (x >= min.x && x <= max.x)
            && (y >= min.y && y <= max.y)
            && (z >= min.z && z <= max.z);
    }

    //-----------------------------------------------
    template<class T>
    inline bool AABBox<T>::isPointIn( const T& x, const T& y, const T& z, bool* pIsSurface ) const
    {
        if (pIsSurface)
        {
            if (min.x == x || max.x == x
                || min.y == y || max.y == y
                || min.z == z || max.z == z)
                *pIsSurface = true;
        } // if.

        return isPointIn(x, y, z);
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> AABBox<T>::getNearestPoint( const Vector3<T>& point ) const
    {
        Vector3<T> v = point;
        if (point.x < min.x)
        {
            v.x = min.x;            
        }
        else if (point.x > max.x)
        {
            v.x = max.x;
        }
        if (point.y < min.y)
        {
            v.y = min.y;            
        }
        else if (point.y > max.y)
        {
            v.y = max.y;
        }
        if (point.z < min.z)
        {
            v.z = min.z;            
        }
        else if (point.z > max.z)
        {
            v.z = max.z;
        }

        return v;
    }
	

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_AABBOX_H_2010051403039__