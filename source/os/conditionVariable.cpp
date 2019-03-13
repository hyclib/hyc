//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"
#include "../../include/common/cnObjectPtr.h"

#ifdef HYC_COMPILE_MODULE_THREAD

#include "../../include/os/conditionVariable.h"

//-----------------------------------------------
//	windows 平台
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../_win32/conditionVariable.hpp"

//----------------------------------------------
//  Mac Os / iOS
#elif CN_PLATFORM == CN_PLATFORM_IOS || CN_PLATFORM == CN_PLATFORM_MACOSX
#include "../_iphone/conditionVariable.hpp"

//-----------------------------------------------
//	其他平台
#else
#include "../_linux/conditionVariable.hpp"

#endif // BP_PLATFORM == PLATFORM_WINDOWS

#endif
