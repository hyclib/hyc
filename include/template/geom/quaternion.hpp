// Copyright(c) 2010-2010 lipengxiang
// hyui Graphic Engine
// 	<Geometric> define quaternion for system

#ifndef __BP_QUATERNION_H_20100316230725__
#define __BP_QUATERNION_H_20100316230725__


namespace hyc
{
namespace geom
{

    //--------------------------------------------------
    //*	quaternion, i.e. a rotation around an axis.
    //* has four elements (w,x,y,z), is 4 dimension; so vector operator like dot corss product can use in quaternion
    //--------------------------------------------------
    template<class T>
    struct Quaternion
	{
        //--------------------------------------------------
        //*     member variable
        //--------------------------------------------------

		T w;         // T
		T x, y, z;   // imaginary

		//--------------------------------------------------
		//*     constructor & destructor
		//--------------------------------------------------

		Quaternion();
		Quaternion( const T& w, const T& x, const T& y, const T& z );
		Quaternion( const Quaternion& q );

		
        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Quaternion& operator=( const Quaternion& rhs );
        //* the orientataion of -q is same as the orientation of q
        Quaternion operator-() const;

        Quaternion operator*( const T v ) const;
        Quaternion& operator*=( const T v );

        Quaternion operator+( const Quaternion& rhs ) const;
        Quaternion& operator+=( const Quaternion& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Quaternion& rhs ) const;
        bool operator!=( const Quaternion& rhs ) const;


        //--------------------------------------------------
        //*     method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Quaternion& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the quaternion whether is identity quaternion
        bool isIdentity() const;

        //* get the identity quaternion
        Quaternion getIdentity() const;

        //* Normalizes the quaternion
        //* null quaternion do nothing
        void normalize();

        //* Normalize the quaternion quickly. no precision
        //* null quaternion do nothing
        void normalize_q();

        //* Calculates the dot product
        //* the result of quaternion dot product is (w1w2 + x1x2 + y1y2 + z1z2)
        T dotProduct( const Quaternion& rhs ) const;

        //* get cross product with another Quaternion; in left-hand coordinate system
        //* the result of quaternion cross product is (w1w2-v1v2, [w1v2+w2v1+v2×v1])
        Quaternion crossProduct( const Quaternion& rhs ) const;

        //* get inverse quaternion; this operator is slowly
        //* a inverse quaternion can undo the transform for origin quaternion
        Quaternion getInverse() const;

        //* get the conjugate quaternion; [w -V]
        //* quicker than inverse
        //* !TIPS: conjugate q is same as inverse q when q is normalize
        Quaternion getConjugate() const;

        //* get the difference from q1, q2; e.g. q1d=q2, get d; or mean as q2-q1=d (d = q1^-1*q2)
        //* the difference quaternion is the orientation difference from two quaternion
        //* !WARNING: q1, i.e.*this, must be normalize()
        Quaternion getDifference( const Quaternion& q2 ) const;

        //* Spherical Linear Interpolation
        //* set this quaternion to the result of the interpolation between two quaternions
        //* @parameter:
        //*     - t:    [0, 1]; when t==0, result is equals start, t==1, result is equals end
        //* !WARNING: this method not check parameter valid
        void slerp( const Quaternion& start, const Quaternion& end, const T& t );

        //* Spherical Linear Interpolation; quickly than slerp
        //* set this quaternion to the result of the interpolation between two quaternions
        //* @parameter:
        //*     - t:    [0, 1]; when t==0, result is equals start, t==1, result is equals end
        //*     - cos:  start.dotProduct(end); if cos close to 1.0 , invSin is ignore
        //*     - tan:  sin / cos;  sin can get use in sqrt( 1-cos*cos )
        //*     - invSin:   1 / sin
        //* !WARNING: this method not check parameter valid
        void _slerp( const Quaternion& start, const Quaternion& end, const T& t,
            const T& cos, const T& tan, const T& invSin );

	};

    typedef Quaternion<real> Quaternion_f;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Quaternion<T>::Quaternion()
    {
    }

    template<class T>
    inline Quaternion<T>::Quaternion( const T& wc, const T& xc, const T& yc, const T& zc )
        :w(wc),x(xc),y(yc),z(zc)
    {
    }

    template<class T>
    inline Quaternion<T>::Quaternion( const Quaternion<T>& q )
        :w(q.w),x(q.x),y(q.y),z(q.z)
    {
    }

    template<class T>
    inline Quaternion<T>& Quaternion<T>::operator=( const Quaternion<T>& rhs )
    {
        w = rhs.w;
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return (*this);
    }

    //-----------------------------------------------
    template<class T> 
    inline Quaternion<T> Quaternion<T>::operator-() const
    {
        return Quaternion<T>( -w, -x, -y, -z );
    }

    template<class T>
    inline Quaternion<T> Quaternion<T>::operator*( const T v ) const
    {
        return Quaternion<T>( w*v, x*v, y*v, z*v );
    }

    template<class T>
    inline Quaternion<T>& Quaternion<T>::operator*=( const T v )
    {
        w *= v;
        x *= v;
        y *= v;
        z *= v;
        return (*this);
    }

    template<class T> 
    inline Quaternion<T> Quaternion<T>::operator+( const Quaternion<T>& rhs ) const
    {
        return Quaternion<T>( w+rhs.w, x+rhs.x, y+rhs.y, z+rhs.z );
    }

    template<class T>
    inline Quaternion<T>& Quaternion<T>::operator+=( const Quaternion<T>& rhs )
    {
        w += rhs.w;
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return (*this);
    }

    template<class T>
    inline bool Quaternion<T>::operator==( const Quaternion<T>& rhs ) const
    {
        return ( Math::Equals(w, rhs.w) &&
            Math::Equals(x, rhs.x) &&
            Math::Equals(y, rhs.y) &&
            Math::Equals(z, rhs.z) );
    }

    template<class T>
    inline bool Quaternion<T>::operator!=( const Quaternion<T>& rhs ) const
    {
        return !((*this) == rhs);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Quaternion<T>::isIdentity() const
    {
        return (*this) == getIdentity();
    }

    //-----------------------------------------------
    template<class T>
    inline Quaternion<T> Quaternion<T>::getIdentity() const
    {
        return Quaternion<T>(1,0,0,0);
    }

    //-----------------------------------------------
    template<class T> 
    inline void Quaternion<T>::normalize()
    {
        T lq = w*w + x*x + y*y + z*z;

        if ( Math::Equals(lq, 1) || Math::Equals(lq, 0) )
            return;

        //n = 1.0f / sqrtf(n);
        lq = (T)Math::InvSqrt( lq );
        (*this) *= lq;
    }

    template<class T> 
    inline void Quaternion<T>::normalize_q()
    {
        T lq = w*w + x*x + y*y + z*z;

        if ( Math::Equals(lq, 1) || Math::Equals(lq, 0) )
            return;

        //n = 1.0f / sqrtf(n);
        lq = (T)Math::InvSqrt_q( lq );
        (*this) *= lq;
    }

    //-----------------------------------------------
    template<class T>
    inline T Quaternion<T>::dotProduct( const Quaternion<T>& rhs ) const
    {
        return (w*rhs.w + x*rhs.x + y*rhs.y + z*rhs.z);
    }

    //-----------------------------------------------
    template<class T> 
    inline Quaternion<T> Quaternion<T>::crossProduct( const Quaternion<T>& rhs ) const
    {
        // [w1  v1]×[w2  v2] = [w1w2-v1v2   w1v2+w2v1+v2×v1]
        return Quaternion<T>(
            w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z,
            w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
            w*rhs.y + y*rhs.w + z*rhs.x - x*rhs.z,
            w*rhs.z + z*rhs.w + x*rhs.y - y*rhs.x
            );
    }

    //-----------------------------------------------
    template<class T> 
    inline Quaternion<T> Quaternion<T>::getInverse() const
    {
        T invModule = Math::InvSqrt( w*w + x*x + y*y + z*z );
        Quaternion<T> conjugate = getConjugate();
        conjugate *= invModule;
        return conjugate;
    }

    //-----------------------------------------------
    template<class T>
    inline Quaternion<T> Quaternion<T>::getConjugate() const
    {
        return Quaternion<T>( w, -x, -y, -z );
    }

    //-----------------------------------------------
    template<class T> 
    inline Quaternion<T> Quaternion<T>::getDifference( const Quaternion<T>& q2 ) const
    {
        return (*this).getConjugate().crossProduct( q2 );
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Quaternion<T>::equals( const Quaternion<T>& rhs, const T& tolerance ) const
    {
        return Math::Equals(w, rhs.w, tolerance)
            && Math::Equals(x, rhs.x, tolerance)
            && Math::Equals(y, rhs.y, tolerance)
            && Math::Equals(z, rhs.z, tolerance);
    }

    //-----------------------------------------------
    template<class T> 
    inline void Quaternion<T>::slerp( const Quaternion<T>& start, const Quaternion<T>& end, const T& t )
    {
        // slerp(start,end,t) = Sin(1-t)w / sinw * start + sintw / sinw * end

        T cs = start.dotProduct(end);

        // -q 的orientation == q; 点乘为负,则反转一个q,取短弧
        if (cs < 0)
        {
            (*this) = -start;
            cs = -cs;
        }
        else
        {
            (*this) = start;
        } // if...else

        // 过于接近避免除0, 使用线性插值
        T k0, k1;
        if (cs > 0.9999)
        {
            k0 = 1 - t;
            k1 = t;
        }
        else
        {
            T sn = Math::Sqrt(1 - cs*cs);
            cs = Math::Atan( sn / cs );
            sn = 1 / sn;
            k0 = Math::Sin( (1-t)*cs ) * sn;
            k1 = Math::Sin( t*cs ) * sn;
        } // if...else

        w = w*k0 + end.w*k1;
        x = x*k0 + end.x*k1;
        y = y*k0 + end.y*k1;
        z = z*k0 + end.z*k1;
    }

    //-----------------------------------------------
    template<class T> 
    inline void Quaternion<T>::_slerp( const Quaternion<T>& start, const Quaternion<T>& end, const T& t,
        const T& cos, const T& tan, const T& invSin)
    {
        // slerp(start,end,t) = Sin(1-t)w / sinw * start + sintw / sinw * end

        // -q 的orientation == q; 点乘为负,则反转一个q,取短弧
        T omega;
        if (cos < 0)
        {
            (*this) = -start;
            omega = -tan;
        }
        else
        {
            (*this) = start;
            omega = tan;
        } // if...else

        // 过于接近避免除0, 使用线性插值
        T k0, k1;
        if (cos > 0.9999)
        {
            k0 = 1 - t;
            k1 = t;
        }
        else
        {
            k0 = Math::Sin( (1-t)*omega ) * invSin;
            k1 = Math::Sin( t*omega ) * invSin;
        } // if...else

        w = w*k0 + end.w*k1;
        x = x*k0 + end.x*k1;
        y = y*k0 + end.y*k1;
        z = z*k0 + end.z*k1;
    }

     

} // end namespace geom
} // end namespace hyc

#endif // __BP_QUATERNION_H_20100316230725__