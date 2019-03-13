// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// bRAINpoint 3D Graphic Engine
//   

#ifndef __BP_CNSTRING_H_2010102413206__
#define __BP_CNSTRING_H_2010102413206__

#include "../container/array.h"

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	multi-byte char
    //----------------------------------------------------------------------------------------------------
    class HYC_API cString : public MemoryAllocObject
    {
    public:
        static const u32  npos;   // 不可能达到的长度.
    public:
        cString();
        cString(const cString& str);
        cString(const char* str);
        cString(const char* str, u32 size);
        ~cString();

        // override operator
    public:
        cString&    operator=(const cString& str);
        cString&    operator=(const char* str);
        bool        operator==(const cString& str) const;
        bool        operator==(const char* str) const;
        bool        operator!=(const cString& str) const;
        bool        operator!=(const char* str) const;
        cString     operator+(const cString& str) const;
        cString     operator+(const char& c) const;
        cString     operator+(const char* str) const;
        cString&    operator+=(const cString& str);
        cString&    operator+=(const char& c);
        cString&    operator+=(const char* str);
        cString&    operator+=(const wchar_t& c);
        cString&    operator+=(const wchar_t* str);
        char        operator[](u32 index) const;
        char&       operator[](u32 index);

        bool        operator<(const cString& str) const;
        bool        operator<(const char* str) const;
        bool        operator<=(const cString& str) const;
        bool        operator<=(const char* str) const;
        bool        operator>=(const cString& str) const;
        bool        operator>=(const char* str) const;
        bool        operator>(const cString& str) const;
        bool        operator>(const char* str) const;

        friend cString HYC_API operator+(const char& c, const cString& right);
        friend cString HYC_API operator+(const char* str, const cString& right);

        // stl method
    public:
        const char& at(u32 index) const;
        void        append(u32 count, char c);
        void        append(const char* pstr, u32 count);
        void        append(const cString& str);
        const char* c_str() const;
        u32         capacity() const;
        s32         compare( u32 off, u32 count, const char* pStr ) const;
        s32         compare( u32 off, u32 count, const cString& str ) const;
        void        clear();
        bool        empty() const;
        void        erase(u32 off, u32 count = 1);
        s32         find(char c, u32 off = 0) const;
        s32         find(const char* str, u32 off = 0) const;
        s32         find_last_of(char c, u32 off = npos) const;
        s32         find_last_of(const char* str, u32 off = npos) const;
        void        insert(u32 pos, u32 count, char c);
        void        insert(u32 pos, const cString& str);
        void        insert(u32 pos, const char* str);
        void        insert(u32 pos, const char* str, u32 count);
        void        resize(u32 newSize, char c);
        void        reserve(u32 reserveSize);
        u32         size() const;
        cString     substr(u32 off, u32 count) const;

        // extand method
    public:
        // 格式化字符串，可以不用在乎字符串本身的长度，format不能等于c_str()本身.
        // cString中的字符串需使用""; wString中的字符串需使用L"".
        // 支持的格式化:
        //      %c          - char
        //      %s          - char*; (遇0结束，不同于std遇\t\r\n也结束)
        //      %x or %X    - unsigned char
        //      %d or %i    - int                   (%2d; 两位右对齐)
        //      %hd or %hi  - short int             (%2hd; 两位右对齐)
        //      %ld or %li  - long int              (%2ld; 两位右对齐)
        //      %lld or %lli- long long             (%2lld; 两位右对齐)
        //      %u          - unsigned int          (%2u; 两位右对齐)
        //      %hu         - unsigned short int    (%2hu; 两位右对齐)
        //      %lu         - unsigned long int     (%2lu; 两位右对齐)
        //      %llu        - unsigned long long    (%2llu; 两位右对齐)
        //      %f          - float;                (%.3f; 小数点后保留3位, 精度及整数部分最大为两位数)
        //      %lf         - double                (%.3f; 小数点后保留3位, 精度及整数部分最大为两位数)
        //      %%          - %
        void        format(const char* format, ...);
        void        vformat(const char* format, va_list _ArgList);
        void        replace(u32 startPos, u32 endPos, char oldC, char newC);
        void        replace(u32 startPos, u32 endPos, char oldC, const cString& newStr);
        void        replace(u32 startPos, u32 endPos, const cString& oldStr, char newC);
        void        replace(u32 startPos, u32 endPos, const cString& oldStr, const cString& newStr);
        void        toLower();
        void        toUpper();
		void		split( char delimiter, OUT Array<cString>* arrStr ) const;	// 使用分隔符分隔字符串(结果不包含分隔符)
		void		split( const char* delimiter, OUT Array<cString>* arrStr ) const;
        bool        isBeginWith( const char* str ) const;
        
        // 清除字符串两边的指定符号.
        void        trim( char c = ' ' );

        // 填充;当字符长度小于指定长度时使用指定字符填充.
        void        fill(u32 minSize, char c)  { if (size() < minSize) resize(minSize, c); }
        
#ifdef _DEBUG
        const char* pDebugStr;
#endif // _DEBUG

    private:
        char*   _pBuf;
        u32     _size;
        u32     _capa;
    };


    //----------------------------------------------------------------------------------------------------
    //*	wide char
    //----------------------------------------------------------------------------------------------------
    class HYC_API wString : public MemoryAllocObject
    {
    public:
        static const u32  npos;   // 不可能达到的长度
    public:
        wString();
        wString(const wString& str);
        wString(const wchar_t* str);
        wString(const wchar_t* str, u32 size);

        ~wString();

        // override operator
    public:
        wString&    operator=(const wString& str);
        wString&    operator=(const wchar_t* str);
        bool        operator==(const wString& str) const;
        bool        operator==(const wchar_t* str) const;
        bool        operator!=(const wString& str) const;
        bool        operator!=(const wchar_t* str) const;
        wString     operator+(const wString& str) const;
        wString     operator+(const wchar_t& c) const;
        wString     operator+(const wchar_t* str) const;
        wString&    operator+=(const wString& str);
        wString&    operator+=(const char& c);
        wString&    operator+=(const char* str);
        wString&    operator+=(const wchar_t& c);
        wString&    operator+=(const wchar_t* str);
        wchar_t     operator[](u32 index) const;
        wchar_t&    operator[](u32 index);

        bool        operator<(const wString& str) const;
        bool        operator<(const wchar_t* str) const;
        bool        operator<=(const wString& str) const;
        bool        operator<=(const wchar_t* str) const;
        bool        operator>=(const wString& str) const;
        bool        operator>=(const wchar_t* str) const;
        bool        operator>(const wString& str) const;
        bool        operator>(const wchar_t* str) const;

        friend wString HYC_API operator+(const wchar_t& c, const wString& right);
        friend wString HYC_API operator+(const wchar_t* str, const wString& right);

        // stl method
    public:
        const wchar_t& at(u32 index) const;
        void        append(u32 count, wchar_t c);
        void        append(const wchar_t* pstr, u32 count);
        void        append(const wString& str);
        void        assign(u32 count, wchar_t c);
        const wchar_t* c_str() const;
        u32         capacity() const;
        s32         compare( u32 off, u32 count, const wchar_t* pStr ) const;
        s32         compare( u32 off, u32 count, const wString& Str ) const;
        void        clear();
        bool        empty() const;
        void        erase(u32 off, u32 count = 1);
        s32         find(wchar_t c, u32 off = 0) const;
        s32         find(const wchar_t* str, u32 off = 0) const;
        s32         find_last_of(wchar_t c, u32 off = npos) const;
        s32         find_last_of(const wchar_t* str, u32 off = npos) const;
        void        insert(u32 pos, u32 count, wchar_t c);
        void        insert(u32 pos, const wString& str);
        void        insert(u32 pos, const wchar_t* str);
        void        insert(u32 pos, const wchar_t* str, u32 count);
        void        resize(u32 newSize, wchar_t c);
        void        reserve(u32 reserveSize);
        u32         size() const;
        wString     substr(u32 off, u32 count) const;

        // extand method
    public:
        // (编译时需打开 U__STRICT_ANSI__)
        // 格式化字符串，可以不用在乎字符串本身的长度，format不能等于c_str()本身.
        // cString中的字符串需使用""; wString中的字符串需使用L"".
        void        format(const wchar_t* format, ...);
        void        vformat(const wchar_t* format, va_list _ArgList);
        void        replace(u32 startPos, u32 endPos, wchar_t oldC, wchar_t newC);
        void        replace(u32 startPos, u32 endPos, wchar_t oldC, const wString& newStr);
        void        replace(u32 startPos, u32 endPos, const wString& oldStr, wchar_t newC);
        void        replace(u32 startPos, u32 endPos, const wString& oldStr, const wString& newStr);
        void        toLower();
        void        toUpper();
		void		split( wchar_t delimiter, OUT Array<wString>* arrStr ) const;	// 使用分隔符分隔字符串(结果不包含分隔符)
		void		split( const wchar_t* delimiter, OUT Array<wString>* arrStr ) const;
        bool        isBeginWith( const wchar_t* str ) const;

        // 清除字符串两边的指定符号.
        void        trim( wchar_t c = L' ' );

        // 填充;当字符长度小于指定长度时使用指定字符填充.
        void        fill(u32 minSize, wchar_t c)  { if (size() < minSize) resize(minSize, c); }

#ifdef _DEBUG
        const wchar_t* pDebugStr;
#endif // _DEBUG

    private:
        wchar_t*   _pBuf;
        u32     _size;
        u32     _capa;
    };

} // end namespace hyc



#include "../utils/stringUtils.h"

namespace hyc
{
    extern cString HYC_API  operator+(const char& c, const cString& right);
    extern cString HYC_API  operator+(const char* str, const cString& right);
    extern bool    HYC_API  operator==(const char* str, const cString& right);
    extern bool    HYC_API  operator!=(const char* str, const cString& right);
    extern wString HYC_API  operator+(const wchar_t& c, const wString& right);
    extern wString HYC_API  operator+(const wchar_t* str, const wString& right);
    extern bool    HYC_API  operator==(const wchar_t* str, const wString& right);
    extern bool    HYC_API  operator!=(const wchar_t* str, const wString& right);
}


//----------------------------------------------------------------------------------------------------
//*	define tchar type and name same char operator
//----------------------------------------------------------------------------------------------------
 
#if defined(_UNICODE) || defined(UNICODE)

#   define  tcscpy     wcscpy
#   define  tcsncpy    wcsncpy
#   define  tcscmp     wcscmp
#   define  tcsncmp    wcsncmp
#   define  tcslen     wcslen
#   define  tcsnlen    wcsnlen
#   define  tcschr     wcschr
#   define  tcscat     wcscat
#   define  tcsncat    wcsncat
#   define  tcsset     wcsnset
#   define  tsprintf   swprintf

#   define  tatof      _wtof
#   define  tatoi      _wtoi
#   define  tatol      _wtol

#else

#   define  tcscpy     strcpy
#   define  tcsncpy    strncpy
#   define  tcscmp     strcmp
#   define  tcsncmp    strncmp
#   define  tcslen     strlen
#   define  tcsnlen    strnlen
#   define  tcschr     strchr
#   define  tcscat     strcat
#   define  tcsncat    strncat
#   define  tcsset(pStr,num,val)     memset(pStr,val,num)
#   define  tsprintf   sprintf

#   define  tatof      atof
#   define  tatoi      atoi
#   define  tatol      atol

#endif // UNICODE

/**
 * override memcpy for ARM cpu Memory Alignment
 */
#if defined(CN_PLATFORM_MOBILE)
#if !defined(__i386__) && defined(__arm__)
extern "C" void * memcpy(void * dst, const void * src, size_t len);
extern "C" wchar_t * wmemcpy(wchar_t * dst, const wchar_t * src, size_t len);
#endif
#endif

#endif // end of define __BP_CNSTRING_H_2010102413206__
