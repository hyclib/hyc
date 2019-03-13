// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   <Geometric> define 2d rectangle of system

#ifndef __BP_RECTANGLE2_H_20100511132524__
#define __BP_RECTANGLE2_H_20100511132524__

#include "vector2.hpp"

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	2 dimension rectangle
    //--------------------------------------------------
    template<class T>
    struct Rectangle2
    {
        //--------------------------------------------------
        //*	 divide the area to 9 region
        //--------------------------------------------------
        //    LT|  TOP   | RT
        //     ------------
        //    L | INSIDE | R
        //     ------------
        //    LB| BOTTOM | RB
        enum Region
        {
            RR_LT       = 1<<0, // left top corner
            RR_TOP      = 1<<1, // top
            RR_RT       = 1<<2, // right top corner
            RR_LEFT     = 1<<3, // left
            RR_INSIDE   = 1<<4, // inside
            RR_RIGHT    = 1<<5, // right
            RR_LB       = 1<<6, // left bottom
            RR_BOTTOM   = 1<<7, // bottom
            RR_RB       = 1<<8, // right bottom
        };

        //--------------------------------------------------
        //*	variable member
        //--------------------------------------------------

        Vector2<T> pos;
        Size<T>    size;

        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------

        Rectangle2();
        Rectangle2( const Rectangle2& rhs );
        Rectangle2( const Vector2<T>& leftTop, const Size<T>& size );
        Rectangle2( const T& x, const T& y, const T& width, const T& height );

        //--------------------------------------------------
        //*     override operators
        //--------------------------------------------------

        Rectangle2& operator=( const Rectangle2& rhs );

        // all compare operator use torlerance of MathHelper
        bool operator==( const Rectangle2& rhs ) const;
        bool operator!=( const Rectangle2& rhs ) const;

        //--------------------------------------------------
        //*     member
        //--------------------------------------------------

        //* use specify tolerance to compare
        bool equals( const Rectangle2& rhs, const T& tolerance = Math::TOLERANCE ) const;

        //* judge the rectangle whether Size<T> == 0
        bool isNull() const;

        //* check the size >= 0
        bool isVaild() const;

        //* move rectangle
        void move( const Vector2<T>& direction );

//         //* judge the spacify point is in which region
//         //* @return:
//         //*     return value hint the whether in border of two region, e.g.
//         //*     in border of left:  RR_INSIDE | RR_LEFT
//         //*     in border of top:   RR_INSIDE | RR_TOP
//         //*     in border of right: RR_INSIDE | RR_RIGHT
//         //*     in border of bottom:RR_INSIDE | RR_BOTTOM
//         //*     in corner of pos:     RR_INSIDE | RR_LT
//         //*     in corner of rightTop:    RR_INSIDE | RR_RT
//         //*     in corner of leftbottpm:  RR_INSIDE | RR_LB
//         //*     in corner of rightbottom: RR_INSIDE | RR_RB
//         Region inWhichRegion( const Vector2<T>& point ) const;

        //! judge the spacify point whether is in the inside of rectangle.
        bool isPointIn( const T& x, const T& y ) const;
        bool isPointIn( const T& x, const T& y, bool* pIsSurface ) const;
        bool isPointIn( const Vector2<T>& point ) const                     { return isPointIn(point.x, point.y); }
        bool isPointIn( const Vector2<T>& point, bool* pIsSurface ) const   { return isPointIn(point.x, point.y, pIsSurface); }

        //! add spacify point to rect, make a new rect which must contain point and origin rect
        void addPoint( const Vector2<T>& point );

        //! get position
        const Vector2<T>&  getPos() const;

        //! get size
        const Size<T>& getSize() const;

        //! get center point
        Vector2<T> getCenter() const;

        //! get the area of rectangle
        T getArea() const;

        //! judge the spacify rect whether is collide with this rect
        bool isCollided( const Rectangle2& rhs ) const;
        bool isCollided( const Vector2<T>& pt, const Size<T>& size ) const;

        //! merge this rect and spacify to a new rect
        void merge( const Rectangle2& rhs );

        //! get difference the rect of two rect
        //! @parameter:
        //!     - diff: the result
        //!	@return:
        //!		false: if two haven't difference area
        bool getDifference( const Rectangle2& rhs, OUT Rectangle2& diff ) const;        
    };


    typedef Rectangle2<real>    Rectangle2_f;
    typedef Rectangle2<integer> Rectangle2_i;


    //--------------------------------------------------
    //*	    implement the class
    //--------------------------------------------------

    //-----------------------------------------------
    template<class T>
    inline Rectangle2<T>::Rectangle2()
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Rectangle2<T>::Rectangle2( const Rectangle2<T>& rhs )
        : pos(rhs.pos), size(rhs.size)
    {
    }

    //-----------------------------------------------
    template<class T> 
    inline Rectangle2<T>::Rectangle2( const Vector2<T>& cleftTop, const Size<T>& csize )
        : pos(cleftTop), size(csize)
    {
    }
    
    //-----------------------------------------------
    template<class T> 
    inline Rectangle2<T>::Rectangle2( const T& x, const T& y, const T& width, const T& height)
        : pos(x, y), size(width, height)
    {
    }

    //-----------------------------------------------
    template<class T>
    inline Rectangle2<T>& Rectangle2<T>::operator=( const Rectangle2<T>& rhs )
    {
        pos = rhs.pos;
        size = rhs.size;
        return *this;
    }

    //-----------------------------------------------
    template<class T>
    inline bool Rectangle2<T>::operator==( const Rectangle2<T>& rhs ) const
    {
        return equals(rhs, Math::TOLERANCE);
    }

    //-----------------------------------------------
    template<class T>
    inline bool Rectangle2<T>::operator!=( const Rectangle2<T>& rhs ) const
    {
        return !(*this == rhs);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Rectangle2<T>::equals( const Rectangle2<T>& rhs, const T& tolerance ) const
    {
        return Math::Equals( pos.x, rhs.pos.x, tolerance ) && Math::Equals( pos.y, rhs.pos.y, tolerance )
            && Math::Equals( size.width, rhs.size.width, tolerance ) && Math::Equals( size.height, rhs.size.height, tolerance );
    }

    //-----------------------------------------------
    template<class T>
    inline bool Rectangle2<T>::isNull() const
    {
        return Math::Equals( size.width, 0 )
            || Math::Equals( size.height, 0 );
    }
    
    //-----------------------------------------------
    template<class T>
    inline bool Rectangle2<T>::isVaild() const
    {
        return size.width >= 0 && size.height >= 0;
    }

    //-----------------------------------------------
    template<class T> 
    inline void Rectangle2<T>::move( const Vector2<T>& direction )
    {
        pos += direction;
    }

    //-----------------------------------------------
    template<class T> 
    inline const Vector2<T>& Rectangle2<T>::getPos() const
    {
        return pos;
    }

    //-----------------------------------------------
    template<class T> 
    inline const Size<T>& Rectangle2<T>::getSize() const
    {
        return size;
    }

    //-----------------------------------------------
    template<class T> 
    inline Vector2<T> Rectangle2<T>::getCenter() const
    {
        return Vector2<T>( (pos.x+size.width/2), (pos.y+size.height/2) );
    }

    //-----------------------------------------------
    template<class T> 
    inline T Rectangle2<T>::getArea() const
    {
        return size.width * size.height;
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Rectangle2<T>::isPointIn( const T& x, const T& y ) const
    {
        return !(x < pos.x || x > (pos.x+size.width)
            || y < pos.y || y > (pos.y+size.height));
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Rectangle2<T>::isPointIn( const T& x, const T& y, bool* pIsSurface ) const
    {
        if (pIsSurface)
        {
            if (x == pos.x || x == pos.x+size.width
                || y == pos.y || y == pos.y+size.height)
                *pIsSurface = true;
        }

        return isPointIn(x, y);
    }

    //-----------------------------------------------
    template<class T>
    inline void Rectangle2<T>::addPoint( const Vector2<T>& point )
    {
        // x
        if (point.x < pos.x)
        {
            size.width += pos.x - point.x;
            pos.x = point.x;
        }
        else if (point.x > pos.x+size.width)
        {
            size.width = point.x-pos.x;
        }

        // y
        if (point.y > pos.y+size.height)
        {
            size.height = point.y - pos.y;
        }
        else if (point.y < pos.y)
        {
            size.height += pos.y - point.y;
            pos.y = point.y;
        }
    }

    //-----------------------------------------------
    template<class T>
    inline bool Rectangle2<T>::isCollided( const Rectangle2<T>& rhs ) const
    {
        return isPointIn(rhs.pos) 
            || isPointIn(rhs.pos.x+rhs.size.width, rhs.pos.y+rhs.size.height)
            || isPointIn(rhs.pos.x, rhs.pos.y+rhs.size.height) 
            || isPointIn(rhs.pos.x+rhs.size.width, rhs.pos.y);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Rectangle2<T>::isCollided( const Vector2<T>& pt, const Size<T>& size ) const
    {
        return isPointIn(pt) 
            || isPointIn(pt.x+size.width, pt.y+size.height)
            || isPointIn(pt.x, pt.y+size.height) 
            || isPointIn(pt.x+size.width, pt.y);
    }

    //-----------------------------------------------
    template<class T> 
    inline void Rectangle2<T>::merge( const Rectangle2<T>& rhs )
    {
        addPoint(Vector2<T>(rhs.pos.x+rhs.size.width, rhs.pos.y+rhs.size.height));
        addPoint(rhs.pos);
    }

    //-----------------------------------------------
    template<class T> 
    inline bool Rectangle2<T>::getDifference( const Rectangle2<T>& rhs, OUT Rectangle2<T>& diff ) const
    {
        diff.pos.x = Math::Max(pos.x, rhs.pos.x);
        diff.pos.y = Math::Max(pos.y, rhs.pos.y);
        diff.size.width = Math::Min(pos.x+size.width, rhs.pos.x+rhs.size.width) - diff.pos.x;
        diff.size.height = Math::Min(pos.y+size.height, rhs.pos.y+rhs.size.height) - diff.pos.y;

        return diff.isVaild();
    }
     
	

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_RECTANGLE2_H_20100511132524__