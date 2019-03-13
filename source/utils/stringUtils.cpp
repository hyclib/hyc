//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#include "../../include/utils/stringUtils.h"
#include <string>
#include <sstream>

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma warning(disable: 4996)
#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS

//----------------------------------------------------------------------------------------------------
//*	windows骞冲彴
//----------------------------------------------------------------------------------------------------
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../_win32/stringUtils.hpp"

//----------------------------------------------------------------------------------------------------
//*	鍏朵粬骞冲彴
//----------------------------------------------------------------------------------------------------
#else
#include "../_linux/stringUtils.hpp"
#endif

//----------------------------------------------------------------------------------------------------
//*	鍚勫钩鍙扮殑鍏叡瀹炵幇
//----------------------------------------------------------------------------------------------------

namespace hyc {

    bool StringUtils::SetLocale(const char* countryCode)
    {
        return NULL != setlocale(LC_ALL, countryCode);
    }
    

    //-----------------------------------------------
    bool StringUtils::Narrow2wide( const char* mbstr,uinteger cSizeMbs, OUT wString& vcstr )
    {
        // check parameters
        if ( NULL == mbstr || !cSizeMbs )
        {
            return false;
        }

        wchar_t* pBuf = (wchar_t*)MemoryAlloc::Malloc(sizeof(wchar_t)*(cSizeMbs+1)); // new wchar_t[cSizeMbs+1];
        
        bool r = Narrow2wide(mbstr, cSizeMbs, pBuf, cSizeMbs);
        
        if (r)
        {
            pBuf[cSizeMbs] = 0;
            vcstr = pBuf;
        }

        MemoryAlloc::Free(pBuf);  //SAFE_DELETE(pBuf);
        return r;
    }

    //-----------------------------------------------
    bool StringUtils::Narrow2wide( const cString& mbstr, OUT wString& vcstr )
    {
        return Narrow2wide( mbstr.c_str(), mbstr.size(), vcstr );    
    }

    //-----------------------------------------------
    bool StringUtils::Wide2narrow( const wchar_t* vcstr, uinteger cSizeVcs, OUT cString& mbstr )
    {
        // check parameters
        if ( NULL == vcstr || !cSizeVcs )
        {
            return false;
        }

        uinteger len = cSizeVcs*sizeof(wchar_t);
        char* pBuf = (char*)MemoryAlloc::Malloc(len+1);
        bool r = Wide2narrow(vcstr, cSizeVcs, pBuf, len);

        if (r)
        {
            pBuf[len] = 0;
            mbstr = pBuf;
        }

        MemoryAlloc::Free(pBuf);
        return r;
    }

    //-----------------------------------------------
    bool StringUtils::Wide2narrow( const wString& vcstr, OUT cString& mbstr )
    {
        return Wide2narrow( vcstr.c_str(), vcstr.size(), mbstr );
    }

    //-----------------------------------------------
    bool StringUtils::IsEmpty( const char* str )
    {
        return NULL == str || 0 == strlen(str);
    }

    //-----------------------------------------------
    bool StringUtils::IsEmpty( const wchar_t* str )
    {
        return NULL == str || 0 == wcslen(str);
    }

    //-----------------------------------------------
    uinteger StringUtils::GetLength( const char* str )
    {
        return str ? strlen(str) : 0;
    }

    //-----------------------------------------------
    uinteger StringUtils::GetLength( const wchar_t* str )
    {
        return str ? wcslen(str) : 0;
    }

    //-----------------------------------------------
    uinteger StringUtils::GetByteSize( const char* str )
    {
        return GetLength(str);
    }

    //-----------------------------------------------
    uinteger StringUtils::GetByteSize( const wchar_t* str )
    {
        return GetLength(str)*sizeof(wchar_t);
    }

    //-----------------------------------------------
    s32 StringUtils::StrToS32( const char* pszStr )
    {
        return atoi( pszStr );
    }

    //-----------------------------------------------
    s32 StringUtils::StrToS32( const wchar_t* pszStr )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return _wtoi(pszStr);
#else
        s32 value;
        swscanf(pszStr, L"%d", &value);
        return value;
#endif
    }
    //-----------------------------------------------
    s64 StringUtils::StrToS64( const char* pszStr )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return _atoi64( pszStr );
#else
        s64 value;

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sscanf(pszStr, "%lld", &value);
#else
        sscanf(pszStr, "%lld", &value);
#endif
        return value;
#endif
    }

    //-----------------------------------------------
    s64 StringUtils::StrToS64( const wchar_t* pszStr )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return _wtoi64( pszStr );
#else
        s64 value;
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swscanf(pszStr, L"%lld", &value);
#else
        swscanf(pszStr, L"%ld", &value);
#endif
        return value;
#endif
    }
    //-----------------------------------------------
    f64 StringUtils::StrToF64( const char* pszStr )
    {
        return atof( pszStr );
    }

    //-----------------------------------------------
    f64 StringUtils::StrToF64( const wchar_t* pszStr )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return _wtof(pszStr);
#else
        f64 value;
        swscanf(pszStr, L"%lf", &value);
        return value;
#endif
    }

    //-----------------------------------------------
    f32 StringUtils::StrToF32( const char* pszStr )
    {
        return (f32)atof( pszStr );
    }

    //-----------------------------------------------
    f32 StringUtils::StrToF32( const wchar_t* pszStr )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return (f32)_wtof(pszStr);
#else
        f32 value;
        swscanf(pszStr, L"%f", &value);
        return value;
#endif
    }

    //----------------------------------------
    hyc::u32 StringUtils::StrToU32( const char* pszStr )
    {
        u32 value;
        sscanf(pszStr, "%u", &value);
        return value;
//         u32 value;
//         std::stringstream ss(pszStr);
//         ss >> value;
//         return value;
    }

    //----------------------------------------
    hyc::u64 StringUtils::StrToU64( const char* pszStr )
    {
        u64 value;

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sscanf(pszStr, "%llu", &value);
#else
        sscanf(pszStr, "%llu", &value);
#endif
        return value;
    }

    //----------------------------------------
    hyc::u32 StringUtils::StrToU32( const wchar_t* pszStr )
    {
        u32 value;
        swscanf(pszStr, L"%u", &value);
        return value;
    }

    //----------------------------------------
    hyc::u64 StringUtils::StrToU64( const wchar_t* pszStr )
    {
        u64 value;

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swscanf(pszStr, L"%llu", &value);
#else
        swscanf(pszStr, L"%lu", &value);
#endif
        return value;
    }
    
    //----------------------------------------
    integer StringUtils::StrToInteger( const wchar_t* pszStr )
    {
        integer value;
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swscanf(pszStr, L"%ld", &value);
#else
        swscanf(pszStr, L"%lld", &value);
#endif
        return value;
    }
    
    //----------------------------------------
    integer StringUtils::StrToInteger( const char* pszStr )
    {
        integer value;
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sscanf(pszStr, "%ld", &value);
#else
        sscanf(pszStr, "%lld", &value);
#endif
        return value;
    }
    
    
    //----------------------------------------
    uinteger StringUtils::StrToUInteger( const wchar_t* pszStr )
    {
        uinteger value;
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swscanf(pszStr, L"%lu", &value);
#else
        swscanf(pszStr, L"%llu", &value);
#endif
        return value;
    }
    
    //----------------------------------------
    uinteger StringUtils::StrToUInteger( const char* pszStr )
    {
        uinteger value;
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sscanf(pszStr, "%lu", &value);
#else
        sscanf(pszStr, "%llu", &value);
#endif
        return value;
    }

    
    //-----------------------------------------------
    const char* StringUtils::IntegerToStr( integer value, OUT cString& str )
    {
        char buf[24] = {0};
        
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sprintf(buf, "%ld", value);
#else
        sprintf(buf, "%lld", value);
#endif
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const wchar_t* StringUtils::IntegerToStr( integer value, OUT wString& str )
    {
        wchar_t buf[24] = {0};
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%ld", value);
#else
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%lld", value);
#endif
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const char* StringUtils::UIntegerToStr( uinteger value, OUT cString& str )
    {
        char buf[24] = {0};
        
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sprintf(buf, "%lu", value);
#else
        sprintf(buf, "%llu", value);
#endif
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const wchar_t* StringUtils::UIntegerToStr( uinteger value, OUT wString& str )
    {
        wchar_t buf[24] = {0};
        
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%lu", value);
#else
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%llu", value);
#endif
        str = buf;
        return str.c_str();
    }

    //-----------------------------------------------
    const char* StringUtils::S32ToStr( s32 value, OUT cString& str )
    {
        char buf[12] = {0};
        sprintf(buf, "%d", value);
        str = buf;
        return str.c_str();
    }

    //-----------------------------------------------
    const wchar_t* StringUtils::S32ToStr( s32 value, OUT wString& str )
    {
        wchar_t buf[12] = {0};
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%d", value);
        str = buf;
        return str.c_str();
    }
    //-----------------------------------------------
    const char* StringUtils::U32ToStr( u32 value, OUT cString& str )
    {
        char buf[12] = {0};
        sprintf(buf, "%u", value);
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const wchar_t* StringUtils::U32ToStr( u32 value, OUT wString& str )
    {
        wchar_t buf[12] = {0};
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%u", value);
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const char* StringUtils::S64ToStr( s64 value, OUT cString& str )
    {
        char buf[25] = {0};
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sprintf(buf, "%lld", value);
#else
        sprintf(buf, "%lld", value);
#endif
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const wchar_t* StringUtils::S64ToStr( s64 value, OUT wString& str )
    {
        wchar_t buf[25] = {0};

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%lld", value);
#else
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%ld", value);
#endif
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const char* StringUtils::U64ToStr( u64 value, OUT cString& str )
    {
        char buf[25] = {0};

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        sprintf(buf, "%llu", value);
#else
        sprintf(buf, "%llu", value);
#endif
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const wchar_t* StringUtils::U64ToStr( u64 value, OUT wString& str )
    {
        wchar_t buf[25] = {0};
#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%llu", value);
#else
        swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%lu", value);
#endif
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const char* StringUtils::F32ToStr( f32 value, OUT cString& str, u32 precision )
    {
        char buf[50] = {0};
        if (precision > 6)  precision = 6;
        if (6 == precision)
            sprintf(buf, "%f", value);
        else
        {
            cString s;
            StringUtils::U32ToStr(precision, s);
            s = "%." + s + "f";
            sprintf(buf, s.c_str(), value);
        }
        str = buf;
        return str.c_str();
    }
    
    //-----------------------------------------------
    const wchar_t* StringUtils::F32ToStr( f32 value, OUT wString& str, u32 precision )
    {
        wchar_t buf[50] = {0};
        if (precision > 6)  precision = 6;
        if (6 == precision)
            swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%f", value);
        else
        {
            wString s;
            StringUtils::U32ToStr(precision, s);
            s = L"%." + s + L"f";
            swprintf(buf, sizeof(buf)/sizeof(buf[0]), s.c_str(), value);
        }
        str = buf;
        return str.c_str();
    }

    //-----------------------------------------------
    const char* StringUtils::F64ToStr( f64 value, OUT cString& str, u32 precision )
    {
        char buf[350] = {0};
        if (precision > 14)  precision = 14;
        if (6 == precision)
            sprintf(buf, "%lf", value);
        else
        {
            cString s;
            StringUtils::U32ToStr(precision, s);
            s = "%." + s + "lf";
            sprintf(buf, s.c_str(), value);
        }
        str = buf;
        return str.c_str();
    }

    //-----------------------------------------------
    const wchar_t* StringUtils::F64ToStr( f64 value, OUT wString& str, u32 precision )
    {
        //std::wstringstream ss;
        //ss << value;
        //str = ss.str().c_str();
        //return str.c_str();
        wchar_t buf[350] = {0};
        if (precision > 14)  precision = 14;
        if (6 == precision)
            swprintf(buf, sizeof(buf)/sizeof(buf[0]), L"%lf", value);
        else
        {
            wString s;
            StringUtils::U32ToStr(precision, s);
            s = L"%." + s + L"lf";
            swprintf(buf, sizeof(buf)/sizeof(buf[0]), s.c_str(), value);
        }
        str = buf;
        return str.c_str();
    }

    //-----------------------------------------------
    const tchar* StringUtils::Escape( const tchar* str, OUT String& temp )
    {
        temp = str;
        temp.replace(0, temp.size(), _T('\''), _T("''"));
        return temp.c_str();
// 
//         static const tchar sets[] = { _T('\'') };
//         static const u32 count = 1;
//         String nStr = _T("\\ ");
// 
//         temp = str;
//         for (u32 i = 0; i < count; i++)
//         {
//             nStr[1] = sets[i];
//             temp.replace(0, temp.size(), sets[i], nStr);
//         }
//         
// //         // '\'.
// //         temp.replace(0, temp.size(), _T('\\'), _T("\\\\"));
// 
//         return temp.c_str();
    }

} // end namespace hyc
