//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_THREAD_H_20130402221140__
#define __BP_THREAD_H_20130402221140__

SyncObject sync;

class ThreadDemo : Thread
{
public:
    virtual uinteger onInvoke(void* lpParam)
    {
        sync.lock();
        // do something.
        sync.unlock();

        return 0;
    }
};

void os_thread()
{
    ThreadDemo t;
    t.start();
}


#endif // end of define __BP_THREAD_H_20130402221140__