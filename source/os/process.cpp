//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_PROCESS

#include "../../include/os/process.h"

//-----------------------------------------------
//	windows 平台
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../_win32/process.hpp"

//-----------------------------------------------
//  linux 平台
#elif CN_PLATFORM == CN_PLATFORM_LINUX
#include "../_linux/process.hpp"

//-----------------------------------------------
//	其他平台
#else
#pragma error("其他平台未实现")

#endif // BP_PLATFORM == PLATFORM_WINDOWS


//----------------------------------------------------------------------------------------------------
//*	公共实现
//----------------------------------------------------------------------------------------------------
 
namespace hyc
{

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_PROCESS
