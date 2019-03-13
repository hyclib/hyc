//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  the utility of string
//

#ifndef __BP_STRINGUTILS_H_20101206134234__
#define __BP_STRINGUTILS_H_20101206134234__


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	the utility of string
    //  系统默认 local=zh_CN; 当使用其他国家语言时,可能需要先调用 setlocale(LC_ALL, "xxx").
    //----------------------------------------------------------------------------------------------------
    class HYC_API StringUtils
    {
    public:
        static bool      SetLocale( const char* countryCode = "zh_CN" );
        
    public:
        /*---------------------------
          get the number of elem / get the byte size of str
        ---------------------------*/
        static uinteger  GetLength( const char* str );
        static uinteger  GetLength( const wchar_t* str );
        static uinteger  GetByteSize( const char* str );
        static uinteger  GetByteSize( const wchar_t* str );

        /*---------------------------
        !  judgement the string whether is null or empty
        ---------------------------*/
        static bool IsEmpty( const char* str );
        static bool IsEmpty( const wchar_t* str );

        /*---------------------------
        !  convert multibyte characters to a corresponding sequence of wide characters
        !  \param cMbs: The number of multibyte characters to convert
        !  \param cVcs: The length of the corresponding sequence of wide characters
        ---------------------------*/
        static bool Narrow2wide( const char* mbstr, uinteger cSizeMbs, OUT wchar_t* vcstr, OUT uinteger& cSizeVcs );
        static bool Narrow2wide( const char* mbstr, uinteger cSizeMbs, OUT wString& vcstr );
        static bool Narrow2wide( const cString& mbstr, OUT wString& vcstr );

        /*---------------------------
        !  convert a corresponding sequence of wide characters to multibyte characters
        !  \param cVcs: The number of the corresponding sequence of wide characters to convert
        !  \param cMbs: The length of multibyte characters
        ---------------------------*/
        static bool Wide2narrow( const wchar_t* vcstr, uinteger cSizeVcs, OUT char* mbstr, OUT uinteger& cSizeMbs );
        static bool Wide2narrow( const wchar_t* vcstr, uinteger cSizeVcs, OUT cString& mbstr );
        static bool Wide2narrow( const wString& vcstr, OUT cString& mbstr );

        /*---------------------------
        !  convert string to number
        ---------------------------*/
        
        static integer  StrToInteger( const char* pszStr );
        static uinteger StrToUInteger( const char* pszStr );
        static integer  StrToInteger( const wchar_t* pszStr );
        static uinteger StrToUInteger( const wchar_t* pszStr );
        
        static s32  StrToS32( const char* pszStr );
        static s64  StrToS64( const char* pszStr );
        static u32  StrToU32( const char* pszStr );
        static u64  StrToU64( const char* pszStr );
        static f32  StrToF32( const char* pszStr );
        static f64  StrToF64( const char* pszStr );
        static s32  StrToS32( const wchar_t* pszStr );
        static s64  StrToS64( const wchar_t* pszStr );
        static u32  StrToU32( const wchar_t* pszStr );
        static u64  StrToU64( const wchar_t* pszStr );
        static f32  StrToF32( const wchar_t* pszStr );
        static f64  StrToF64( const wchar_t* pszStr );

        /*---------------------------
        !  convert string to number; return the string of str
        ---------------------------*/
        static const char*     IntegerToStr( integer value, OUT cString& str );
        static const wchar_t*  IntegerToStr( integer value, OUT wString& str );
        static const char*     UIntegerToStr( uinteger value, OUT cString& str );
        static const wchar_t*  UIntegerToStr( uinteger value, OUT wString& str );
        
        static const char*  S32ToStr( s32 value, OUT cString& str );
        static const char*  S64ToStr( s64 value, OUT cString& str );
        static const char*  U32ToStr( u32 value, OUT cString& str );
        static const char*  U64ToStr( u64 value, OUT cString& str );
        static const char*  F32ToStr( f32 value, OUT cString& str, u32 precision = 6 ); // 最大精度为6.
        static const char*  F64ToStr( f64 value, OUT cString& str, u32 precision = 6 ); // 最大精度为14.
        static const wchar_t*  S32ToStr( s32 value, OUT wString& str );
        static const wchar_t*  S64ToStr( s64 value, OUT wString& str );
        static const wchar_t*  U32ToStr( u32 value, OUT wString& str );
        static const wchar_t*  U64ToStr( u64 value, OUT wString& str );
        static const wchar_t*  F32ToStr( f32 value, OUT wString& str, u32 precision = 6 ); // 最大精度为6.
        static const wchar_t*  F64ToStr( f64 value, OUT wString& str, u32 precision = 6 ); // 最大精度为14.

        //! 将src转换成dest并返回结果.
        inline static const char*     StrToStr( const wchar_t* src, OUT cString& dest )   { Wide2narrow(src, dest); return dest.c_str(); }
        inline static const char*     StrToStr( const char* src,    OUT cString& dest )   { dest = src; return dest.c_str(); }
        inline static const wchar_t*  StrToStr( const wchar_t* src, OUT wString& dest )   { dest = src; return dest.c_str(); }
        inline static const wchar_t*  StrToStr( const char* src,    OUT wString& dest )   { Narrow2wide(src, dest); return dest.c_str(); }
        inline static const char*     StrToStr( const wString& src, OUT cString& dest )   { Wide2narrow(src, dest); return dest.c_str(); }
        inline static const char*     StrToStr( const cString& src, OUT cString& dest )   { dest = src; return dest.c_str(); }
        inline static const wchar_t*  StrToStr( const wString& src, OUT wString& dest )   { dest = src; return dest.c_str(); }
        inline static const wchar_t*  StrToStr( const cString& src, OUT wString& dest )   { Narrow2wide(src, dest); return dest.c_str(); }

        //! escape;数据库字符串转义,存储至temp中并返回.
        static const tchar* Escape( const tchar* str, OUT String& temp );
    };


} // end namespace hyc

#endif // end of define __BP_STRINGUTILS_H_20101206134234__