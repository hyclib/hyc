//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// common libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#if defined(CN_PLATFORM_MOBILE)
#undef HYC_COMPILE_MODULE_IO_DEVICE
#endif

#if defined(HYC_COMPILE_MODULE_IO_DEVICE)

#include "../../include/io/mouse.h"
#include "../../include/io/keyborad.h"

//-----------------------------------------------
//	windows 平台
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../_WIN32/Mouse.hpp"

//-----------------------------------------------
//	其他平台
#else
#pragma error("其他平台未实现")

#endif // BP_PLATFORM == PLATFORM_WINDOWS

#endif
