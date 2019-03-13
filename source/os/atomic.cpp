
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//


#include "../preheader.h"
#include "../../include/os/thread.h"
#include "../../include/os/atomic.h"

//-----------------------------------------------
//	windows 平台
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
// #include "../_win32/atomic.hpp"

//-----------------------------------------------
//	其他平台
#else
// #include "../_linux/atomic.hpp"

#endif // BP_PLATFORM == PLATFORM_WINDOWS

//__sync_fetch_and_add 

namespace hyc {
    
    //-----------------------------------------------
    AtomicSyncObject::~AtomicSyncObject()
    {
        lockAll();
        unlockAll();

        if (_tid != 0)
#ifdef _DEBUG
            assert(false); //"AtomicSyncObject is lock in destroy".
#else
            error_all(_T("AtomicSyncObject is lock in destroy"));
#endif // _DEBUG
    }

    //-----------------------------------------------
    void AtomicSyncObject::lockAll()
    {
        integer old = 0;
        integer tid = Thread::CurThreadId();

        // 等待锁.
        while (!atomic_compare_exchange((integer volatile*)&_tid, &old, tid))
        {
            old = 0;
        }

        assert(Thread::CurThreadId() == _tid);
    }

    //-----------------------------------------------
    void AtomicSyncObject::unlockAll()
    {
        assert(Thread::CurThreadId() == _tid);

        integer old = 0;
        integer tid = Thread::CurThreadId();

        HY_VERIFY(atomic_compare_exchange((integer volatile*)&_tid, &tid, old));
    }

    //-----------------------------------------------
    bool AtomicSyncObject::tryLock()
    {
        integer old = 0;
        integer tid = Thread::CurThreadId();

        return atomic_compare_exchange((integer volatile*)&_tid, &old, tid);
    }


}
