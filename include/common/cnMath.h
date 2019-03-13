//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_CNMATH_H_20101220135747__
#define __BP_CNMATH_H_20101220135747__


namespace hyc
{
    //--------------------------------------------------
    //! basic math algorithm
    //! 1.wrap c-runtime math function
    //! 2.extend some function     
    //--------------------------------------------------
    class HYC_API Math
    {
    public:
        /*---------------------------
          const.
        ---------------------------*/
        static const real   PI;
        static const real   PI_2;       //! PI * 2
        static const real   PI_HALF;    //! PI / 2
        static       real   TOLERANCE;  //! use to compare
        static const real   TOLERANCE_DEFAULT;  //! use to compare

        /*---------------------------
          angle function.
        ---------------------------*/
        static real Radian2Degree( const real& radian );    //! (radian)->(degree)
        static real Degree2Radian( const real& degree );    //! (degree)->(radian)
        static real MakeIn2Pi( const real& radianValue );   //! Make sure value in [0~2pi]
        static real MakeInPi( const real& radianValue );    //! Make sure vlaue in [-pi~pi]

        /*---------------------------
          compare function.
        ---------------------------*/
        static bool Equals( const real& f1, const real& f2, const real& tolerance = TOLERANCE );    //! ==
        static bool LessThan( const real& f1, const real& f2, const real& tolerance = TOLERANCE );  //!less than <
        static bool LessEquals( const real& f1, const real& f2, const real& tolerance = TOLERANCE );    //!less or equals <=
        static bool MoreThan( const real& f1, const real& f2, const real& tolerance = TOLERANCE );      //!more than >
        static bool MoreEquals( const real& f1, const real& f2, const real& tolerance = TOLERANCE );    //!more or equals >=

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   pragma warning(disable:4018)   //warning C4018: '>' : signed/unsigned mismatch
#endif
        template<class _T2>
        static const _T2&   Max( const _T2& a, const _T2& b ) { return (((a) > (b)) ? (a) : (b)); }
        template<class _T2, class _T3>
        static _T2          Max( const _T2& a, const _T3& b ) { return (((a) > (b)) ? (a) : (b)); }
        template<class _T2>
        static const _T2&   Min( const _T2& a, const _T2& b ) { return (((a) < (b)) ? (a) : (b)); }
        template<class _T2, class _T3>
        static _T2          Min( const _T2& a, const _T3& b ) { return (((a) < (b)) ? (a) : (b)); }
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   pragma warning(default:4018)
#endif

        /*---------------------------
          triangle function.
        ---------------------------*/
        static real Abs( real t )   { return (t > 0) ? t : (-t); }
        static real Ceil( real t )  { return ::ceil(t); }
        static real Floor( real t ) { return ::floor(t); }
        static real Round( real t ) { return t >= 0.0 ? ::floor(t+0.5) : ::ceil(t-0.5); }
        static integer  Sign( real t )  { return (t == 0) ? 0 : ((t > 0) ? 1 : -1); }

        static real Sin( real t )   { return ::sin(t); }
        static real Asin( real t )  {  if (t<=-1.0) { return -PI_HALF; } if (t>=1.0) { return PI_HALF; } return ::asin(t); }
        static real Cos( real t )   { return ::cos(t); }
        static real Acos( real t )  { if (t<=-1.0) { return PI; } if (t>=1.0) { return 0; } return ::acos(t); }
        static real Tan( real t )   { return ::tan(t); }
        static real Atan( real t )  { return ::atan(t); }

        //static real Mod( real v, real mod ) {  }

        template<class _T2>
        static real Pow( real t, _T2 power )    { return ::pow(t, power); }
        static real Sqrt( real t )              { return ::sqrt(t); }
        static f32  Sqrt_q( f32 number );       //! Sqrt, use Quake 3 Sqrt algorithm, unexactitude
        static real InvSqrt( real t )           { return (real)1.0 / Sqrt(t); } //! 1.0 / Sqrt; use in calculator vector normalize
        static f32  InvSqrt_q( f32 t );         //! 1.0 / Sqrt; use q3 Sqrt algorithm
        static bool IsPowerOf2( u32 v )         { return v > 0 && (v & (v-1)) == 0; }
        static u32  GetBestPowerOf2( u32 v )    { u32 p = 1; while (p < v) { p <<= 1; } return p; }
        
        /*---------------------------
          swap function.
        ---------------------------*/
        template<class T>
        static void Swap( OUT T& r1, OUT T& r2 )        { T t = r1; r1 = r2; r2 = t; }
        static void Swap( OUT s8& r1, OUT s8& r2 )      { r1 ^= r2; r2 ^= r1; r1 ^= r2; }
        static void Swap( OUT u8& r1, OUT u8& r2 )      { r1 ^= r2; r2 ^= r1; r1 ^= r2; }
        static void Swap( OUT s16& r1, OUT s16& r2 )    { r1 ^= r2; r2 ^= r1; r1 ^= r2; }
        static void Swap( OUT u16& r1, OUT u16& r2 )    { r1 ^= r2; r2 ^= r1; r1 ^= r2; }
        static void Swap( OUT s32& r1, OUT s32& r2 )    { r1 ^= r2; r2 ^= r1; r1 ^= r2; }
        static void Swap( OUT u32& r1, OUT u32& r2 )    { r1 ^= r2; r2 ^= r1; r1 ^= r2; }
        static void Swap( OUT s64& r1, OUT s64& r2 )    { r1 ^= r2; r2 ^= r1; r1 ^= r2; }
        static void Swap( OUT u64& r1, OUT u64& r2 )    { r1 ^= r2; r2 ^= r1; r1 ^= r2; }


        /*---------------------------
          rand function.
        ---------------------------*/
        //! random seed
        static void     SRand( u32 seed )               { ::srand(seed); ::rand(); }
        //! genarate a number in [0, MAX_INT]
        static uinteger Rand()                          { return ::rand(); }
        //! genarate a real in [0.0, 1.0]
        static real     Rand01()                        { real r = ::rand() / (real)RAND_MAX; if (r > 1.0) r = 1.0; return r; }
        //! genarate a real in [low, high]
        static real     RandRange( real low, real high )     { return ((high-low) * Rand01() + low); }
        //! select a event from multiple random events.
        //! @param ratios   the event probability array, each must less equal 1.0 and total value mustn't less equal 1.0.
        //! @return the index of selected event, no selected event return -1;
        static s32      RandMulti( const real ratios[], const u32& count );

    };

    //-----------------------------------------------
    inline s32  Math::RandMulti( const real ratios[], const u32& count )
    {
        assert(ratios);
        if (count == 0) return -1;
        real total = 0;
        for (u32 i = 0; i < count; i++)
        {
            assert(ratios[i] <= 1.0);
            total += ratios[i];
        }
        total = Rand01() * total;

        real v = 0;
        for (u32 i = 0; i < count; i++)
        {
            v += ratios[i];
            if (total <= v)
                return i;
        }

        return -1;                
    }

} // end namespace hyc

#endif // end of define __BP_CNMATH_H_20101220135747__
