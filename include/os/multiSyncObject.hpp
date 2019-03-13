//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_MULTISYNCOBJECT_HPP_20130517103724__
#define __BP_MULTISYNCOBJECT_HPP_20130517103724__

#include "thread.h"
#include "conditionVariable.h"

namespace hyc
{
	/**
	 * 多重同步对象; 使用于以下的情景:
     *     a/b/c..: 为使用者, 进行attach/unattach操作.每次锁定将引用计数+1, 每次释放锁将引用计数-1;
     *     o:       为中心, 进行wait操作, 只有当引用计数为0时(没有锁时), 继续操作, 否则等待.
     *     ---------------------------------------------
     *      a   b   c   d   e   ....    n
     *      \   \   |   |   |           /
     *       \   \  |   |   |          /
     *        \   \ |   |   |         /
     *                  |
     *                  o
     *     此结构可用于服务器容器的编程, 如: 并行分配多个对象, 等所有的对象工作都完成了, 才做最终的工作.
	 */
    class MultiSyncObject : public INoncopyable, public MemoryAllocObject, public RefCount {
    public:
        MultiSyncObject();
        ~MultiSyncObject();

        /** 
         * @brief 由使用者调用.
         */
        void            attach();
        void            unattach();

        /**
         * @brief 由中心调用.
         * @return 令牌对象保证多线程不冲突.
         */
        SyncTokenPtr    wait();

        /**
         * @brief 由中心调用. 超时等待, 当超时还未等待信号时, 返回false;
         */
        bool            waitTimeout( s32 timeout );

    private:
        SyncObject          _sync;
        ConditionVariable   _cv;
        u32                 _ref;
    };


    inline MultiSyncObject::MultiSyncObject() : _ref(0) {
    }

    inline MultiSyncObject::~MultiSyncObject() {
        _cv.broadcast();    // prevent to dead-lock.
    }

    inline void MultiSyncObject::attach() {
        _sync.lock();
        {
            _ref++;
        }
        _sync.unlock();
    }

    inline void MultiSyncObject::unattach() {
        _sync.lock();
        {
            if (0 == _ref)
            {
                error_all(_T("Error Call"));
                throw ExceptionType_Forbid;
            }
            if (--_ref == 0)    _cv.broadcast();
        }
        _sync.unlock();
    }

    inline SyncTokenPtr MultiSyncObject::wait() {
        _sync.lock();
        {
            if (0 == _ref)
            {
                SyncTokenPtr token = _sync.getToken();
                _sync.unlock();
                return token;
            }

            _cv.wait(_sync);
        }
        SyncTokenPtr token = _sync.getToken();
        _sync.unlock();
        return token;
    }

    inline bool MultiSyncObject::waitTimeout( s32 timeout ) {
        _sync.lock();
        {
            if (0 == _ref)
            {
                SyncTokenPtr token = _sync.getToken();
                _sync.unlock();
                return token;
            }

            _cv.waitTimeout(_sync, timeout);
        }
        SyncTokenPtr token = _sync.getToken();
        _sync.unlock();
        return token;
    }
} // end namespace hyc

#endif // end of define __BP_MULTISYNCOBJECT_HPP_20130517103724__