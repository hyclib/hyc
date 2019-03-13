//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc framework
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#ifdef HYC_COMPILE_MODULE_THREADPOOL

#include "../../include/os/threadPool.h"

IMPLEMENT_INTERFACE_SINGLETON(hyc::ThreadPool);

//----------------------------------------------------------------------------------------------------
//*	platform perform
//----------------------------------------------------------------------------------------------------
 
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   include "../_win32/threadPool.hpp"
#elif CN_PLATFORM == CN_PLATFORM_IOS
#elif CN_PLATFORM == CN_PLATFORM_LINUX
#	include "../_linux/threadPool.hpp"
#else
#pragma error "其他平台未实现"
#endif

#endif // HYC_COMPILE_MODULE_THREADPOOL
