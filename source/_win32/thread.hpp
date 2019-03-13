//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  线程的win32平台实现
//

#ifndef __BP_THREAD_HPP_20101108100955__
#define __BP_THREAD_HPP_20101108100955__

#include "../../include/os/conditionVariable.h"
#include "../_trace.h"

namespace hyc
{
    //     typedef u32 (ThreadCallback::*CallbackPointer)(void* lpParam);
    //     // 静态量
    //     CallbackPointer Thread::PrivateClass::ms_callbackPointer   = &ThreadCallback::invoke;
    //     typedef std::map<s32, PCRITICAL_SECTION>    CsMap;

    //----------------------------------------------------------------------------------------------------
    //*	Thread::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class Thread::PrivateClass : public MemoryAllocObject
    {
    public:
        static DWORD WINAPI CallbackHandler(LPVOID lpThreadParameter)
        {
            Thread* thread = static_cast<Thread*>(lpThreadParameter);
            PRIVATE_CLASS_GET_OTHER(thread, p);

            integer count = thread->getRef();
            if (count > 0)
            {
                thread->addRef();
                p->m_addRef = 1;
            }
            assert(thread);
            
            if (thread->onStart(p->m_lpParam))
            {
                while (true) {
                    if (p->m_exit || thread->onInvoke(p->m_lpParam))
                        break;
                }
                thread->onFinish(p->m_lpParam);
            }
            p->m_exit = false;

#if defined(HYC_COMPLIE_SUPPORT_TRACE)
            Trace_release(thread->getThreadId());
#endif

            integer expected = 1;
            if (atomic_compare_exchange(&p->m_addRef, &expected, 0))
            {
                thread->release();
            }

            if (p->m_hThread)
            {
                ::CloseHandle(p->m_hThread);
                p->m_hThread = 0;
                p->m_status  = ThreadStatus_Dead;
                p->m_id = 0;
            }

            return 0;
        }

        PrivateClass() : m_exit(false), m_addRef(0), m_hThread(NULL), m_lpParam(NULL), m_id(0)  {}

        HANDLE              m_hThread;
        LPVOID              m_lpParam;
        ThreadStatus        m_status;
        s32                 m_id;
        class __TimeoutSync : public RefCount {
        public:
            ConditionVariable    m_cv;
            SyncObject           m_sync;
            bool                 m_useThis;            
        };
        ObjectPtr<__TimeoutSync>    m_pTS;
        bool                m_exit;
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

        // 回调及参数保存
        p->m_lpParam = lpParam;

        DWORD id = 0;
        // 创建线程
        p->m_hThread = 0; //::CreateThread(NULL, 0, Thread::PrivateClass::CallbackHandler, this, CREATE_SUSPENDED, &id);
        //         if (NULL == p->m_hThread)
        //             UTILSLOG(_T("Thread_windows::Thread_windows"), _T("failure ::CreateThread in Thread_windows::Thread_windows"));

        p->m_status = ThreadStatus_Uncreated;
        p->m_id = (s32)id;
    }

    Thread::~Thread()
    {
        PRIVATE_CLASS_GET(p);
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

        if (p->m_hThread)
        {
            if (ThreadStatus_Uncreated == p->m_status)
            {
                ::CloseHandle(p->m_hThread);
            }
            else
            {
                trace(_T("use Thread::abort"));

                if (ThreadStatus_Suspend == p->m_status)
                {
                    if (p->m_pTS.get() && p->m_pTS->m_useThis)
                    {
                        p->m_status = ThreadStatus_Dead;
                        p->m_pTS->m_cv.signal();
                        p->m_pTS->m_sync.lock();
                        p->m_pTS->m_sync.unlock();
                    }
                }

                if (CurThreadId() == getThreadId())
                {
                    ::ExitThread(0);
                    ::CloseHandle(p->m_hThread);
                }
                else
                {
                    ::TerminateThread(p->m_hThread, 0);
                    ::CloseHandle(p->m_hThread);
                }

                integer expected = 1;
                if (atomic_compare_exchange(&p->m_addRef, &expected, 0))
                {
                    release();
                }
            }

            p->m_status = ThreadStatus_Dead;
            p->m_hThread = NULL;
            p->m_id = 0;
        }

        p->m_exit = false;
    }

    //-----------------------------------------------
    bool Thread::start()
    {
        PRIVATE_CLASS_GET(p);

        if (getThreadId() == CurThreadId())
            return true;

        if (p->m_hThread)
        {
            if ( p->m_pTS.get() && p->m_pTS->m_useThis )  // suspendTimeout
            {
                p->m_status = ThreadStatus_Running;
                p->m_pTS->m_useThis = false;
                p->m_pTS->m_cv.signal();
            }
            else    // suspend
            {
                if (0xFFFFFFFF == ::ResumeThread(p->m_hThread))
                {
                    //                 UTILSLOG(_T("Thread_windows::start"), _T("failure ::ResumeThread in Thread_windows::start"));
                    p->m_status = ThreadStatus_Dead;
                    return false;
                }
            } // if..else

            p->m_status = ThreadStatus_Running;
            return true;
        }
        else
        {
            DWORD id;
            // 创建线程
            p->m_hThread = ::CreateThread(NULL, 0, Thread::PrivateClass::CallbackHandler, this, CREATE_SUSPENDED, &id);
            //         if (NULL == p->m_hThread)
            //             UTILSLOG(_T("Thread_windows::Thread_windows"), _T("failure ::CreateThread in Thread_windows::Thread_windows"));

            p->m_status = (NULL==p->m_hThread) ? ThreadStatus_Dead : ThreadStatus_Uncreated;
            p->m_id = (s32)id;

            if (!p->m_hThread)
                return false;

            start();
            return true;
        } // if..else.

    }

    //-----------------------------------------------
    bool Thread::suspend()
    {
        PRIVATE_CLASS_GET(p);

        if (CurThreadId() != getThreadId())
            return false;

        if (p->m_hThread)
        {
            p->m_status = ThreadStatus_Suspend;
            if (0xFFFFFFFF == ::SuspendThread(p->m_hThread))
            {
                //                 UTILSLOG(_T("Thread_windows::suspend"), _T("failure ::SuspendThread in Thread_windows::suspend"));
                p->m_status = ThreadStatus_Dead;
                return false;
            }
            return true;
        } // if

        //         UTILSLOG(_T("Thread_windows::suspend"), _T("thread not be created; failure ::SuspendThread in Thread_windows::suspend"));
        p->m_status = ThreadStatus_Dead;
        return false;
    }

    //-----------------------------------------------
    bool Thread::suspendTimeout( s32 timeout )
    {
        PRIVATE_CLASS_GET(p);

        if (CurThreadId() != getThreadId())
            return false;

        if (!p->m_pTS.get())
            p->m_pTS = new PrivateClass::__TimeoutSync;
        
        p->m_pTS->m_sync.lock();
        p->m_status = ThreadStatus_Suspend;
        p->m_pTS->m_useThis = true;
        bool r = p->m_pTS->m_cv.waitTimeout(p->m_pTS->m_sync, timeout);
        p->m_pTS->m_sync.unlock();
        return r;
    }

    //-----------------------------------------------
    void Thread::join()
    {
        PRIVATE_CLASS_GET(p);
        if (p->m_hThread)
        {
            if (getThreadId() == CurThreadId())
                return;
            ::WaitForSingleObject( p->m_hThread, INFINITE );
        }
    }

    //-----------------------------------------------
    uinteger Thread::getExitCode() const
    {
        PRIVATE_CLASS_GET_CONST(p);

        DWORD exitCode;
        BOOL r = ::GetExitCodeThread( p->m_hThread, &exitCode );
        if (0 == r || STILL_ACTIVE == exitCode)
            //UTILSLOG(_T("Thread_windows::getExitCode"), _T("failure ::GetExitCodeThread in Thread_windows::getExitCode maybe thread is runing"));
            return 0;

        return exitCode;
    }

//     //-----------------------------------------------
//     void Thread::CurThreadAbort( u32 exitCode/* = 0*/ )
//     {
//         HANDLE hThread = ::GetCurrentThread();
//         ::TerminateThread( hThread, exitCode );
//     }

    //     //-----------------------------------------------
    //     bool Thread::CurThreadSuspend()
    //     {
    //         HANDLE hThread = ::GetCurrentThread();
    //         DWORD dwResult = ::SuspendThread(hThread);
    //         if (0xFFFFFFFF == dwResult)
    //         {
    //             return false;
    //         }
    //         return true;
    //     }

    //-----------------------------------------------
    integer Thread::CurThreadId()
    {
        return ::GetCurrentThreadId();
    }

    //-----------------------------------------------
    void Thread::Sleep( u32 milliseconds )
    {
        ::Sleep( milliseconds );
    }

    //-----------------------------------------------
    void Thread::exit()
    {
        PRIVATE_CLASS_GET(p);
        p->m_exit = true;
    }


    //-----------------------------------------------
    CurrentThread::CurrentThread()
    {
        Thread::PrivateClass* p = Thread::m_pPrivateClass;
        p->m_id = CurThreadId();
        p->m_hThread = ::OpenThread(THREAD_ALL_ACCESS, TRUE, p->m_id );
        p->m_status = ThreadStatus_Running;
    }

    //-----------------------------------------------
    CurrentThread::~CurrentThread()
    {
        Thread::PrivateClass* p = Thread::m_pPrivateClass;
        if (p->m_hThread)
        {
            ::CloseHandle( p->m_hThread );
            p->m_hThread = NULL;
        }
    }

// 
//     //-----------------------------------------------
//     void Thread::SuspendThreadById( s32 pid )
//     {
//         HANDLE hThread = ::OpenThread( THREAD_ALL_ACCESS, TRUE, (DWORD)pid );
//         if (hThread)
//         {
//             ::SuspendThread( hThread );
//             ::CloseHandle( hThread );
//         }
//     }
// 
//     //-----------------------------------------------
//     void Thread::StartThreadById( s32 pid )
//     {
//         HANDLE hThread = ::OpenThread( THREAD_ALL_ACCESS, TRUE, (DWORD)pid );
//         if (hThread)
//         {
//             ::ResumeThread( hThread );
//             ::CloseHandle( hThread );
//         }
//     }

    //----------------------------------------------------------------------------------------------------
    //*	SyncObject::PrivateClass
    //----------------------------------------------------------------------------------------------------
//     class SyncObject::PrivateClass
//     {
//     public:
//         PrivateClass()
//         {
//             //             m_bLocked = false;
//             ::InitializeCriticalSection(&m_csLock);
//         }
//         ~PrivateClass()
//         {
//             //             if (m_bLocked)
//             //             {
//             unlock();
//             //             }
//             ::DeleteCriticalSection(&m_csLock);
//         }
// 
//         //-----------------------------------------------
//         void lock()
//         {
//             ::EnterCriticalSection(&m_csLock);
//             //             m_bLocked = true;
//         }
// 
//         //-----------------------------------------------
//         void unlock()
//         {
//             //             if (m_bLocked)
//             //             {
//             ::LeaveCriticalSection(&m_csLock);
//             //                 m_bLocked = false;
//             //             }
//         }
// 
//         CRITICAL_SECTION    m_csLock;
//         //         bool                m_bLocked;
// 
//         friend class SyncToken;
//     };


    //----------------------------------------------------------------------------------------------------
    //*	SyncObject
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    SyncObject::SyncObject()
    {
        ::InitializeCriticalSection(&m_csLock);
    }

    //-----------------------------------------------
    SyncObject::~SyncObject()
    {
        //debug_print("[so %d]", (u32)this);

        if ((s32)m_csLock.OwningThread != 0)
        {
            error_all(_T("syncObject is lock in destroy"));
            throw ExceptionType_Forbid;
        }

        ::DeleteCriticalSection(&m_csLock);
    }

    //-----------------------------------------------
    SyncTokenPtr SyncObject::getToken()
    {
        return new SyncToken(*this);
    }

//     //-----------------------------------------------
//     void SyncObject::forceUnlock()
//     {
//         while (-1 > m_csLock.LockCount && NULL != m_csLock.OwningThread)
//         {
//             ::LeaveCriticalSection(&m_csLock);
//         }
//     }

    //-----------------------------------------------
    void SyncObject::lock()
    {
        ::EnterCriticalSection(&m_csLock);
    }

    //-----------------------------------------------
    void SyncObject::unlock()
    {
        if ((s32)m_csLock.OwningThread != Thread::CurThreadId())
        {
            error_all(_T("syncObject unlock no by owner"));
            throw ExceptionType_Forbid;
        }

        ::LeaveCriticalSection(&m_csLock);
    }

    //-----------------------------------------------
    bool SyncObject::tryLock()
    {
         return 0 != ::TryEnterCriticalSection(&m_csLock);
    }


} // end namespace hyc

#endif // end of define __BP_THREAD_HPP_20101108100955__