//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  编码工具; Unicode转换格式(UTF, UCS Transformation Format)
//

#ifndef __BP_ENCODINGUTILS_H_20101109131547__
#define __BP_ENCODINGUTILS_H_20101109131547__

#include "../container/byteStream.h"

namespace hyc
{
	//----------------------------------------------------------------------------------------------------
	//! Define UTF type
	//----------------------------------------------------------------------------------------------------
    typedef u8      utf8;   //! utf-8
    typedef u16     utf16;  //! utf-16
    typedef u32     utf32;  //! utf-32


    //----------------------------------------------------------------------------------------------------
    //!	convert between utf-8, utf-16 and utf-32
    //----------------------------------------------------------------------------------------------------
    class HYC_API EncodingUtils
    {
    public:
        //! 转换标记
        typedef enum {
            CF_strict = 0,  //! 严格转换
            CF_lenient      //! 宽松转换
        } ConversionFlag;

    public:

        /*---------------------------
          utf编码
        ---------------------------*/

        //! 判断是否是合法的utf-8
        static bool IsLegalUTF8( const utf8* pStream, u32 length );
        static bool IsLegalUTF8File( const utf8* pStream, u32 length );

        //! string 与 UTF8 间的转换; 返回转换的字符长度.
        static u32  StrToUTF8( const char* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength );
        static u32  StrToUTF8( const wchar_t* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength );
        static u32  UTF8ToStr( const utf8* pSrcStream, u32 srcLength, OUT char* pDestStream, u32 destLength );
        static u32  UTF8ToStr( const utf8* pSrcStream, u32 srcLength, OUT wchar_t* pDestStream, u32 destLength );

        //! unicode间转换; 返回转换的字符长度.
        static u32 UTF8toUTF16( const utf8* pSrcStream, u32 srcLength, OUT utf16* pDestStream, u32 destLength, ConversionFlag flags = CF_strict );
        static u32 UTF8toUTF32( const utf8* pSrcStream, u32 srcLength, OUT utf32* pDestStream, u32 destLength, ConversionFlag flags = CF_strict );
        static u32 UTF16toUTF8( const utf16* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength, ConversionFlag flags = CF_strict );
        static u32 UTF16toUTF32( const utf16* pSrcStream, u32 srcLength, OUT utf32* pDestStream, u32 destLength, ConversionFlag flags = CF_strict );
        static u32 UTF32toUTF8( const utf32* pSrcStream, u32 srcLength, OUT utf8* pDestStream, u32 destLength, ConversionFlag flags = CF_strict );
        static u32 UTF32toUTF16( const utf32* pSrcStream, u32 srcLength, OUT utf16* pDestStream, u32 destLength, ConversionFlag flags = CF_strict );

        //! utf16 <-> \uxxxx 格式
        //! src 与 dest不能相等.
        static bool UTF16ToUStr( const utf16* pSrcStream, u32 srcLength, OUT String& uStr );
        static void UStrToStr( const char* pStr, u32 srcLength, OUT String& strDest );
        static void UStrToStr( const wchar_t* pStr, u32 srcLength, OUT String& strDest );
        static void UStrToStr( const String& str, OUT String& strDest )     { UStrToStr(str.c_str(), str.size(), strDest); }

        /*---------------------------
          convert Byte to hex
        ---------------------------*/
        static void ByteToHex( const Byte* pVale, u32 cLenData, OUT cString& str );
        static void ByteToHex( const Byte* pVale, u32 cLenData, OUT wString& str );
        static void ByteToHex( const Byte* pVale, u32 cLenData, OUT char* pStr, u32 cLenStr );          //! 要全部转换需: cLenStr == cLenData*2
        static void ByteToHex( const Byte* pVale, u32 cLenData, OUT wchar_t* pStr, u32 cLenStr );       //! 要全部转换需: cLenStr == cLenData*2
        static void HexToByte( const char* pHexStr, u32 cLenStr, OUT Byte* pValue, u32 cLenData );      //! 要全部转换需: cLenStr == cLenData*2
        static void HexToByte( const wchar_t* pHexStr, u32 cLenStr, OUT Byte* pValue, u32 cLenData );   //! 要全部转换需: cLenStr == cLenData*2

        /*---------------------------
        !  base64; 
        !  base64编码用于将二进制流转换成字符流用于http的传输
        !  \param needReturn 每76个字符就需要加上一个回车换行
        ---------------------------*/
        static bool IsBase64( const char& c );
        static void Base64_Encode( const Byte* pData, u32 cLenData, OUT cString& strBase64, bool needReturn = true );
        static void Base64_Decode( const char* pStrBase64, u32 cLenStr, OUT ByteStream& stream );

        /*---------------------------
        !  url encode
        ---------------------------*/
        static void Url_Encode( const char* pStream, u32 cLenData, OUT cString& strUrlEncode );
        static void Url_Decode( const char* pUrlEncode, u32 cLenData, OUT cString& stream );
    };

} // end namespace hyc

#endif // end of define __BP_ENCODINGUTILS_H_20101109131547__
