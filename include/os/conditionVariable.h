//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_CONDITIONVARIABLE_H_20120222172343__
#define __BP_CONDITIONVARIABLE_H_20120222172343__

#include "thread.h"
 
namespace hyc
{
	//----------------------------------------------------------------------------------------------------
	//!	条件变量
    //! \example
    //!     ConditionVariable cv;
    //!     SyncObject sync;
    //!     sync.lock();
    //!     cv.wait(sync);
    //!     sync.unlock();
	//----------------------------------------------------------------------------------------------------
    class HYC_API ConditionVariable : public INoncopyable, public RefCount
    {
        PRIVATE_CLASS_DECLARE
    public:
        ConditionVariable();
        virtual ~ConditionVariable();
        
        /**
         * @brief 等待信号; sync 是用来对cv进行安全锁的
         *        执行wait之前需由本线程对sync锁定, 以防止线程竞争, 在队列更新后会解锁; 在退出之前会先锁定sync, 然后在退出之后由本线程解锁
         */
        void    wait( SyncObject& sync );

        /**
         * @brief 超时等待, 当超时还未等待信号时, 返回false; sync 是用来对cv进行安全锁的
         * 执行wait之前需由本线程对sync锁定, 以防止线程竞争, 在队列更新后会解锁; 在退出之前会先锁定sync, 然后在退出之后由本线程解锁
         */
        bool    waitTimeout( SyncObject& sync, s32 timeout );

        /**
         * @brief 发送信号
         */
        void    signal();

        /**
         * @brief 唤醒全部等待信号的线程
         */
        void    broadcast();
    };
    typedef ObjectPtr<ConditionVariable>    ConditionVariablePtr;
	 

} // end namespace hyc

#endif // end of define __BP_CONDITIONVARIABLE_H_20120222172343__

