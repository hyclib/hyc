/*
 *  Thread.hpp
 *  common
 *
 *  Created by pengxiang li on 1/18/11.
 *  Copyright 2011 hy. All rights reserved.
 *
 */

#include <pthread.h>
#include <signal.h>
#include "../../include/os/conditionVariable.h"
#include "../_trace.h"

//#define gettid() syscall(__NR_gettid)

#if CN_PLATFORM != CN_PLATFORM_WINDOWS
#include <signal.h>
#include <locale.h>

inline static void thread_handle_quit(int)
{
    if (hyc::Thread::CurThreadId() != hyc::Thread::MainThreadId())
    {
        int retval = 0;
        pthread_exit(&retval);
    }
}

#endif

namespace hyc
{
    int g_retval = 0;
    
    //----------------------------------------------------------------------------------------------------
    //*	Thread::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class Thread::PrivateClass : public MemoryAllocObject
    {
    public:
        static void* CallbackHandler(void* lpThreadParameter)
        {
//            sigset_t mask;
//            sigfillset(&mask);
//            pthread_sigmask(SIG_BLOCK, &mask, NULL);
//
//            // 设置线程退出信号.
//            struct sigaction actQuit;
//            ZeroMemory(&actQuit, sizeof(actQuit));
//            sigemptyset(&actQuit.sa_mask);
//            actQuit.sa_flags = 0;
//            actQuit.sa_handler = thread_handle_quit;
//            sigaction(SIGALRM, &actQuit, NULL);
            
            pthread_detach(pthread_self());

            //----------------------------------------------------------------------------------------------------
            //*	initialize linux.
            //----------------------------------------------------------------------------------------------------
#if CN_PLATFORM != CN_PLATFORM_WINDOWS
            
            // 设置local.
            setlocale(LC_ALL, "zh_CN");
            
            // 设置线程退出信号.
            struct sigaction actQuit;
            sigemptyset(&actQuit.sa_mask);
            actQuit.sa_flags = SA_RESTART;
            actQuit.sa_handler = thread_handle_quit;
            HY_VERIFY(0 == sigaction(SIGQUIT, &actQuit, NULL));
            HY_VERIFY(0 == sigaction(SIGABRT, &actQuit, NULL));
            
            // 忽略SIGPIPE信号.
            struct sigaction act;
            sigemptyset(&act.sa_mask);
            act.sa_flags = SA_RESTART;
            act.sa_handler = SIG_IGN;
            HY_VERIFY(0 == sigaction(SIGPIPE, &act, NULL));
            HY_VERIFY(0 == sigaction(SIGCHLD, &act, NULL));
            HY_VERIFY(0 == sigaction(SIGINT, &act, NULL));
            //HY_VERIFY(0 == sigaction(SIGSEGV, &act, NULL));
#endif


            Thread* thread = static_cast<Thread*>(lpThreadParameter);
            assert(thread);
            PRIVATE_CLASS_GET_OTHER(thread, p);

            integer count = thread->getRef();
            if (count > 0)
            {
                thread->addRef();
                p->m_addRef = 1;
            }
            
            if ( thread->onStart(p->m_lpParam) )
            {
                while (true) {
                    if (p->m_exit || thread->onInvoke(p->m_lpParam))
                        break;
                }
                thread->onFinish(p->m_lpParam);
            }

            p->m_hThread = 0;
            p->m_status = ThreadStatus_Dead;
            p->m_id = 0;
            
            p->m_exit = false;

#if defined(HYC_COMPLIE_SUPPORT_TRACE)
            Trace_release(thread->getThreadId());
#endif
            integer expected = 1;
            if (atomic_compare_exchange(&p->m_addRef, &expected, 0))
            {
                thread->release();
            }
            
            return &g_retval;
        }
        
        PrivateClass()
        {
            m_status = ThreadStatus_Uncreated;
            m_exitCode = 0;
            m_id = 0;
            m_hThread = 0;
            m_lpParam = 0;
            m_exit = false;
            m_addRef = 0;
        }
        
        // thread
        ThreadStatus    m_status;
        pthread_t       m_hThread;
        void*           m_lpParam;
        u32             m_exitCode;
        long            m_id;
        ConditionVariable   m_cv;
        SyncObject          m_cvSync;
        bool            m_exit;
        volatile integer    m_addRef;
    };

    //----------------------------------------------------------------------------------------------------
    //*	Thread
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    Thread::Thread( void* lpParam )
    {
        PRIVATE_CLASS_CREATE;
        PRIVATE_CLASS_GET(p);
        
        p->m_lpParam = lpParam;
    }
    
    Thread::~Thread()
    {
        abort();
        PRIVATE_CLASS_DESTROY;
    }
    
    //-----------------------------------------------
    ThreadStatus Thread::getStatus() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_status;
    }
    
    //-----------------------------------------------
    void Thread::abort()
    {
        PRIVATE_CLASS_GET(p);
        this->exit();
        
        if ( p->m_hThread && ThreadStatus_Dead != getStatus() && ThreadStatus_Uncreated != getStatus() )
        {
            trace(_T("use Thread::abort"));

            if ( ThreadStatus_Suspend == getStatus() )
            {
                p->m_status = ThreadStatus_Dead;
                p->m_cv.signal();
                Sleep(1);   // Give up the controls
                p->m_cvSync.lock();
                p->m_cvSync.unlock();
                
                //Sleep(100); // wait for suspend unlock mutex
            }
            
            //pthread_kill( p->m_hThread, SIGQUIT );
            
            if (CurThreadId() != this->getThreadId())
            {
                //pthread_cancel( p->m_hThread );
                //pthread_join( p->m_hThread, NULL );
                if (pthread_kill(p->m_hThread, 0) != ESRCH)
                {
                    pthread_kill(p->m_hThread, SIGQUIT);
                    //pthread_join(p->m_hThread, NULL);
                    onFinish(p->m_lpParam);
                    
                    integer expected = 1;
                    if (atomic_compare_exchange(&p->m_addRef, &expected, 0))
                    {
                        release();
                    }
                }
            }
            else
            {
                onFinish(p->m_lpParam);
                p->m_hThread = 0;
                p->m_status = ThreadStatus_Dead;
                p->m_id = 0;
                pthread_exit(&g_retval);
                p->m_exit = false;
                return;
            }
            
            p->m_hThread = 0;
            p->m_status = ThreadStatus_Dead;
            p->m_id = 0;
        }
        p->m_exit = false;
    }
    
    //-----------------------------------------------
    bool Thread::start()
    {
        PRIVATE_CLASS_GET(p);
        
        if ( getThreadId() == CurThreadId() )       return true;
        
        if ( ThreadStatus_Uncreated == getStatus() || ThreadStatus_Dead == getStatus() )
        {
            p->m_status = ThreadStatus_Running;
            if ( 0 != pthread_create(&p->m_hThread, NULL, Thread::PrivateClass::CallbackHandler, this) )
            {
                assert(0); // create thread failure
                p->m_status = ThreadStatus_Uncreated;
                return false;
            }
            p->m_id = (long)p->m_hThread; // gettid();
            return true;
        } // if
        else if ( ThreadStatus_Suspend == getStatus() )
        {
            p->m_status = ThreadStatus_Running;
            p->m_cv.signal();
        } // if
        
        return true;
    }
    
    //-----------------------------------------------
    bool Thread::suspend()
    {
        if ( getThreadId() != CurThreadId() || ThreadStatus_Running != getStatus() )   return false;

        // suspend
        PRIVATE_CLASS_GET(p);
        p->m_status = ThreadStatus_Suspend;
        p->m_cvSync.lock();
        p->m_cv.wait(p->m_cvSync);
        p->m_cvSync.unlock();

        assert(p->m_status == ThreadStatus_Running || p->m_status == ThreadStatus_Dead);
        
        return true;
    }
    
    //-----------------------------------------------
    bool Thread::suspendTimeout( s32 timeout )
    {
        if ( getThreadId() != CurThreadId() || ThreadStatus_Running != getStatus()  )   return false;
        
        // suspend
        PRIVATE_CLASS_GET(p);
        p->m_cvSync.lock();
        p->m_status = ThreadStatus_Suspend;
        bool r = p->m_cv.waitTimeout(p->m_cvSync, timeout);
        if (p->m_status == ThreadStatus_Suspend)
            p->m_status = ThreadStatus_Running;
        p->m_cvSync.unlock();

        assert(p->m_status == ThreadStatus_Running || p->m_status == ThreadStatus_Dead);

        return r;
    }
    
    //-----------------------------------------------
    uinteger Thread::getExitCode() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_exitCode;
    }

    //-----------------------------------------------
    void Thread::join()
    {
        PRIVATE_CLASS_GET(p);
        if (p->m_hThread) 
        {
            if (getThreadId() == CurThreadId())
                return;

            void* ret;
            pthread_join( p->m_hThread, &ret );
        }
    }

    //-----------------------------------------------
    void Thread::Sleep( u32 milliseconds )
    {
        /*struct timespec timeOut;
        
        timeOut.tv_sec = milliseconds / 1000;
        timeOut.tv_nsec = milliseconds % 1000;
		timeOut.tv_nsec *= 1000000;
        nanosleep( &timeOut, NULL );*/
        
        struct timeval timeout;
        
        timeout.tv_sec = milliseconds/1000;
        timeout.tv_usec = milliseconds % 1000;
		timeout.tv_usec *= 1000;
        
        select( 0, NULL, NULL, NULL, & timeout );
    }
    
    //-----------------------------------------------
    void Thread::exit()
    {
        PRIVATE_CLASS_GET(p);
        p->m_exit = true;
    }
    
    //-----------------------------------------------
    //void Thread::CurThreadAbort( u32 exitCode/* = 0*/ )
    //{
    //    pthread_exit( (void*)g_retval );
    //}
    
    //-----------------------------------------------
    integer Thread::CurThreadId()
    {
        return (integer)::pthread_self(); // gettid();
    }

    //-----------------------------------------------
    CurrentThread::CurrentThread()
    {
        Thread::PrivateClass* p = Thread::m_pPrivateClass;
        p->m_status = ThreadStatus_Running;
        p->m_id = CurThreadId();
        p->m_hThread = pthread_self();
    }

    //-----------------------------------------------
    CurrentThread::~CurrentThread()
    {
        Thread::PrivateClass* p = Thread::m_pPrivateClass;
        p->m_hThread = 0;
    }

    //----------------------------------------------------------------------------------------------------
    //*	SyncObject
    //----------------------------------------------------------------------------------------------------
     
    //-----------------------------------------------
    SyncObject::SyncObject()
    {
        pthread_mutexattr_t  attr;
        pthread_mutexattr_init( &attr );
        pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
        
        pthread_mutex_init( &m_hMutex, &attr );
        
        pthread_mutexattr_destroy( &attr );
    }
    
    //-----------------------------------------------
    SyncObject::~SyncObject()
    {
        //forceUnlock();
        pthread_mutex_destroy( &m_hMutex );
    }
    
    //-----------------------------------------------
    SyncTokenPtr SyncObject::getToken()
    {
        //PRIVATE_CLASS_GET(p);
        return new SyncToken(*this);
    }
// 	
//     //-----------------------------------------------
//     void SyncObject::forceUnlock()
//     {
//         unlock();
//     }
    
    //-----------------------------------------------
    void SyncObject::lock()
    {
        pthread_mutex_lock( &m_hMutex );
    }
    
    //-----------------------------------------------
    void SyncObject::unlock()
    {
        pthread_mutex_unlock( &m_hMutex );
    }

    //-----------------------------------------------
    bool SyncObject::tryLock()
    {
        return 0 == pthread_mutex_trylock( &m_hMutex );
    }

    
	
} // end namespace hyc


