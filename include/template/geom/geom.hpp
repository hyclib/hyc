// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// hyui Graphic Engine
//   include the interface of geometric

#ifndef __BP_GEOMETRIC_H_20100714110407__
#define __BP_GEOMETRIC_H_20100714110407__

// class declare
namespace hyc
{
namespace geom
{
    template<class T> struct AABBox;
    template<class T> struct Circle;
    template<class T> struct Cylinder;
    struct EulerAngles;
    template<class T> struct Line2;
    template<class T> struct Line3;
    template<class T> struct Matrix3x3;
    template<class T> struct Matrix4x4;
    template<class T> struct Plane3;
    template<class T> struct Quaternion;
    template<class T> struct Ray3;
    template<class T> struct Rectangle2;
    template<class T> struct Sphere;
    template<class T> struct Triangle2;
    template<class T> struct Triangle3;
    template<class T> struct Vector2;
    template<class T> struct Vector3;
    class  Geom2Util;
    class  Geom3Utils;
    class  Matrix3x3Utils;
    class  Matrix4x4Utils;
    class  QuaternionUtils;
}
}

// geometric primitive
#include "../../hycPlatform.h"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "aabbox.hpp"
#include "circle.hpp"
#include "cylinder.hpp"
#include "line2.hpp"
#include "line3.hpp"
#include "matrix3x3.hpp"
#include "matrix4x4.hpp"
#include "eulerAngles.hpp"
#include "plane3.hpp"
#include "quaternion.hpp"
#include "ray3.hpp"
#include "rectangle2.hpp"
#include "sphere.hpp"
#include "triangle2.hpp"
#include "triangle3.hpp"

#include "utils/geomUtils.hpp"
#include "utils/icurve2d.hpp"
#include "utils/bezierCurve2d.hpp"
#include "utils/colorUtils.hpp"
#include "utils/matrix3x3Utils.hpp"
#include "utils/matrix4x4Utils.hpp"
#include "utils/quaternionUtils.hpp"

#endif // end of define __BP_GEOMETRIC_H_20100714110407__
