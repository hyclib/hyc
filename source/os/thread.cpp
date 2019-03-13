//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"
#include "../globalVar.hpp"

#ifdef HYC_COMPILE_MODULE_THREAD

#include "../../include/os/thread.h"

//-----------------------------------------------
//	windows 平台
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../_win32/thread.hpp"

//----------------------------------------------
//  Mac Os / iOS
#elif CN_PLATFORM == CN_PLATFORM_IOS || CN_PLATFORM == CN_PLATFORM_MACOSX
#include "../_iphone/thread.hpp"

//-----------------------------------------------
//	其他平台
#else
#include "../_linux/thread.hpp"

#endif // BP_PLATFORM == PLATFORM_WINDOWS


//----------------------------------------------------------------------------------------------------
//*	公共实现
//----------------------------------------------------------------------------------------------------

namespace hyc {

    //-----------------------------------------------
    integer Thread::MainThreadId()
    {
        return g_mainThreadId;
    }

	
    //----------------------------------------------------------------------------------------------------
    //*	SyncToken
    //----------------------------------------------------------------------------------------------------
	
    //-----------------------------------------------
    SyncToken::SyncToken(SyncObject& pObj)
        : _obj(pObj)
    {		
        //PRIVATE_CLASS_GET_OTHER_SUPER(&pObj, pSync, SyncObject);
        pObj.lock();
    }
	
    //-----------------------------------------------
    SyncToken::~SyncToken()
    {
        _obj.unlock();
        //unlock();
    }

    //-----------------------------------------------
    hyc::integer Thread::getThreadId() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_id;
    }


} // end namespace of hyc

#endif // HYC_COMPILE_MODULE_THREAD
