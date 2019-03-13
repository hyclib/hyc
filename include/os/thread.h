//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  线程处理
//

#ifdef HYC_COMPILE_MODULE_THREAD

#ifndef __BP_THREAD_H_2010110894459__
#define __BP_THREAD_H_2010110894459__

#include "../common/cnObjectPtr.h"

namespace hyc
{
    class SyncObject;
    class ConditionVariable;

    //----------------------------------------------------------------------------------------------------
    //!	令牌; 因为可能需要在网络中使用, 故不使用私类
    //----------------------------------------------------------------------------------------------------
    class HYC_API SyncToken : public RefCount, public MemoryAllocObject
    {
    public:
        ~SyncToken();

    protected:
        SyncToken(SyncObject& pObj);
        SyncObject& _obj;
        friend class SyncObject;
    };
    typedef ObjectPtr<SyncToken>    SyncTokenPtr;

    //----------------------------------------------------------------------------------------------------
    //!	Synchronism object; use to mark a synchronization
    //!  只能用于进程内线程的同步; 因为可能需要在网络中使用, 故不使用私类
    //----------------------------------------------------------------------------------------------------
    class HYC_API SyncObject : public INoncopyable, public MemoryAllocObject, public RefCount
    {
    public:
        SyncObject();
        ~SyncObject();

        /**
         * @brief 使用这种方式获得的SyncTokenRef, 必须在SyncObject释放之前释放, 否则系统崩溃
         */
        SyncTokenPtr    getToken();

        /**
         * @brief 使用引用计数的方式进行锁
         *      调用一次lock, 本线程对锁的引用计数+1
         *      调用一次unlock, 本线程对锁的引用计数-1
         *      没有引用计数时, 本线程对锁无锁
         */
        void            lock();
        void            unlock();

        /**
         * @brief 成功锁定时返回true, 否则为false已被其他线程占用
         */
        bool            tryLock();

    protected:

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        CRITICAL_SECTION    m_csLock;
#else
        pthread_mutex_t     m_hMutex;
#endif
        friend class ConditionVariable;
    };
    typedef ObjectPtr<SyncObject>   SyncObjectPtr;

    
    //----------------------------------------------------------------------------------------------------
    //!	Thread interface
    //----------------------------------------------------------------------------------------------------

    //--------------------------------
    typedef enum {
        ThreadStatus_Uncreated,
        ThreadStatus_Running,
        ThreadStatus_Suspend,
        ThreadStatus_Dead,
    } ThreadStatus;    

    //--------------------------------
    class HYC_API Thread : public INoncopyable, public MemoryAllocObject, public RefCount
    {
        PRIVATE_CLASS_DECLARE;
    public:
        Thread(void* lpParam = NULL);
        virtual ~Thread();

        /*---------------------------
          对当前线程的操作
        ---------------------------*/
        static  integer CurThreadId();
        static  integer MainThreadId();
        static  void    Sleep( u32 milliseconds );

        /*---------------------------
          线程的操作
        ---------------------------*/

        /**
         * @brief wait for thread abort
         */
        void            join();

        /**
         * @brief it would be best not to use this function
         */
        void            abort();

        /**
         * @brief start or resume thread
         */
        bool            start();

        /**
         * @brief 线程的挂起只能由线程自身调用
         */
        bool            suspend();
        /**
         * @brief 在超时时间内挂起线程，如果这此时间内没有手动启动线程，将返回false
         */
        bool            suspendTimeout( s32 timeout );

        uinteger        getExitCode() const;
        ThreadStatus    getStatus() const;
        integer         getThreadId() const;
        
        /**
         * @brief 安全退出函数, 调用此函数后, 在onInvoke中安全退出.
         */
        virtual void    exit();

        /**
         * @brief After thread start, invoke will run
         *  onStart 函数返回 false时,将不再执行线程,onInvoke及onFinish函数不会被调用.
         *  onInvoke函数将不断地循环执行, 只有当返回true时退出.
         *      onStart -> onInvoke-> ... ->onInvoke -> onFinish.
         */
        virtual bool    onStart(void* lpParam)   { return true; }
        virtual bool    onInvoke(void* lpParam) = 0;
        virtual void    onFinish(void* lpParam)   {}
    };
    typedef ObjectPtr<Thread>   ThreadPtr;

    //-----------------------------------------------
    //! 当前线程; 对象析构后不会中断线程
    class HYC_API CurrentThread : public Thread
    {
    public:
        CurrentThread();
        virtual ~CurrentThread();

        virtual bool     onInvoke(void* lpParam) { return false; }
    };


} // end namespace hyc

#endif // end of define __BP_THREAD_H_2010110894459__
#endif // HYC_COMPILE_MODULE_THREAD
