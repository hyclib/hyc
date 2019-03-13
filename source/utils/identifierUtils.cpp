//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//
#include "../preheader.h"
#ifdef HYC_COMPILE_MODULE_IDENTIFIER

#include "../../include/utils/encodingUtils.h"
#include "../../include/utils/identifierUtils.h"

#if CN_PLATFORM == CN_PLATFORM_IOS

#elif CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma comment(lib, "Rpcrt4.lib")
#include "../_win32/identifierUtils.hpp"

#else
#include "../_linux/identifierUtils.hpp"

#endif

#endif // HYC_COMPILE_MODULE_IDENTIFIER
