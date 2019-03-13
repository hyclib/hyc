//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hy yun network
// author:       lipengxiang
// description:  
//

#ifndef __BP_OS_H_20120208153935__
#define __BP_OS_H_20120208153935__


void _testOs();

hyc::Thread* g_thread1 = NULL;
hyc::Thread* g_thread2 = NULL;
hyc::Thread* g_thread3 = NULL;
hyc::Thread* g_thread4 = NULL;
int g_i = 0;
hyc::SyncObject sync0;

class Thread0 : public hyc::Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        hyc::SyncTokenPtr token = sync0.getToken();
        for (u32 i = 0; i < 100; ++i)
        {
            
            print_all_line(_T("%d t0:%d"), (int)getThreadId(), g_i++);
        }
        return true;
    }
};


//-----------------------------------------------
// start/suspend
class Thread1 : public hyc::Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        hy_assertTrue( getThreadId() == Thread::CurThreadId() );
        hy_assertTrue( getThreadId() == Thread::CurThreadId() );

        for (u32 i = 0; i < 10; ++i)
        {
            Sleep(10);
            print_all(_T("t1:%d "), i);
            g_thread2->start();
            hy_assertTrue( suspend() );
        }
        return true;
    }
};

class Thread2 : public hyc::Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        for (u32 i = 0; i < 10; ++i)
        {
            Sleep(10);
            print_all_line(_T(" t2:%d"), i);
            g_thread1->start();
            if (i < 9)
                hy_assertTrue( suspend() );
        }
        return true;
    }
};

//-----------------------------------------------
// start/suspendTimeout
class Thread3 : public hyc::Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        //g_thread4->start();
        print_all_line(_T("%s"), suspendTimeout(4000)?_T("be waked"):_T("timeout"));
        return true;
    }
};
class Thread4 : public hyc::Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        Sleep(3000);
        g_thread3->start();
        return true;
    }
};

SyncObject osSync;
AtomicSyncObject osAtomicSync;
class Thread5 : public Thread
{
public:
	bool onInvoke(void*)
	{
		osAtomicSync.lockAll();
		{
			hy_assertTrue(osSync.tryLock());
			osSync.unlock();
		}
		osAtomicSync.unlockAll();

		return true;
	}
};

ConditionVariable osCV;
class Thread6 : public Thread
{
public:
    bool onInvoke(void*)
    {
        Timer timer;
        timer.start();
        osSync.lock();
        bool r = osCV.waitTimeout(osSync, 2000);
        osSync.unlock();

        print_all_line(_T("condition variable timeout %d %u"), (int)r, timer.getMilliseconds());


        return true;
    }
};

//////////////////////////////////////////
// os
void os()
{
    ///////////////////////////////////
    //  thread
    ///////////////////////////////////

    Thread0* pT = new Thread0;
    Thread0* pT1 = new Thread0;
    pT->start();
    pT1->start();
     Thread::Sleep(3000);
     delete pT;
    delete pT1;
 
     // start/suspend
     print_all_line(_T("\n====================================\n== thread test start start/suspend\n===================================="));
     g_thread1 = new Thread1();
     g_thread2 = new Thread2();
     g_thread1->start();
     g_thread1->start();
     g_thread1->join();
     g_thread2->join();
 
     delete g_thread1;
     delete g_thread2;
     print_all_line(_T("\n====================================\n== thread test end\n===================================="));
 
     // start/suspend
     print_all_line(_T("\n====================================\n== thread test start start/suspendTimeout\n===================================="));
     print_all_line(_T("must be 1.timeout 2.be waked"));
     g_thread3 = new Thread3();
     g_thread3->start();
     for (u32 i = 0; i < 6; ++i)
     {
         print_all_line(_T("%d"), i );
         Thread::Sleep(1000);
     }
     delete g_thread3;
     g_thread3 = new Thread3();
     g_thread4 = new Thread4();
     g_thread3->start();
     g_thread4->start();
     Thread::Sleep(10000);
     delete g_thread3;
     delete g_thread4;
 
     // atomic sync lock.
     Array<ThreadPtr> arr;
     for (u32 i = 0; i < 100; i++)
     {
     	arr.push_back(new Thread5);
     }
 
     for (u32 i = 0; i < arr.size(); i++)
     {
     	arr[i]->start();
     }
 
     for (u32 i = 0; i < arr.size(); i++)
     {
     	arr[i]->join();
     }
 
     print_all_line(_T("\n====================================\n== thread test end\n===================================="));

    print_all_line(_T("\n====================================\n== ConditionVariable test\n===================================="));
    Thread* pThread6;

    pThread6 = new Thread6;
    pThread6->start();
    Thread::Sleep(1000);
    osCV.broadcast();
    pThread6->join();
    delete pThread6;
    print_all_line(_T("\n====================================\n== ConditionVariable test end\n===================================="));


}


#endif // end of define __BP_OS_H_20120208153935__

