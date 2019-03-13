//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hy UI
// author:       lipengxiang
// description:  
//

#ifndef __BP_COLORUTILS_H_20120104164409__
#define __BP_COLORUTILS_H_20120104164409__

namespace hyc
{
namespace geom
{

    //----------------------------------------------------------------------------------------------------
    //!	Color
    //----------------------------------------------------------------------------------------------------
    typedef u32 Color;
    
    //----------------------------------------------------------------------------------------------------
    //!	Colorf
    //----------------------------------------------------------------------------------------------------
    struct Colorf
    {
    	Colorf() : a(1.0), r(1.0), g(1.0), b(1.0)  {}
        Colorf( real a, real r, real g, real b ) : a(a), r(r), g(g), b(b)   {}
        Colorf( const Color& c );
        Colorf( const Colorf& c ) : a(c.a), r(c.r), g(c.g), b(c.b)    {}
        void operator=(Color c);
        bool operator==(const Colorf& c) const  { return r==c.r && g==c.g && b==c.b && a==c.a; }
        bool operator!=(const Colorf& c) const  { return !operator==(c); }
        Color   getValue() const;
        
        real a;
        real r;
        real g;
        real b;
    };
    
    //----------------------------------------------------------------------------------------------------
    //*	棰滆壊宸ュ叿
    //----------------------------------------------------------------------------------------------------
    class ColorUtils
    {
    public:
        
        //-----------------------------------------------
        static void Convert2Color( const Colorf& cf, OUT Color& c ) { SetA(c,cf.a*255); SetR(c,cf.r*255); SetG(c,cf.g*255); SetB(c,cf.b*255); }
        static void Convert2Colorf( const Color& c, OUT Colorf& cf ){ u8 t; GetA(c, t); cf.a = t/255.0; GetR(c, t); cf.r = t/255.0; GetG(c, t); cf.g = t/255.0; GetB(c, t); cf.b = t/255.0; }
        
        //-----------------------------------------------
        static Color  DodgeColor( const Color& color, const s16& dodgeValue );
        static Colorf DodgeColor( const Colorf& color, const s16& dodgeValue );
        static Color  InverseColor( const Color& color );
        static Colorf InverseColor( const Colorf& color );
 
        //-----------------------------------------------
        static void GetA( const Color& color, OUT u8& A )   { A = (color & 0xff000000) >> 6*4; }
        static void SetA( OUT Color& color, const u8& A )   { color = (color & 0x00ffffff) | (((u32)A) << 6*4); }
        static void GetA( const Colorf& color, OUT u8& A )  { A = color.a; }
        static void SetA( OUT Colorf& color, const u8& A )  { color.a = A; }

        //-----------------------------------------------
        static void GetR( const Color& color, OUT u8& R )   { R = (color & 0x00ff0000) >> 4*4; }
        static void SetR( OUT Color& color, const u8& R )   { color = (color & 0xff00ffff) | (((u32)R) << 4*4); }
        static void GetR( const Colorf& color, OUT u8& R )  { R = color.r; }
        static void SetR( OUT Colorf& color, const u8& R )  { color.r = R; }

        //-----------------------------------------------
        static void GetG( const Color& color, OUT u8& G )   { G = (color & 0x0000ff00) >> 2*4; }
        static void SetG( OUT Color& color, const u8& G )   { color = (color & 0xffff00ff) | (((u32)G) << 2*4); }
        static void GetG( const Colorf& color, OUT u8& G )  { G = color.g; }
        static void SetG( OUT Colorf& color, const u8& G )  { color.g = G; }

        //-----------------------------------------------
        static void GetB( const Color& color, OUT u8& B )   { B = (color & 0x000000ff); }
        static void SetB( OUT Color& color, const u8& B )   { color = (color & 0xffffff00) | (((u32)B)); }
        static void GetB( const Colorf& color, OUT u8& B )  { B = color.b; }
        static void SetB( OUT Colorf& color, const u8& B )  { color.b = B; }

        //-----------------------------------------------
        static const Colorf&    ColorBlack()        { static Colorf c(0xff000000); return c; }
        static const Colorf&    ColorDarkGray()     { static Colorf c(0xffa9a9a9); return c; }
        static const Colorf&    ColorLightGray()    { static Colorf c(0xffd3d3d3); return c; }
        static const Colorf&    ColorWhite()        { static Colorf c(0xffffffff); return c; }
        static const Colorf&    ColorGray()         { static Colorf c(0xff808080); return c; }
        static const Colorf&    ColorRed()          { static Colorf c(0xffff0000); return c; }
        static const Colorf&    ColorGreen()        { static Colorf c(0xff00ff00); return c; }
        static const Colorf&    ColorBlue()         { static Colorf c(0xff0000ff); return c; }
        static const Colorf&    ColorCyan()         { static Colorf c(0xff00ffff); return c; }  // 闈涢潚
        static const Colorf&    ColorYellow()       { static Colorf c(0xffffff00); return c; }
        static const Colorf&    ColorMagenta()      { static Colorf c(0xffff00ff); return c; }  // 鍝佺孩
        static const Colorf&    ColorOrange()       { static Colorf c(0xffffa500); return c; }
        static const Colorf&    ColorPurple()       { static Colorf c(0xff800080); return c; }  // 绱?        static const Colorf&    ColorBrown()        { static Colorf c(0xffa52a2a); return c; }
        static const Colorf&    ColorAlpha()        { static Colorf c(0x00000000); return c; }
    };


    //-----------------------------------------------
    inline Color ColorUtils::DodgeColor( const Color& color, const s16& dodgeValue )
    {
        u32 r = ((u32)color & 0x00ff0000) >> 4*4;
        u32 g = ((u32)color & 0x0000ff00) >> 2*4;
        u32 b = (u32)color & 0x000000ff;

        //r += (r>255/2) ? -deepValue : deepValue;
        //g += (g>255/2) ? -deepValue : deepValue;
        //b += (b>255/2) ? -deepValue : deepValue;

        r += dodgeValue;
        g += dodgeValue;
        b += dodgeValue;
        
        Color c = color;
        c >>= 6*4;
        c <<= 2*4;    c |= r;
        c <<= 2*4;    c |= g;
        c <<= 2*4;    c |= b;
        return c;
    }
    
    //-----------------------------------------------
    inline Colorf ColorUtils::DodgeColor( const Colorf& color, const s16& dodgeValue )
    {
        f32 t = dodgeValue / 255.0f;
        
        Colorf c(color);
        //c.r += (color.r>0.5) ? -t : t;
        //c.g += (color.g>0.5) ? -t : t;
        //c.b += (color.b>0.5) ? -t : t;
        c.r += t;
        c.g += t;
        c.b += t;
        
        return c;
    }
    //-----------------------------------------------
    inline Color ColorUtils::InverseColor( const Color& color )
    {
        u32 r = ((u32)color & 0x00ff0000) >> 4*4;
        u32 g = ((u32)color & 0x0000ff00) >> 2*4;
        u32 b = (u32)color & 0x000000ff;

        r = 255-r;
        g = 255-g;
        b = 255-b;

        Color c = color;
        c >>= 6*4;
        c <<= 2*4;    c |= r;
        c <<= 2*4;    c |= g;
        c <<= 2*4;    c |= b;
        return c;
    }
    //-----------------------------------------------
    inline Colorf ColorUtils::InverseColor( const Colorf& color )
    {
        Colorf c;
        c.r = 1.0-color.r;
        c.g = 1.0-color.g;
        c.b = 1.0-color.b;
        c.a = color.a;
        return c;
    }

    inline Colorf::Colorf(const Color& c)
    : a(0), r(0), g(0), b(0)
    {
        ColorUtils::Convert2Colorf(c, *this);
    }
    inline void Colorf::operator=(Color c)
    {
        ColorUtils::Convert2Colorf(c, *this);
    }
    inline Color Colorf::getValue() const
    {
        Color c;
        ColorUtils::Convert2Color(*this, c);
        return c;
    }

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_COLORUTILS_H_20120104164409__

