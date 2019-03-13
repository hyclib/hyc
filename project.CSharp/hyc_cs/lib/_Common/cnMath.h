//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyc
// author:       lipengxiang
// description:  
//

#pragma once

namespace Hy
{
    //----------------------------------------------------------------------------------------------------
    //* 常用数学库
    //----------------------------------------------------------------------------------------------------
    public ref class Math
    {
    private:
        Math()  {}
    public: // method
        /*---------------------------
        静态量
        ---------------------------*/
        static property hyc::real    PI     { hyc::real get() { return hyc::Math::PI; } }
        static property hyc::real    PI_2   { hyc::real get() { return hyc::Math::PI_2; } }
        static property hyc::real    PI_HALF    { hyc::real get() { return hyc::Math::PI_HALF; } }
        static property hyc::real    TOLERANCE_DEFAULT  { hyc::real get() { return hyc::Math::TOLERANCE_DEFAULT; } }
        static property hyc::real    TOLERANCE
        { 
            hyc::real get() { return hyc::Math::TOLERANCE; }
            void      set(hyc::real v) { hyc::Math::TOLERANCE = v; }
        }

        /*---------------------------
        几何函数
        ---------------------------*/
        static hyc::real Radian2Degree( hyc::real radian )    { return hyc::Math::Radian2Degree(radian); }    // 弧度(radian)->角度(degree)
        static hyc::real Degree2Radian( hyc::real degree )    { return hyc::Math::Degree2Radian(degree); }   // 角度(degree)->弧度(radian)
        static hyc::real MakeIn2Pi( hyc::real radianValue )   { return hyc::Math::MakeIn2Pi(radianValue); }   // 保证角度在[0~2pi]范围内
        static hyc::real MakeInPi( hyc::real radianValue )    { return hyc::Math::MakeInPi(radianValue); }    // 保证角度在[-pi~pi]范围内

        /*---------------------------
        浮点比较
        ---------------------------*/
        static bool Equals( hyc::real f1, hyc::real f2 )    { return hyc::Math::Equals(f1, f2); }    // ==
        static bool LessThan( hyc::real f1, hyc::real f2 )  { return hyc::Math::LessThan(f1, f2); }  //less than <
        static bool LessEquals( hyc::real f1, hyc::real f2 ){ return hyc::Math::LessEquals(f1, f2); }  //less or equals <=
        static bool MoreThan( hyc::real f1, hyc::real f2 )  { return hyc::Math::MoreThan(f1, f2); }  //more than >
        static bool MoreEquals( hyc::real f1, hyc::real f2 ){ return hyc::Math::MoreEquals(f1, f2); }  //more or equals >=
// 
//         template<class _T2>
//         static _T2& Max( _T2& a, _T2& b ) { return (((a) > (b)) ? (a) : (b)); }
//         template<class _T2>
//         static _T2& Min( _T2& a, _T2& b ) { return (((a) < (b)) ? (a) : (b)); }
//         template<class _T2>
//         static const _T2&   Max( const _T2& a, const _T2& b ) { return (((a) > (b)) ? (a) : (b)); }
//         template<class _T2>
//         static const _T2&   Min( const _T2& a, const _T2& b ) { return (((a) < (b)) ? (a) : (b)); }

        /*---------------------------
        数学函数
        ---------------------------*/
        static hyc::real Abs( hyc::real t )   { return hyc::Math::Abs(t); }
        static hyc::real Ceil( hyc::real t )  { return hyc::Math::Ceil(t); }
        static hyc::real Floor( hyc::real t ) { return hyc::Math::Floor(t); }
        static hyc::s32  Sign( hyc::real t )  { return hyc::Math::Sign(t); }

        static hyc::real Sin( hyc::real t )   { return hyc::Math::Sin(t); }
        static hyc::real Asin( hyc::real t )  { return hyc::Math::Asin(t); }
        static hyc::real Cos( hyc::real t )   { return hyc::Math::Cos(t); }
        static hyc::real Acos( hyc::real t )  { return hyc::Math::Acos(t); }
        static hyc::real Tan( hyc::real t )   { return hyc::Math::Tan(t); }
        static hyc::real Atan( hyc::real t )  { return hyc::Math::Atan(t); }

        //static hyc::real Mod( hyc::real v, hyc::real mod ) {  }

        static hyc::real Pow( hyc::real t, hyc::real power )    { return hyc::Math::Pow(t, power); }
        static hyc::real Sqrt( hyc::real t )                    { return hyc::Math::Sqrt(t); }
        static hyc::f32  Sqrt_q( hyc::f32 number )              { return hyc::Math::Sqrt_q(number); }
        static hyc::real InvSqrt( hyc::real t )                 { return hyc::Math::InvSqrt(t); } // 1.0 / Sqrt; use in calculator vector normalize
        static hyc::f32  InvSqrt_q( hyc::f32 t )                { return hyc::Math::InvSqrt_q(t); }    // 1.0 / Sqrt; use q3 Sqrt algorithm

        /*---------------------------
        随机函数
        ---------------------------*/
        // random seed
        static void SRand( hyc::u32 seed )          { hyc::Math::SRand(seed); }
        // genarate a number in [0, MAX_INT]
        static hyc::u32  Rand()                     { return hyc::Math::Rand(); }
        // genarate a hyc::real in [0.0, 1.0]
        static hyc::real Rand_01()                  { return hyc::Math::Rand01(); }
        // genarate a hyc::real in [low, high]
        static hyc::real Rand( hyc::real low, hyc::real high )     { return hyc::Math::RandRange(low, high); }
    };

} // end namespace Hy
