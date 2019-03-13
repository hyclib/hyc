// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 3d ray of the system

#ifndef __BP_RAY3_H_20100520121327__
#define __BP_RAY3_H_20100520121327__

#include "vector3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	3d ray
    //--------------------------------------------------
    template<class T>
    struct Ray3
    {
        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------
        Vector3<T> origin;
        Vector3<T> direction;

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Ray3();
        Ray3( const Ray3& rhs );
        Ray3( const Vector3<T>& direction, const Vector3<T>& origin = Vector3<T>(0,0,0) );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Ray3& operator=( const Ray3& rhs );
        Ray3 operator-() const;

        // all compare operator use torlerance of MathHelper
        bool operator==( const Ray3& rhs ) const;
        bool operator!=( const Ray3& rhs ) const;

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Ray3& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the point whether is in ray
        bool isPointIn( const Vector3<T>& point ) const { return isPointIn(point.x, point.y, point.z); }
        bool isPointIn( const T& x, const T& y, const T& z ) const;

        //* get the length of ray
        T getLength() const;

        //* set the length of ray
        void setLength( const T& length );

        //* gets the position of a point t units along the ray
        //* @parameter:
        //*     - t:  the multiple of length away from ray origin
        Vector3<T> getPointOnRay( const T& t ) const;

        //* get nearest point from spacify point to this ray
        Vector3<T> getNearestPoint( const Vector3<T>& point ) const;

        //* convert to line
        Line3<T> toLine() const;

// 
//         //* detect whether intersect with the plane
//         //* @parameter:
//         //*     - lenOffOrigin:  the distance away from ray origin
//         bool intersect( const Plane3& p, OUT T& lenOffOrigin ) const;
// 
//         //* detect whether intersect with the aabbox
//         //* @remark:
//         //*     if origin point is inside of aabbox, return origin point
//         //* @parameter:
//         //*     - lenOffOrigin:  the distance away from ray origin
//         bool intersect( const AABBox& p, OUT T& lenOffOrigin ) const;
// 
//         //* detect whether intersect with the Sphere
//         //* @parameter:
//         //*     - lenOffOrigin:  the distance away from ray origin
//         bool intersect( const Sphere& p, OUT T& lenOffOrigin ) const;         
    };

    typedef Ray3<real>      Ray3_f;
    typedef Ray3<integer>   Ray3_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T> 
    inline Ray3<T>::Ray3()
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Ray3<T>::Ray3( const Ray3<T>& rhs )
        : origin(rhs.origin), direction(rhs.direction)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Ray3<T>::Ray3( const Vector3<T>& cdirection, const Vector3<T>& corigin /*= Vector3<T>::ZERO */ )
        : origin(corigin), direction(cdirection)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Ray3<T>& Ray3<T>::operator=( const Ray3<T>& rhs )
    {
        origin = rhs.origin;
        direction = rhs.direction;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline Ray3<T> Ray3<T>::operator-() const
    {
        return Ray3<T>(-direction, origin);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Ray3<T>::operator==( const Ray3<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Ray3<T>::operator!=( const Ray3<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Ray3<T>::equals( const Ray3<T>& rhs, const T& tolerance ) const
    {
        return origin.equals(rhs.origin, tolerance) && direction.equals(rhs.direction, tolerance);
    }

    //-----------------------------------------------
    template<class T> 
    inline T Ray3<T>::getLength() const
    {
        return direction.getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline void Ray3<T>::setLength( const T& length )
    {
        direction *= (length / direction.getLength());
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Ray3<T>::getPointOnRay( const T& t ) const
    {
        return Vector3<T>(origin + (direction * t));
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Ray3<T>::isPointIn( const T& x, const T& y, const T& z ) const
    {
        return toLine().isPointIn(x, y, z);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Ray3<T>::getNearestPoint( const Vector3<T>& point ) const
    {
        Vector3<T> v = toLine().getNearestPoint(point);
        if (!isPointIn(v))
        {
            if (direction.x > 0)
            {
                return v.x > origin.x ? origin+direction : origin;
            }
            else if (direction.x < 0)
            {
                return v.x < origin.x ? origin+direction : origin;
            }
            else if (direction.y > 0)
            {
                return v.y > origin.y ? origin+direction : origin;
            }
            else if (direction.y < 0)
            {
                return v.y < origin.y ? origin+direction : origin;
            }
            else if (direction.z > 0)
            {
                return v.z > origin.z ? origin+direction : origin;
            }
            else if (direction.z < 0)
            {
                return v.z < origin.z ? origin+direction : origin;
            }
        } // if

        return v;
    }

    //-----------------------------------------------
    template<class T>
    inline Line3<T> Ray3<T>::toLine() const
    {
        Line3<T> ln;
        ln.start = this->origin;
        ln.end = ln.start+this->direction;
        return ln;
    }

    // 
    //     //-----------------------------------------------
    //     bool Ray3<T>::intersect( const Plane3& p, OUT T& lenOffOrigin ) const
    //     {
    //         T cs = p.normal.dotProduct(direction);
    //         if (Math::equalsf(cs, 0))    // Parallel
    //         {
    //             return false;
    //         }
    //         else
    //         {
    //             lenOffOrigin = p.normal.dotProduct(origin) + p.d;
    //             lenOffOrigin = -(lenOffOrigin/cs);
    //             return lenOffOrigin >= 0;
    //         }
    //     }
    // 
    //     //-----------------------------------------------
    //     bool Ray3<T>::intersect( const AABBox& box, OUT T& lenOffOrigin ) const
    //     {
    //         // box 为空
    //         if (box.isNull())
    //         {
    //             lenOffOrigin = box.max;
    //             return isPointIn(box.max);
    //         }
    // 
    //         lenOffOrigin = 0;
    // //         T t;
    // //         bool hit = false;
    // //         Vector3<T> hitpoint;
    //         const Vector3<T>& min = box.min;
    //         const Vector3<T>& max = box.max;
    //         const Vector3<T>& rayorig = origin;
    //         const Vector3<T>& raydir = direction;
    // 
    //         // Check origin inside first
    //         if ( rayorig > min && rayorig < max )
    //         {
    //             lenOffOrigin = 0;
    //             return true;
    //         }
    // 
    //         // Check each face in turn, only check closest 3
    //         // Min x
    //         if (rayorig.x <= min.x && raydir.x > 0)
    //         {
    //             t = (min.x - rayorig.x) / raydir.x;
    //             if (t >= 0)
    //             {
    //                 // Substitute t back into ray and check bounds and dist
    //                 hitpoint = rayorig + raydir * t;
    //                 if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
    //                     hitpoint.z >= min.z && hitpoint.z <= max.z &&
    //                     (!hit || t < lenOffOrigin))
    //                 {
    //                     hit = true;
    //                     lenOffOrigin = t;
    //                 }
    //             }
    //         }
    //         // Max x
    //         if (rayorig.x >= max.x && raydir.x < 0)
    //         {
    //             t = (max.x - rayorig.x) / raydir.x;
    //             if (t >= 0)
    //             {
    //                 // Substitute t back into ray and check bounds and dist
    //                 hitpoint = rayorig + raydir * t;
    //                 if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
    //                     hitpoint.z >= min.z && hitpoint.z <= max.z &&
    //                     (!hit || t < lenOffOrigin))
    //                 {
    //                     hit = true;
    //                     lenOffOrigin = t;
    //                 }
    //             }
    //         }
    //         // Min y
    //         if (rayorig.y <= min.y && raydir.y > 0)
    //         {
    //             t = (min.y - rayorig.y) / raydir.y;
    //             if (t >= 0)
    //             {
    //                 // Substitute t back into ray and check bounds and dist
    //                 hitpoint = rayorig + raydir * t;
    //                 if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
    //                     hitpoint.z >= min.z && hitpoint.z <= max.z &&
    //                     (!hit || t < lenOffOrigin))
    //                 {
    //                     hit = true;
    //                     lenOffOrigin = t;
    //                 }
    //             }
    //         }
    //         // Max y
    //         if (rayorig.y >= max.y && raydir.y < 0)
    //         {
    //             t = (max.y - rayorig.y) / raydir.y;
    //             if (t >= 0)
    //             {
    //                 // Substitute t back into ray and check bounds and dist
    //                 hitpoint = rayorig + raydir * t;
    //                 if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
    //                     hitpoint.z >= min.z && hitpoint.z <= max.z &&
    //                     (!hit || t < lenOffOrigin))
    //                 {
    //                     hit = true;
    //                     lenOffOrigin = t;
    //                 }
    //             }
    //         }
    //         // Min z
    //         if (rayorig.z <= min.z && raydir.z > 0)
    //         {
    //             t = (min.z - rayorig.z) / raydir.z;
    //             if (t >= 0)
    //             {
    //                 // Substitute t back into ray and check bounds and dist
    //                 hitpoint = rayorig + raydir * t;
    //                 if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
    //                     hitpoint.y >= min.y && hitpoint.y <= max.y &&
    //                     (!hit || t < lenOffOrigin))
    //                 {
    //                     hit = true;
    //                     lenOffOrigin = t;
    //                 }
    //             }
    //         }
    //         // Max z
    //         if (rayorig.z >= max.z && raydir.z < 0)
    //         {
    //             t = (max.z - rayorig.z) / raydir.z;
    //             if (t >= 0)
    //             {
    //                 // Substitute t back into ray and check bounds and dist
    //                 hitpoint = rayorig + raydir * t;
    //                 if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
    //                     hitpoint.y >= min.y && hitpoint.y <= max.y &&
    //                     (!hit || t < lenOffOrigin))
    //                 {
    //                     hit = true;
    //                     lenOffOrigin = t;
    //                 }
    //             }
    //         }
    // 
    //         return hit;
    //     }
    // 
    //     //-----------------------------------------------
    //     bool Ray3<T>::intersect( const Sphere& p, OUT T& lenOffOrigin ) const
    //     {
    // 
    //     }


     
	

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_RAY3_H_20100520121327__