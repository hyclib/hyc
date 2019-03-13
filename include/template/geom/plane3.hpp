// Copyright(c) 2010-2010 lipengxiang
// hyui Graphic Engine
// 	<Geometric>  Plane3
//

#ifndef __BP_PLANE3_H_20100330210249__
#define __BP_PLANE3_H_20100330210249__

#include "vector3.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	Plane3
    //--------------------------------------------------
    template<class T>
    struct Plane3
    {
        //--------------------------------------------------
        //*     member variable
        //--------------------------------------------------

        Vector3<T>  normal;    // normal vector
        T           d;         // distance from origin

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Plane3();
        Plane3( const Plane3& rhs );
        Plane3( const Vector3<T>& normal, T distance );
        Plane3( const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3 );  // the order of vector is influential

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Plane3& operator=( const Plane3& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Plane3& rhs ) const;   // must call normalize() first
        bool operator!=( const Plane3& rhs ) const;   // must call normalize() first

        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        //* !WARNING: must call normalize() first
        bool equals( const Plane3& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* normalize the normal
        void normalize();

        //* increase the plane
        void increase( T step );

        //* decrease the plane
        void decrease( T step );

        //* judge the specify point is on which face about plane
        //* \return 0-inside, 1-front, -1-back.
        s32 onWhichFace( const Vector3<T>& point ) const;

        //* get the distance from specify point to plane
        T getDistance( const Vector3<T>& point ) const;

        //* get nearest point from spacify point to this plane
        Vector3<T> getNearestPoint( const Vector3<T>& point ) const;
    };

    typedef Plane3<real>    Plane3_f;
    typedef Plane3<integer> Plane3_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Plane3<T>::Plane3()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Plane3<T>::Plane3( const Plane3<T>& rhs )
        :normal(rhs.normal), d(rhs.d)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Plane3<T>::Plane3( const Vector3<T>& normalc, T distancec )
        :normal(normalc), d(distancec)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Plane3<T>::Plane3( const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3 )
    {
        normal = (v1-v2).crossProduct(v2-v3);
        normal.normalize();
        d = normal.dotProduct(v1);
    }

    //-----------------------------------------------
    template<class T>
    inline Plane3<T>& Plane3<T>::operator=( const Plane3<T>& rhs )
    {
        normal = rhs.normal;
        d = rhs.d;
        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Plane3<T>::operator==( const Plane3<T>& rhs ) const
    {
        // direction is same
        return equals(rhs, Math::TOLERANCE);
    }

    template<class T>
    inline bool Plane3<T>::operator!=( const Plane3<T>& rhs ) const
    {
        return !((*this) == rhs);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Plane3<T>::equals( const Plane3<T>& rhs, const T& tolerance ) const
    {
        // direction is same
        if ( normal == rhs.normal )
        {
            // distance is same
            if ( Math::Equals(d, rhs.d, tolerance) )
            {
                return true;
            }
        } // if

        return false;
    }

    //-----------------------------------------------
    template<class T>
    inline void Plane3<T>::normalize()
    {
        normal.normalize();
    }

    //-----------------------------------------------
    template<class T>
    inline s32 Plane3<T>::onWhichFace( const Vector3<T>& point ) const
    {
        T d = getDistance(point);    // distance from point to plane
        if ( Math::Equals( 0, d ) )
            return 0;
        else if ( d > 0 )
            return 1;
        else
            return -1;
    }

    //-----------------------------------------------
    template<class T>
    inline void Plane3<T>::increase( T step )
    {
        d += step;
    }

    //-----------------------------------------------
    template<class T>
    inline void Plane3<T>::decrease( T step )
    {
        d -= step;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Plane3<T>::getNearestPoint( const Vector3<T>& point ) const
    {
        return point + normal * (d-point.dotProduct(normal));
    }

    //-----------------------------------------------
    template<class T>
    inline T Plane3<T>::getDistance( const Vector3<T>& point ) const
    {
        return point.dotProduct(normal)-d;
    }
     

} // end namespace geom
} // end namespace hyc

#endif // __BP_PLANE3_H_20100330210249__