//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyc
// author:       lipengxiang
// description:  
//

#pragma once

#include <vcclr.h>
#using <mscorlib.dll>

#include "../_Common/cnSingleton.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Hy
{
namespace OS
{
    //----------------------------------------------------------------------------------------------------
    //*	线程池工作项
    //----------------------------------------------------------------------------------------------------

    //--------------------------------
    //  线程池工作项
    public interface class IThreadPoolWorkItem
    {
    public:
        /*---------------------------
        After thread start, invoke will run
        !注意: 不要在回调中手动终止线程(CurThreadAbort/abort), 需让其自然终止
        return: in normal conditions, 1 is failure, 0 is success
        ---------------------------*/
        bool     onInvoke();
    };

    namespace internal
    {
        class __ThreadPoolWorkItem : public hyc::ThreadPoolWorkItem
        {
            gcroot<IThreadPoolWorkItem^>  _listener;
        public:
            __ThreadPoolWorkItem()  {}

            void setListener( IThreadPoolWorkItem^ lis ) { _listener = lis; assert(nullptr != lis); }
            IThreadPoolWorkItem^    getListener()   { return _listener; }

            virtual void    onStart()   {}
            virtual bool    onInvoke();
            virtual void    onFinish()   {}

        };
    }

    //----------------------------------------------------------------------------------------------------
    //*	线程池
    //----------------------------------------------------------------------------------------------------

    //--------------------------------
    //  线程池工作模式
    public enum class ThreadPoolMode
    {
        ThreadPoolMode_Default,     // 内部使用完成端口 (non-I/O)
        ThreadPoolMode_IOThread,    // 使用 (I/O)工作线程, 处于警告状态, APC (使用non-IO工作线程更有效率)
        ThreadPoolMode_LongWork,    // 回调函数的执行需要较长的时间时设置, 有助于系统决定是否创建新的thread
    };

    //--------------------------------
    public ref class ThreadPool : Singleton<ThreadPool^>
    {
        hyc::ThreadPool*    _ptr;
    public:
        ThreadPool()    {   _ptr = new hyc::ThreadPool(); }
        ~ThreadPool()   {   this->!ThreadPool();}
        !ThreadPool()   {   SAFE_DELETE(_ptr);  }

        /*---------------------------
            添加新的工作项目到线程池
        ---------------------------*/
        // void QueueWorkItem( ThreadPoolWorkItemProc pWorkItem, void* param, ThreadPoolMode mode = ThreadPoolMode_Default );
        void QueueWorkItem( IThreadPoolWorkItem^ pWorkItem, ThreadPoolMode mode/* = ThreadPoolMode::ThreadPoolMode_Default*/ )
        {
            if (nullptr != pWorkItem)
            {
                internal::__ThreadPoolWorkItem* pItem = new internal::__ThreadPoolWorkItem();
                pItem->setListener(pWorkItem);
                _ptr->QueueWorkItem( pItem, (hyc::ThreadPoolMode)mode );
            }
        }
    };

} // end namespace OS
} // end namespace Hy
