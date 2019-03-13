// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   geometric tests

#ifndef __BP_UTILGEOM_H_20100527175513__
#define __BP_UTILGEOM_H_20100527175513__

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	utility of 2d geometric, use for geometric tests
    //--------------------------------------------------
    class Geom2Utils
    {
    public:
        //* detect whether intersection occurred between line and Rectangle
        template<class T>
        static bool Intersect( const Line2<T>& rhs1, const Rectangle2<T>& rhs2, OUT Vector2<T>& intersectPoint );
    };


    //--------------------------------------------------
    //*	utility of 3d geometric, use for geometric tests
    //--------------------------------------------------
    class Geom3Utils
    {
    public:

        //<

        //* detect whether intersection occurred between line and line
        template<class T>
        static bool Intersect( const Line3<T>& rhs1, const Line3<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between line and ray
        template<class T>
        static bool Intersect( const Line3<T>& rhs1, const Ray3<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between line and plane
        template<class T>
        static bool Intersect( const Line3<T>& rhs1, const Plane3<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between line and aabbox
        template<class T>
        static bool Intersect( const Line3<T>& rhs1, const AABBox<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between line and sphere
        template<class T>
        static bool Intersect( const Line3<T>& rhs1, const Sphere<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //<

        //* detect whether intersection occurred between ray and ray
        template<class T>
        static bool Intersect( const Ray3<T>& rhs1, const Ray3<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between ray and plane
        template<class T>
        static bool Intersect( const Ray3<T>& rhs1, const Plane3<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between ray and aabbox
        template<class T>
        static bool Intersect( const Ray3<T>& rhs1, const AABBox<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between ray and sphere
        template<class T>
        static bool Intersect( const Ray3<T>& rhs1, const Sphere<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //<

        //* detect whether intersection occurred between plane and plane
        template<class T>
        static bool Intersect( const Plane3<T>& rhs1, const Plane3<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between plane and aabbox
        template<class T>
        static bool Intersect( const Plane3<T>& rhs1, const AABBox<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //* detect whether intersection occurred between plane and sphere
        template<class T>
        static bool Intersect( const Plane3<T>& rhs1, const Sphere<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //<

        //* detect whether intersection occurred between AABBox<T> and aabbox
        //* @parameter:
        //*     - intersectAABB:    the intersect aabbox;
        template<class T>
        static bool Intersect( const AABBox<T>& rhs1, const AABBox<T>& rhs2, OUT AABBox<T>& intersectAABB );

        //* detect whether intersection occurred between AABBox<T> and sphere
        //* @parameter:
        //*     - intersectPoint:   is the first intersect point;
        template<class T>
        static bool Intersect( const AABBox<T>& rhs1, const Sphere<T>& rhs2, OUT Vector3<T>& intersectPoint );

        //<

        //* detect whether intersection occurred between sphere and sphere
        template<class T>
        static bool Intersect( const Sphere<T>& rhs1, const Sphere<T>& rhs2, OUT Vector3<T>& intersectPoint );


        //-----------------------------------------------
        //  line


        //-----------------------------------------------
        //  ray


        //-----------------------------------------------
        //  plane


        //-----------------------------------------------
        //  AABBox<T>


        //-----------------------------------------------
        //  Sphere<T>

        //* get the distance of sphere to sphere
        template<class T>
        static T SphereDistanceToSphere( const Sphere<T>& rhs1, const Sphere<T>& rhs2 );


    };
	

    //-----------------------------------------------
    template<class T>
    inline bool Geom2Utils::Intersect( const Line2<T>& rhs1, const Rectangle2<T>& rhs2, OUT Vector2<T>& intersectPoint )
    {
        Vector2<T> v;
        Line2<T> l;
        Vector2<T> vrightBottom;
        vrightBottom.x = rhs2.pos.x + rhs2.size.width;
        vrightBottom.y = rhs2.pos.y + rhs2.size.height;

        if (rhs2.isPointIn(rhs1.start))
        {
            v = rhs1.end;
        }
        else
        {
            v = rhs1.start;
        }

        // 在矩形内
        if ( rhs2.isPointIn( v ) )
        {
            intersectPoint = rhs2.getCenter();
            return true;
        }

        // 在矩形外
        if (v.x <= rhs2.pos.x)
        {
            if (v.y <= vrightBottom.y)
            {
                l.start = rhs2.pos;
                l.end.x = rhs2.pos.x;
                l.end.y = vrightBottom.y;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;

                l.start = vrightBottom;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;
            }
            else
            {
                l.start = rhs2.pos;
                l.end.x = rhs2.pos.x;
                l.end.y = vrightBottom.y;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;

                l.end.x = vrightBottom.x;
                l.end.y = rhs2.pos.y;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;
            }
        }
        else
        {
            if (v.y <= vrightBottom.y)
            {
                l.start = vrightBottom;
                l.end.x = rhs2.pos.x;
                l.end.y = vrightBottom.y;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;

                l.end.x = vrightBottom.x;
                l.end.y = rhs2.pos.y;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;
            }
            else
            {
                l.start = vrightBottom;
                l.end.x = vrightBottom.x;
                l.end.y = rhs2.pos.y;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;

                l.start = rhs2.pos;
                if (rhs1.intersectSegment( l, intersectPoint )) return true;
            }
        }

        intersectPoint = rhs2.getCenter();
        return false;
    }

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_UTILGEOM_H_20100527175513__