//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// common framework
// author:       lipengxiang
// description:  memory leak detector
//

#ifndef __BP_HYCLEAKDETECTOR_H_2011031694456__
#define __BP_HYCLEAKDETECTOR_H_2011031694456__

/*---------------------------
!  the triggle is:
!    1. init:    CN_LEAK_DETECTOR;
!    2. on:      #define new CN_LEAK_DETECTOR_ON
!    3. off:     #undef new
---------------------------*/

//----------------------------------------------------------------------------------------------------
//* Windows platform placement new
//----------------------------------------------------------------------------------------------------
 
#if CN_PLATFORM == CN_PLATFORM_WINDOWS

#ifdef _DEBUG
#   include <crtdbg.h>
#   define CN_LEAK_DETECTOR        _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#   define CN_LEAK_DETECTOR_ON    new( _CLIENT_BLOCK, __FILE__, __LINE__)
#   define new CN_LEAK_DETECTOR_ON
#else
#   define CN_LEAK_DETECTOR
#endif // _DEBUG

#else
#define CN_LEAK_DETECTOR

#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS



#endif // end of define __BP_HYCLEAKDETECTOR_H_2011031694456__
