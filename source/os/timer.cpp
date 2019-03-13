//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#ifdef HYC_COMPILE_MODULE_TIMER

#include "../../include/os/timer.h"
#include <time.h>

//-----------------------------------------------
//	windows 平台
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../_win32/timer.hpp"

//-----------------------------------------------
//	其他平台
#else
#include "../_linux/timer.hpp"

#endif // BP_PLATFORM == PLATFORM_WINDOWS

//----------------------------------------------------------------------------------------------------
//*	公共实现
//----------------------------------------------------------------------------------------------------
namespace hyc {

}

#endif // HYC_COMPILE_MODULE_TIMER
