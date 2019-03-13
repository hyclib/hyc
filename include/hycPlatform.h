// Copyright(c) 1999-2010 lipengxiang. All rights reserved.

#ifndef __BP_HYCPLATFORMa_H_2010102403055__
#define __BP_HYCPLATFORMa_H_2010102403055__

#if defined(__WIN32__) || defined(_WIN32)
#	pragma warning(disable: 4819)
#	pragma warning(disable: 4251)
#	pragma warning(disable: 4244)
#	pragma warning(disable: 4273)
#endif

//-----------------------------------------------
#include "hycConfig.h"
#include "hycException.h"
#include "_platform.h"

#ifdef CN_PLATFORM_MOBILE
#ifdef HYC_COMPILE_MODULE_NET_ASYNC
#undef HYC_COMPILE_MODULE_NET_ASYNC
#endif
#endif
 
/*
 * C and C++ runtime
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <wchar.h>


//----------------------------------------------------------------------------------------------------
//*	header file
//----------------------------------------------------------------------------------------------------

#if CN_PLATFORM != CN_PLATFORM_WINDOWS
#	include <stdint.h>
#endif // CN_PLATFORM != CN_PLATFORM_WINDOWS


//----------------------------------------------------------------------------------------------------
//*	export symbol
//----------------------------------------------------------------------------------------------------

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   ifdef COMMON_EXPORTS
#       define HYC_API __declspec(dllexport)
#   elif  defined(_LIB) && defined(HYC_COMPILE_STATIC_LIB)
#       define HYC_API
#   else
#       define HYC_API __declspec(dllimport)
#   endif // COMMON_EXPORTS
#else
#   define  HYC_API
#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS


//----------------------------------------------------------------------------------------------------
//! endian & architecture.
//----------------------------------------------------------------------------------------------------
extern "C" HYC_API bool _CN_IS_BIG_ENDIAN();
extern "C" HYC_API bool _CN_IS_ARCHITECTURE_64();

#define CN_IS_BIG_ENDIAN        _CN_IS_BIG_ENDIAN()
#define CN_IS_ARCHITECTURE_64   _CN_IS_ARCHITECTURE_64()

//----------------------------------------------------------------------------------------------------
//! char* use utf8 encoding.
//! (wchar_t* default use unicode encoding)
//----------------------------------------------------------------------------------------------------
extern "C" HYC_API bool _CN_IS_UTF8_CSTRING();

#define CN_IS_UTF8_CSTRING      _CN_IS_UTF8_CSTRING()

//----------------------------------------------------------------------------------------------------
//*	Necessary Macro
//----------------------------------------------------------------------------------------------------

//! OUT parameter
#ifndef OUT
#   define OUT
#endif // OUT
#ifndef INOUT
#   define INOUT
#endif // INOUT

//! NULL
#ifndef NULL
#   define NULL 0
#endif // NULL

//! DEBUG
#ifdef DEBUG
#   ifndef _DEBUG
#       define _DEBUG
#   endif
#endif

#ifdef _DEBUG
#   ifndef DEBUG
#       define DEBUG
#   endif
#endif


//! release verify
#ifndef HY_VERIFY
#   ifdef _DEBUG
#       define HY_VERIFY(p) assert(p)
#   else
#       define HY_VERIFY(p) p
#   endif
#endif

//! Safe release pointer
#ifndef SAFE_RELEASE
#   define SAFE_RELEASE(p)  if((p)) { (p)->release(); (p) = NULL; }
#endif

//! Safe delete pointer
#ifndef SAFE_DELETE
#   define SAFE_DELETE(p)  if((p)) { delete (p); (p) = NULL; }
#endif // SAFE_DELETE

//! Safe delete pointer array
#ifndef SAFE_DELETE_ARRAY
#   define SAFE_DELETE_ARRAY(p)  if((p)) { delete [](p); (p) = NULL; }
#endif // SAFE_DELETE_ARRAY



//----------------------------------------------------------------------------------------------------
//*	Define type
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------
//     basic data type
//     these type is basic on c/c++ basic data type

namespace hyc {

#define s16_INFINITY            32767
#define s16_INFINITY_NEGATIVE   (-32768)
#define u16_INFINITY            65535
#define s32_INFINITY            (2147483648-1)
#define s32_INFINITY_NEGATIVE   (-2147483648)
#define u32_INFINITY            s32_INFINITY * 2 + 1
#define real_INFINITY           3.40282346638528860e+38

//typedef unsigned char       byte;   //! 8 bit unsigned variable
typedef unsigned char       Byte;   //! prevent to conflict with ::byte.

typedef unsigned char       u8;     //! 8 bit unsigned variable
typedef unsigned short      u16;    //! 16 bit unsigned variable
typedef unsigned int		u32;    //! 32 bit unsigned variable

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
typedef unsigned __int64    u64;    //! 64 bit unsigned variable
#else
typedef unsigned long long  u64;    //! 64 bit unsigned variable
#endif

typedef char                s8;     //! 8 bit signed variable
typedef short               s16;    //! 16 bit signed variable
typedef int					s32;    //! 32 bit signed variable

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
typedef __int64             s64;    //! 64 bit signed variable
#else
typedef long long           s64;    //! 64 bit signed variable
#endif

typedef float               f32;    //! 32 bit floating point variable, 7 digits
typedef double              f64;    //! 64 bit floating point variable, 15 digits

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
typedef f32                 real;
typedef long                integer;
typedef unsigned long       uinteger;
#else
typedef f64                 real;
typedef s64                 integer;
typedef u64                 uinteger;
#endif // CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
    
class cString;
class wString;

#if defined(_UNICODE) || defined(UNICODE)
typedef wString             String;
typedef wchar_t             tchar;
#define END_STR             L'0'
#else
typedef cString             String;
typedef char                tchar;
#define END_STR             0
#endif // UNICODE

} // end namespace of hyc


//----------------------------------------------------------------------------------------------------
//!     property
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------
#define PROPERTY_GET_ONLY(type, name)    \
protected:\
type _##name;\
public:\
const type& get##name() const            { return _##name; }

//-----------------------------------------------
#define PROPERTY_SET_ONLY(type, name)    \
protected:\
type _##name;\
public:\
void        set##name(const type& value) { _##name = value; }

//-----------------------------------------------
#define PROPERTY(type, name)    \
protected:\
type _##name;\
public:\
const type& get##name() const            { return _##name; }\
void        set##name(const type& value) { _##name = value; }


//----------------------------------------------------------------------------------------------------
//*	    private-class macro
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------
//  1. PRIVATE_CLASS_DECLARE    use in class declare
//      private name is: publicClass::PrivateClass
#define PRIVATE_CLASS_DECLARE   \
public:\
class PrivateClass;\
protected:\
    PrivateClass*  m_pPrivateClass;

//-----------------------------------------------
//  2. PRIVATE_CLASS_CREATE     use in class initial
#define PRIVATE_CLASS_CREATE            m_pPrivateClass = new PrivateClass

//-----------------------------------------------
//  3. PRIVATE_CLASS_DESTROY    use in class destroy
#define PRIVATE_CLASS_DESTROY           SAFE_DELETE(m_pPrivateClass);

//-----------------------------------------------
//  4. PRIVATE_CLASS_GET        get the private class
#define PRIVATE_CLASS_GET(pointerName)                              PrivateClass* &pointerName              = m_pPrivateClass;
#define PRIVATE_CLASS_GET_CONST(pointerName)                        const PrivateClass* const &pointerName  = m_pPrivateClass;
#define PRIVATE_CLASS_GET_OTHER(objectPointer, pointerName)         PrivateClass* &pointerName              = (objectPointer)->m_pPrivateClass;
#define PRIVATE_CLASS_GET_OTHER_CONST(objectPointer, pointerName)   PrivateClass* const &pointerName        = (objectPointer)->m_pPrivateClass;
// get the private class of any class.
#define PRIVATE_CLASS_GET_ANY(objectPointer, pointerName, className)               className::PrivateClass* &pointerName   = (objectPointer)->m_pPrivateClass;
#define PRIVATE_CLASS_GET_ANY_CONST(objectPointer, pointerName, className)         const className::PrivateClass* const &pointerName   = (objectPointer)->m_pPrivateClass;
// get the private class of super class.
#define PRIVATE_CLASS_GET_OTHER_SUPER(objectPointer, pointerName, superClassName)         superClassName::PrivateClass* &pointerName          = (objectPointer)->superClassName::m_pPrivateClass;
#define PRIVATE_CLASS_GET_OTHER_CONST_SUPER(objectPointer, pointerName, superClassName)   superClassName::PrivateClass* const &pointerName    = (objectPointer)->superClassName::m_pPrivateClass;


//----------------------------------------------------------------------------------------------------
//*	header file
//----------------------------------------------------------------------------------------------------

#ifdef HYC_COMPILE_MODULE_NET
#   include "network/netPlatform.h"
#endif

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   include <Windows.h>
#   include <tchar.h>

#else
#   include <unistd.h>
#   include <errno.h>
#   include <pthread.h>
#	include <stdint.h>

#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS


//-----------------------------------------------
//	define _T use for auto adapt itself for different char set
#ifndef _T
#   if defined(_UNICODE) || defined(UNICODE)
#       define  _T(s) L##s
#   else
#       define  _T(s) s
#   endif // defined(_UNICODE) || defined(UNICODE)
#endif // _T 

//-----------------------------------------------
// define ZeroMemory macro
#if CN_PLATFORM != CN_PLATFORM_WINDOWS
#   define ZeroMemory   bzero
#endif

#include "_trace.h"
#include "common/cnInterface.h"
#include "common/cnObjectPtr.h"
#include "common/cnSingleton.h"
#include "container/memoryAlloc.h"
#include "common/cnMath.h"
#include "common/cnString.h"


#endif // end of define __BP_HYCPLATFORMa_H_2010102403055__
