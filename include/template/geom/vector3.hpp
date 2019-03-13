// Copyright(c) 2010-2010 lipengxiang
// hyui Graphic Engine
//  <Geometric> define 3d vector of system

#ifndef __BP_VECTOR3D_H_20100316220516__
#define __BP_VECTOR3D_H_20100316220516__

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	3 dimension vector
    //------------------------------------------------- 
    template<class T>
	struct Vector3
	{
        //--------------------------------------------------
        //*	    variable member
        //--------------------------------------------------
        T    x;
        T    y;
        T    z;
		
        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Vector3();
        Vector3( const Vector3& rhs );
		Vector3( T x, T y, T z );
		
        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------


        Vector3& operator=( const Vector3& rhs );
        Vector3 operator-() const;

		Vector3 operator+( const Vector3& rhs ) const;
        Vector3 operator-( const Vector3& rhs ) const;

		Vector3& operator+=( const Vector3& rhs );
        Vector3& operator-=( const Vector3& rhs );

		Vector3 operator*( const T v ) const;
        Vector3 operator/( const T v ) const;

		Vector3& operator*=( const T v );
        Vector3& operator/=( const T v );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Vector3& rhs ) const;
        bool operator!=( const Vector3& rhs ) const;

        
        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Vector3& rhs, const T& tolerance = Math::TOLERANCE ) const;

		//* judge the vector whether is null
		bool isNull() const;

        //* get dot product with another Vector3
        //* the result of 3d dot product is |u||v|cosθ, (x1x2+y1y2+z1z2)
        T dotProduct( const Vector3& rhs ) const;

        //* get cross product with another Vector3; in left-hand coordinate system
        //* the result of 3d cross product is |u||v|sinθ, (y1z2-z1y2, z1x2-x1z2, x1y2-y1x2)
        Vector3 crossProduct( const Vector3& rhs ) const;

        //* Normalize the vector.
        //* the result of normalize is that getLength2 will be 1;  null vector do nothing
        void normalize();

        //* Normalize the vector quickly. no precision
        //* the result of normalize is that getLength2 will be 1;  null vector do nothing
        void normalize_q();

		//* get getDistance of vector from origin
		T getLength() const;

		//* get getDistance of vector from origin quickly
		//* use quake3 sqrt algorithm, no precision
		T getLength_q() const;

		//* get getDistance square of vector from origin
		T getLength2() const;

		//* get getDistance of vector from another Vector3
		T getDistance( const Vector3& rhs ) const;

		//* get getDistance of vector from another Vector3
		T getDistance( const T& x, const T& y, const T& z ) const;

		//* get getDistance of vector from another Vector3 quickly
		//* use quake3 sqrt algorithm, no precision
		T getDistance_q( const Vector3& rhs ) const;

		//* get getDistance of vector from another Vector3 quickly
		//* use quake3 sqrt algorithm, no precision
		T getDistance_q( const T& x, const T& y, const T& z ) const;

		//* get getDistance square of vector from another Vector3
		T getDistance2( const Vector3& rhs ) const;

		//* get getDistance square of vector from another Vector3
		T getDistance2( const T& x, const T& y, const T& z ) const;
	};

    typedef Vector3<real>       Vector3_f;
    typedef Vector3<integer>    Vector3_i;

    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Vector3<T>::Vector3()
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T>::Vector3( const Vector3<T>& rhs )
        : x(rhs.x), y(rhs.y), z(rhs.z)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T>::Vector3( T cx, T cy, T cz )
        : x(cx), y(cy), z(cz)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T>& Vector3<T>::operator=( const Vector3<T>& rhs )
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Vector3<T>::operator==( const Vector3<T>& rhs ) const
    {
        return Math::Equals(x, rhs.x)
            && Math::Equals(y, rhs.y)
            && Math::Equals(z, rhs.z);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Vector3<T>::operator!=( const Vector3<T>& rhs ) const
    {
        return !Math::Equals(x, rhs.x)
            || !Math::Equals(y, rhs.y) 
            || !Math::Equals(z, rhs.z);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Vector3<T>::operator-() const
    {
        return Vector3<T>(-x, -y, -z);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Vector3<T>::operator+( const Vector3<T>& rhs ) const
    {
        return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Vector3<T>::operator-( const Vector3<T>& rhs ) const
    {
        return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T>& Vector3<T>::operator+=( const Vector3<T>& rhs )
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T>& Vector3<T>::operator-=( const Vector3<T>& rhs )
    {
        x -= rhs.x;
        y -= rhs.y; 
        z -= rhs.z;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Vector3<T>::operator*( const T v ) const
    {
        return Vector3<T>(x * v, y * v, z * v);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T> Vector3<T>::operator/( const T v ) const
    {
        return Vector3<T>(x / v, y / v, z / v);
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T>& Vector3<T>::operator*=( const T v )
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector3<T>& Vector3<T>::operator/=( const T v )
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Vector3<T>::isNull() const
    {
        return (*this) == Vector3<T>(0,0,0);
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector3<T>::dotProduct( const Vector3<T>& rhs ) const
    {
        return (x * rhs.x + y * rhs.y + z * rhs.z);
    }

    //-----------------------------------------------
    template<class T>
    inline Vector3<T> Vector3<T>::crossProduct( const Vector3<T>& rhs ) const
    {
        Vector3<T> v;
        v.x = y*rhs.z - z*rhs.y;
        v.y = z*rhs.x - x*rhs.z;
        v.z = x*rhs.y - y*rhs.x;
        return v;
    }

    //-----------------------------------------------
    template<class T> 
    inline void Vector3<T>::normalize()
    {
        T len_invsqr = getLength2();
        if ( Math::Equals( 0, getLength2() ) )
        {
            return;
        }

        len_invsqr = Math::InvSqrt( getLength2() );

        x *= len_invsqr;
        y *= len_invsqr;
        z *= len_invsqr;
    }

    //-----------------------------------------------
    template<class T> 
    inline void Vector3<T>::normalize_q()
    {
        T len_invsqr = getLength2();
        if ( Math::Equals( 0, getLength2() ) )
        {
            return;
        }

        len_invsqr = (T)Math::InvSqrt_q( (f32)getLength2() );

        x *= len_invsqr;
        y *= len_invsqr;
        z *= len_invsqr;
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector3<T>::getLength() const
    {
        return Math::Sqrt( getLength2() );
    }

    //-----------------------------------------------
    template<class T>
    inline T Vector3<T>::getLength_q() const
    {
        return (T)Math::Sqrt_q( (f32)getLength2() );
    }

    //-----------------------------------------------
    template<class T>
    inline T Vector3<T>::getLength2() const
    {
        return (x*x + y*y + z*z);
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector3<T>::getDistance( const Vector3<T>& rhs ) const
    {
        return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector3<T>::getDistance( const T& x, const T& y, const T& z ) const
    {
        return Vector3<T>(this->x - x, this->y - y, this->z - z).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector3<T>::getDistance_q( const Vector3<T>& rhs ) const
    {
        return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z).getLength_q();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector3<T>::getDistance_q( const T& x, const T& y, const T& z ) const
    {
        return Vector3<T>(this->x - x, this->y - y, this->z - z).getLength_q();
    }

    //-----------------------------------------------
    template<class T>
    inline T Vector3<T>::getDistance2( const Vector3<T>& rhs ) const
    {
        return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z).getLength2();
    }

    template<class T>
    inline T Vector3<T>::getDistance2( const T& x, const T& y, const T& z ) const
    {
        return Vector3<T>(this->x - x, this->y - y, this->z - z).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Vector3<T>::equals( const Vector3<T>& rhs, const T& tolerance ) const
    {
        return Math::Equals(x, rhs.x, tolerance)
            && Math::Equals(y, rhs.y, tolerance)
            && Math::Equals(z, rhs.z, tolerance);
    }
     


} // end namespace geom
} // end namespace hyc

#endif // __BP_VECTOR3D_H_20100316220516__