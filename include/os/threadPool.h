//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc framework
// author:       lipengxiang
// description:  线程池
//

#ifdef HYC_COMPILE_MODULE_THREADPOOL

#ifndef __BP_THREADPOOL_H_20110316180028__
#define __BP_THREADPOOL_H_20110316180028__

#include "thread.h"
#include "../common/cnObjectPtrSafe.h"
#include "conditionVariable.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	线程池工作项
    //----------------------------------------------------------------------------------------------------
    
//     //--------------------------------
//     //  方式1: 函数; (删除此方式)
//     typedef unsigned long (__stdcall *ThreadPoolWorkItemProc)(void* param);

    //--------------------------------
    //!  线程池工作项
    class HYC_API ThreadPoolWorkItem : public RefCountSafe
    {
        SyncObject          _syncJoin;
        ConditionVariable   _cvJoin;
        bool                _exit;
    public:
        ThreadPoolWorkItem() : _exit(false) {}
        virtual ~ThreadPoolWorkItem() {}

        bool                isExit() const { return _exit; }

        /**
         * @brief 获得条件变量.
         */
        ConditionVariable&  getCVJoin() { return _cvJoin; }

        /**
         * @brief 等待线程自然终止.
         */
        void                join()  { if (0 == getRef()) return; _syncJoin.lock(); _cvJoin.wait(_syncJoin); _syncJoin.unlock(); }

        /**
         * @brief 安全退出函数, 调用此函数后, 在onInvoke中安全退出.
         */
        virtual void        exit()  { _exit = true; }

        /**
         * @brief After thread start, invoke will run
         *      invoke函数将不断地循环执行, 只有当返回true时退出.
         *      onStart -> onInvoke-> ... ->onInvoke -> onFinish.
         */
        virtual void    onStart()   {}
        virtual bool    onInvoke() = 0;
        virtual void    onFinish()   {}
    };

    //----------------------------------------------------------------------------------------------------
    //!	线程池
    //----------------------------------------------------------------------------------------------------

    //--------------------------------
    //!  线程池工作模式
    typedef enum {
        ThreadPoolMode_Default,     //! 内部使用完成端口 (non-I/O)
        ThreadPoolMode_IOThread,    //! 使用 (I/O)工作线程, 处于警告状态, APC (使用non-IO工作线程更有效率)
        ThreadPoolMode_LongWork,    //! 回调函数的执行需要较长的时间时设置, 有助于系统决定是否创建新的thread
    } ThreadPoolMode;

    //--------------------------------
    //! 线程池
    class HYC_API ThreadPool : public Singleton<ThreadPool>
    {
        PRIVATE_CLASS_DECLARE;
    public:
        ThreadPool();
        ~ThreadPool();

        /**
         * @brief 添加新的工作项目到线程池
         *      工作项内部必须要自然终止.
         * @param pWorkItem 自然终止后由线程池内部释放.
         */
//         void QueueWorkItem( ThreadPoolWorkItemProc pWorkItem, void* param, ThreadPoolMode mode = ThreadPoolMode_Default );
        void QueueWorkItem( ThreadPoolWorkItem* pWorkItem, ThreadPoolMode mode = ThreadPoolMode_Default );
    };



} // end namespace hyc

#endif // end of define __BP_THREADPOOL_H_20110316180028__
#endif // HYC_COMPILE_MODULE_THREADPOOL
