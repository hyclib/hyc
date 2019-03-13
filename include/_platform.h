//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// author:       lipengxiang
// description:  骞冲彴瀹忕殑瀹氫箟
//

#ifndef __BP__PLATFORMa_H_20110304181826123__
#define __BP__PLATFORMa_H_20110304181826123__

//----------------------------------------------------------------------------------------------------
//*	platform macro.
//----------------------------------------------------------------------------------------------------

//--------------------------------------------------
//     Platform macro
//         - CN_PLATFORM        
//              - CN_PLATFORM_WINDOWS
//              - CN_PLATFORM_LINUX
//              - CN_PLATFORM_MACOSX
//              - CN_PLATFORM_XBOX
//              - CN_PLATFORM_PS3
//              - CN_PLATFORM_MOBILE
//                  - CN_PLATFORM_IPHONE
//                  - CN_PLATFORM_ANDROID
//         - CN_CPU_ARCH                The architecture of cpu
//              - CN_CPU_ARCHITECTURE_32
//              - CN_CPU_ARCHITECTURE_64
//         - CN_IS_BIG_ENDIAN
//         - CN_IS_ARCHITECTURE_64
//         - CN_COMPILER                compiler information.
//              - CN_COMPILER_MSVC
//              - CN_COMPILER_GNUC
//              - CN_COMPILER_BORL
//         - CN_COMPILER_VER            Version of compiler
//         - CN_ATTRIBUTE_DEPRECATED    Mark Deprecated function.
#define CN_PLATFORM_WINDOWS   1
#define CN_PLATFORM_LINUX     2
#define CN_PLATFORM_MACOSX    3
#define CN_PLATFORM_XBOX      4
#define CN_PLATFORM_PS3       5
#define CN_PLATFORM_IOS       3 //6
#define CN_PLATFORM_ANDROID   7

// compiler
#define CN_COMPILER_MSVC   1    // COMPILER_MSVC for microsoft visual c++
#define CN_COMPILER_GNUC   2    // COMPILER_GNUC for GNU c++
#define CN_COMPILER_BORL   3    // COMPILER_BORL for borland c++

// computer architecture bit
#define CN_CPU_ARCHITECTURE_32 1
#define CN_CPU_ARCHITECTURE_64 2

//-----------------------------------------------
//* CN_PLANTFORM

#if defined(__APPLE_CC__)
#include <TargetConditionals.h>
#endif

#if defined(__WIN32__) || defined(_WIN32)
#   define CN_PLATFORM     CN_PLATFORM_WINDOWS
#   define OEMRESOURCE

#elif defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
#   define CN_PLATFORM     CN_PLATFORM_IOS
#   define CN_PLATFORM_MOBILE   1

#elif defined(__APPLE_CC__)
#   define CN_PLATFORM     CN_PLATFORM_MACOSX
#   include <mach/mach.h>

#elif defined(__ANDROID__) || defined(__android__) || defined(ANDROID)
#   define CN_PLATFORM     CN_PLATFORM_ANDROID
#   define CN_PLATFORM_MOBILE   1

#elif defined(__LINUX__) || defined(__linux__)
#   define CN_PLATFORM     CN_PLATFORM_LINUX

#elif defined(_XBOX)
#   pragma error "XBOX unimplements"
#   define CN_PLATFORM     CN_PLATFORM_XBOX

#elif defined(__ps3__)
#   pragma error "ps3 unimplements"
#   define CN_PLATFORM     CN_PLATFORM_PS3

#else
#   pragma error "Not support this plantform"
#endif // defined(__WIN32__) || defined(_WIN32)

//-----------------------------------------------
//* CN_COMPILER / CN_COMPILER_VER
#if defined(_MSC_VER)
#   define CN_COMPILER      CN_COMPILER_MSVC
#   define CN_COMPILER_VER  _MSC_VER
#   define CN_ATTRIBUTE_DEPRECATED __declspec(deprecated)
#elif defined(__GNUC__)
#   define CN_COMPILER      CN_COMPILER_GNUC
#   define CN_COMPILER_VER  (((__GNUC__)*100)+(__GNUC_MINOR__*10)+__GNUC_PATCHLEVEL__)
#   define CN_ATTRIBUTE_DEPRECATED __attribute__((deprecated))
#elif defined(__BORLANDC__)
#   define CN_COMPILER      CN_COMPILER_BORL
#   define CN_COMPILER_VER  __BCPLUSPLUS__
#   define CN_ATTRIBUTE_DEPRECATED
#   define __FUNCTION__     __FUNC__ 
#else
#   pragma error "No support this Compiler"
#endif // defined(_MSC_VER)

//-----------------------------------------------
//* CN_CPU_ARCH.
#if defined(__arm64__) || defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define CN_CPU_ARCH  CN_CPU_ARCHITECTURE_64
#else
#   define CN_CPU_ARCH  CN_CPU_ARCHITECTURE_32
#endif // cpu.



#endif // end of define __BP__PLATFORMa_H_20110304181826123__
