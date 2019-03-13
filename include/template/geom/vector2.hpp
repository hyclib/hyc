// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//  <Geometric> define 2d vector of system

#ifndef __BP_VECTOR2D_H_20100311115116__
#define __BP_VECTOR2D_H_20100311115116__

namespace hyc
{
namespace geom
{
   
    //--------------------------------------------------
    //*	2 dimension vector
    //-------------------------------------------------  
    template<class T>
    struct Vector2
    {
        //--------------------------------------------------
        //*	    variable member
        //--------------------------------------------------
        T    x;
        T    y;
      
        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Vector2();
        Vector2( const Vector2& rhs );
        Vector2( T x, T y );
        
        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Vector2& operator=( const Vector2& rhs );
        Vector2 operator-() const;

        Vector2 operator+( const Vector2& rhs ) const;
        Vector2 operator-( const Vector2& rhs ) const;

        Vector2& operator+=( const Vector2& rhs );
        Vector2& operator-=( const Vector2& rhs );

        Vector2 operator*( const T v ) const;
        Vector2 operator/( const T v ) const;

        Vector2& operator*=( const T v );
        Vector2& operator/=( const T v );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Vector2& rhs ) const;
        bool operator!=( const Vector2& rhs ) const;

        
        //--------------------------------------------------
        //*     member method
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Vector2& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the vector whether is null
        bool isNull() const;

        //* get dot product to another vector2d
        //* the result of 2d dot product is |u||v|cosθ (x1x2+y1y2)
        T dotProduct( const Vector2& rhs ) const;

        //* get cross product to another vectoer2d;
        //* the result of 2d cross product is |u||v|sinθ (x1y2-x2y1), i.e. area of triangle
        T crossProduct( const Vector2& rhs ) const;

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

        //* get getDistance of vector from another vector2d
        T getDistance( const Vector2& rhs ) const;

        //* get getDistance of vector from another vector2d
        T getDistance( const T& x, const T& y ) const;

        //* get getDistance of vector from another vector2d quickly
        //* use quake3 sqrt algorithm, no precision
        T getDistance_q( const Vector2& rhs ) const;

        //* get getDistance of vector from another vector2d quickly
        //* use quake3 sqrt algorithm, no precision
        T getDistance_q( const T& x, const T& y ) const;

        //* get getDistance square of vector from another vector2d
        T getDistance2( const Vector2& rhs ) const;

        //* get getDistance square of vector from another vector2d
        T getDistance2( const T& x, const T& y ) const;

        //* get the angle between vector and axis-x positive
        //* anticlockwise, i.e. from axis-s positive to vector
        //* @return:    the angle value in range [0~2pi] (in radian)
        f32 getTheta() const;

        //* rotate the vector anticlockwise around a center
        //* @param angle in radian.
        void rotate( const f32& angle, const Vector2& center = Vector2(0,0) );
    };

    typedef Vector2<real>       Vector2_f;
    typedef Vector2<integer>    Vector2_i;

    //----------------------------------------------------------------------------------------------------
    //! Structure of Size
    //----------------------------------------------------------------------------------------------------
    template<class T>
    struct Size 
    {
        // attribute
        union{
            struct {
                T width;
                T height;
            };
            T component[2];
        };
        
        // method
        Size()  {}
        Size( const T& w, const T& h ) : width(w), height(h)          {}
        Size( const Size& size ) : width(size.width), height(size.height)   {}

        void  operator= ( const Size& size )            { width = size.width; height = size.height; }
        bool  operator==( const Size& size ) const      { return size.width == width && size.height == height; }
        bool  operator!=( const Size& size ) const      { return !(operator==)(size); }
        Size  operator+ ( const Size& size ) const      { Size s(*this); s.width += size.width; s.height += size.height; return s; }
        Size& operator+=( const Size& size )            { width += size.width; height += size.height; return *this; }
        Size  operator- ( const Size& size ) const      { Size s(*this); s.width -= size.width; s.height -= size.height; return s; }
        Size& operator-=( const Size& size )            { width -= size.width; height -= size.height; return *this; }
        Size  operator+ ( const Vector2<T>& size )const { return Size(width+size.x, height+size.y); }
        Size& operator+=( const Vector2<T>& size )      { width+=size.x; height+=size.y; return *this; }
        Size  operator- ( const Vector2<T>& size )const { return Size(width-size.x, height-size.y); }
        Size& operator-=( const Vector2<T>& size )      { width-=size.x; height-=size.y; return *this; }
        Size  operator*( const T v ) const              { return Size(width*v, height*v); }
        Size  operator/( const T v ) const              { return Size(width/v, height/v); }
    };

    typedef Size<real>    Size_f;
    typedef Size<real>    Size_i;

    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T>::Vector2()
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T>::Vector2( const Vector2<T>& rhs )
        : x(rhs.x), y(rhs.y)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T>::Vector2( T cx, T cy )
        : x(cx), y(cy)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T>& Vector2<T>::operator=( const Vector2<T>& rhs )
    {
        x = rhs.x;
        y = rhs.y;
        return (*this);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Vector2<T>::operator==( const Vector2<T>& rhs ) const
    {
        return Math::Equals(x, rhs.x) && Math::Equals(y, rhs.y);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Vector2<T>::operator!=( const Vector2<T>& rhs ) const
    {
        return !((*this) == rhs);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T> Vector2<T>::operator-() const
    {
        return Vector2<T>(-x, -y);
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Vector2<T>::operator+( const Vector2<T>& rhs ) const
    {
        return Vector2<T>(x + rhs.x, y + rhs.y);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T> Vector2<T>::operator-( const Vector2<T>& rhs ) const
    {
        return Vector2<T>(x - rhs.x, y - rhs.y);
    }

    //     //-----------------------------------------------
    //     T Vector2<T>::operator*( const Vector2<T>& rhs ) const
    //     {
    //         return (x * rhs.x + y * rhs.y);
    //     }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T>& Vector2<T>::operator+=( const Vector2<T>& rhs )
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T>& Vector2<T>::operator-=( const Vector2<T>& rhs )
    {
        x -= rhs.x; 
        y -= rhs.y;
        return *this;
    }

    //-----------------------------------------------
    template<class T>
    inline Vector2<T> Vector2<T>::operator*( const T v ) const
    {
        return Vector2<T>(x * v, y * v);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T> Vector2<T>::operator/( const T v ) const
    {
        return Vector2<T>(x / v, y / v);
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T>& Vector2<T>::operator*=( const T v )
    {
        x *= v; 
        y *= v; 
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T>& Vector2<T>::operator/=( const T v )
    {
        x /= v; 
        y /= v; 
        return *this;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Vector2<T>::isNull() const
    {
        return (*this) == Vector2<T>(0,0);
    }

    //-----------------------------------------------
    template<class T>
    inline T Vector2<T>::dotProduct( const Vector2<T>& rhs ) const
    {
        return (x * rhs.x + y * rhs.y);
    }

    //-----------------------------------------------
    template<class T>
    inline T Vector2<T>::crossProduct( const Vector2<T>& rhs ) const
    {
        return (x * rhs.y - y * rhs.x);
    }

    //-----------------------------------------------
    template<class T>
    inline void Vector2<T>::normalize()
    {
        T lenSq = getLength2();
        if (Math::Equals(lenSq, 0))
            return;
        lenSq = (T)Math::InvSqrt( lenSq );
        x = x * lenSq;
        y = y * lenSq;
        return;
    }

    //-----------------------------------------------
    template<class T> 
    inline void Vector2<T>::normalize_q()
    {
        T lenSq = getLength2();
        if (Math::Equals(lenSq, 0))
            return;
        lenSq = (T)Math::InvSqrt_q( (f32)lenSq );
        x = x * lenSq;
        y = y * lenSq;
        return;
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector2<T>::getLength() const
    {
        return (T)Math::Sqrt( getLength2() );
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector2<T>::getLength_q() const
    {
        return (T)Math::Sqrt_q( (f32)getLength2() );
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector2<T>::getLength2() const
    {
        return (x*x + y*y);
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector2<T>::getDistance( const Vector2<T>& rhs ) const
    {
        return Vector2<T>(x - rhs.x, y - rhs.y).getLength();
    }

    //-----------------------------------------------
    template<class T>
    inline T Vector2<T>::getDistance_q( const Vector2<T>& rhs ) const
    {
        return Vector2<T>(x - rhs.x, y - rhs.y).getLength_q();
    }

    //-----------------------------------------------
    template<class T>
    inline T Vector2<T>::getDistance( const T& x, const T& y ) const
    {
        return Vector2<T>(this->x - x, this->y - y).getLength();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector2<T>::getDistance_q( const T& x, const T& y ) const
    {
        return Vector2<T>(this->x - x, this->y - y).getLength_q();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector2<T>::getDistance2( const Vector2<T>& rhs ) const
    {
        return Vector2<T>(x - rhs.x, y - rhs.y).getLength2();
    }

    //-----------------------------------------------
    template<class T> 
    inline T Vector2<T>::getDistance2( const T& x, const T& y ) const
    {
        return Vector2<T>(this->x - x, this->y - y).getLength2();
    }

    //-----------------------------------------------
    template<class T>
    inline f32 Vector2<T>::getTheta() const
    {
        f32 angle;
        if ( Math::Equals(x, 0) )
        {
            if ( y > 0 )
            {
                angle = (T)Math::PI_HALF;
                return angle;
            }
            else if ( y < 0 )
            {
                angle = -(T)Math::PI_HALF;
                return angle;
            }
            else
            {
                angle = 0;
                return angle;
            }
        } // if
        angle = (T)Math::Atan(y/x);
        return Math::MakeIn2Pi(angle);
    }

    //-----------------------------------------------
    template<class T>
    inline void Vector2<T>::rotate( const f32& angle, const Vector2<T>& center /*= Vector2<T>() */ )
    {
        const T cs = (T)Math::Cos(angle);
        const T sn = (T)Math::Sin(angle);
        x -= center.x;
        y -= center.y;
        angle = x*cs - y*sn;
        y = x*sn + y*cs;
        x = angle;
        x += center.x;
        y += center.y;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Vector2<T>::equals( const Vector2<T>& rhs, const T& tolerance ) const
    {
        return Math::Equals( x, rhs.x, tolerance ) && Math::Equals( y, rhs.y, tolerance );
    }

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_VECTOR2D_H_20100311115116__