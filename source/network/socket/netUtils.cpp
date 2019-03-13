//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//


#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/socket/netUtils.h"


//----------------------------------------------------------------------------------------------------
//*	平台实现
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------
//	windows 平台
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../../_win32/network/netUtils.hpp"

//-----------------------------------------------
// 其他平台
#else
#pragma error("其他平台未实现")
#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS

#endif // HYC_COMPILE_MODULE_NET
