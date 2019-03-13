//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_CONDITIONVARIABLE_HPP_20120222173317__
#define __BP_CONDITIONVARIABLE_HPP_20120222173317__

#include "../../include/os/thread.h"

// Platform	                                High-order bit	Next 7 bits	    Low-order byte
// Windows NT 3.51	                        0	            Build number	3
// Windows NT 4.0	                        0	            Build number	4
// Windows 2000 or Windows XP	            0	            Build number	5
// Windows 95, Windows 98, or Windows Me	1	            Reserved	    4
// Win32s with Windows 3.1	                1	            Build number	3

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	Semaphore
    //----------------------------------------------------------------------------------------------------
    class Semaphore
    {
    public:

        Semaphore(long initial= 0)
        {
            _sem = CreateSemaphore(0, initial, 0x7fffffff, 0);
        }
        ~Semaphore()
        {
            if (_sem != INVALID_HANDLE_VALUE)
                CloseHandle(_sem);
        }

        void wait() const
        {
            DWORD rc = WaitForSingleObject(_sem, INFINITE);
//             if(rc != WAIT_OBJECT_0)
//             {
//             }
        }
        bool timedWait(integer timeout) const
        {
            if (_sem == INVALID_HANDLE_VALUE)   return false;
            DWORD rc = WaitForSingleObject(_sem, timeout);
            if(rc != WAIT_TIMEOUT && rc != WAIT_OBJECT_0)
            {
                return false;
            }
            return rc != WAIT_TIMEOUT;
        }
        void post(int count = 1) const
        {
            if (_sem == INVALID_HANDLE_VALUE)   return;
            ReleaseSemaphore(_sem, count, 0);
        }
    private:
        mutable HANDLE _sem;
    };

    //----------------------------------------------------------------------------------------------------
    //!	接口
    //----------------------------------------------------------------------------------------------------
    class __IConditionVariable : public RefCount
    {
    public:
        virtual ~__IConditionVariable()    {}

        //! 等待信号; sync 是用来对cv进行安全锁的
        virtual void    wait( SyncObject& sync ) = 0;

        //! 超时等待, 当超时还未等待信号时, 返回false; sync 是用来对cv进行安全锁的
        virtual bool    waitTimeout( SyncObject& sync, integer timeout ) = 0;

        //! 发送信号
        virtual void    signal() = 0;

        //! 唤醒全部等待信号的线程
        virtual void    broadcast() = 0;
    };

    //----------------------------------------------------------------------------------------------------
    //!	xp
    //----------------------------------------------------------------------------------------------------
    class __XpConditionVariable : public __IConditionVariable
    {
    public:
        enum State
        {
            StateIdle,
            StateSignal,
            StateBroadcast
        };
        mutable State   _state;
        Semaphore       _queue;
        Semaphore       _gate;
        mutable long    _blocked;
        mutable long    _unblocked;
        mutable SyncObject  _sync;
    public:
        __XpConditionVariable()
            :_queue(0),
            _gate(1),
            _blocked(0),
            _unblocked(0),
            _state(StateIdle)
        {

        }

        //-----------------------------------------------
        void wake(bool isBroadcast)
        {
            //
            // Lock gate. The gate will be locked if there are threads waiting
            // to drain from postWait.
            //
            _gate.wait();

            //
            // Lock the internal mutex.
            //
            _sync.lock();

            //
            // Adjust the count of the number of waiting/blocked threads.
            //
            if(_unblocked != 0)
            {
                _blocked -= _unblocked;
                _unblocked = 0;
            }

            //
            // If there are waiting threads then we enter a signal or
            // broadcast state.
            //
            if(_blocked > 0)
            {
                //
                // Unblock some number of waiters. We use -1 for the signal
                // case.
                //
                assert(_state == StateIdle);
                _state = (isBroadcast) ? StateBroadcast : StateSignal;
                //
                // Posting the queue wakes a single waiting thread. After this
                // occurs the waiting thread will wake and then either post on
                // the _queue to wake the next waiting thread, or post on the
                // gate to permit more signaling to proceed.
                //
                // Release before posting to avoid potential immediate
                // context switch due to the mutex being locked.
                //
                _sync.unlock();
                _queue.post();
            }
            else
            {
                //
                // Otherwise no blocked waiters, release the gate.
                //
                // Release before posting to avoid potential immediate
                // context switch due to the mutex being locked.
                //
                _sync.unlock();
                _gate.post();
            }
        }

        //-----------------------------------------------
        void preWait() const
        {
            //
            // _gate is used to protect _blocked. Furthermore, this prevents
            // further threads from entering the wait state while a
            // signal/broadcast is being processed.
            //
            _gate.wait();
            ++_blocked;
            _gate.post();
        }

        //-----------------------------------------------
        void dowait(  ) const
        {
            _queue.wait();
            postWait(false);
        }

        //-----------------------------------------------
        bool timedDowait( integer timeout) const
        {
            bool rc = _queue.timedWait(timeout);
            postWait(!rc);
            return rc;
        }

        //-----------------------------------------------
        void postWait(bool timedOutOrFailed) const
        {
            _sync.lock();

            //
            // One more thread has unblocked.
            //
            ++_unblocked;

            //
            // If _state is StateIdle then this must be a timeout, otherwise its a
            // spurious wakeup which is incorrect.
            //
            if(StateIdle == _state)
            {
                assert(timedOutOrFailed);
                _sync.unlock();
                return;
            }

            if(timedOutOrFailed)
            {
                //
                // If the thread was the last blocked thread and there's a
                // pending signal/broadcast, reset the signal/broadcast to
                // prevent spurious wakeup.
                //
                if(_blocked == _unblocked)
                {
                    _state = StateIdle;
                    //
                    // Consume the queue post to prevent spurious wakeup. Note
                    // that although the internal mutex could be released
                    // prior to this wait() call, doing so gains nothing since
                    // this wait() MUST return immediately (if it does not
                    // there is a major bug and the entire application will
                    // deadlock).
                    //
                    _queue.wait();
                    //
                    // Release before posting to avoid potential immediate
                    // context switch due to the mutex being locked.
                    //
                    _sync.unlock();
                    _gate.post();
                }
            }
            else
            {
                //
                // At this point, the thread must have been woken up because
                // of a signal/broadcast.
                //
                if(_state == StateSignal || _blocked == _unblocked) // Signal or no more blocked threads
                {
                    _state = StateIdle;
                    // Release before posting to avoid potential immediate
                    // context switch due to the mutex being locked.
                    _sync.unlock();
                    _gate.post();
                }
                else // Broadcast and more blocked threads to wake up.
                {
                    // Release before posting to avoid potential immediate
                    // context switch due to the mutex being locked.
                    _sync.unlock();
                    _queue.post();
                }
            }

        }

        //! 等待信号; sync 是用来对cv进行安全锁的
        virtual void    wait( SyncObject& sync )
        {
            preWait();
            sync.unlock();

            dowait();
            sync.lock();
        }

        //! 超时等待, 当超时还未等待信号时, 返回false; sync 是用来对cv进行安全锁的
        virtual bool    waitTimeout( SyncObject& sync, integer timeout )
        {
            preWait();
            sync.unlock();

            bool rc = timedDowait(timeout);
            sync.lock();
            return rc;
        }

        //! 发送信号
        virtual void    signal()
        {
            wake(false);
        }

        //! 唤醒全部等待信号的线程
        virtual void    broadcast()
        {
            wake(true);
        }
    };

    ////----------------------------------------------------------------------------------------------------
    ////!	Vista & 7
    ////----------------------------------------------------------------------------------------------------
    //class __VistaConditionVariable : public __IConditionVariable
    //{
    //public:

    //    
    //public:
    //    __VistaConditionVariable()
    //        :_gate(1),
    //        _blocked(0),
    //        _unblocked(0),
    //        _state(StateIdle)
    //    {

    //    }



    //    //-----------------------------------------------
    //    bool timedDowait(integer timeout) const
    //    {
    //        bool rc = _queue.timedWait(timeout);
    //        postWait(!rc);
    //        return rc;
    //    }

    //    //! 等待信号; sync 是用来对cv进行安全锁的
    //    virtual void    wait()
    //    {
    //        preWait();
    //        _sync.unlock();

    //        dowait();
    //        _sync.lock();
    //    }

    //    //! 超时等待, 当超时还未等待信号时, 返回false; sync 是用来对cv进行安全锁的
    //    virtual bool    waitTimeout( integer timeout )
    //    {
    //        preWait();
    //        _sync.unlock();

    //        bool rc = timedDowait(timeout);
    //        _sync.lock();
    //        return rc;
    //    }

    //    //! 发送信号
    //    virtual void    signal()
    //    {
    //        wake(false);
    //    }

    //    //! 唤醒全部等待信号的线程
    //    virtual void    broadcast()
    //    {
    //        wake(true);
    //    }
    //};


    //----------------------------------------------------------------------------------------------------
    //!	ConditionVariable::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class ConditionVariable::PrivateClass
    {
    public:
        __XpConditionVariable   _cv;
    };

    //----------------------------------------------------------------------------------------------------
    //!	ConditionVariable
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    ConditionVariable::ConditionVariable()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    //-----------------------------------------------
    ConditionVariable::~ConditionVariable()
    {
        PRIVATE_CLASS_DESTROY;
    }
    
    //-----------------------------------------------
    void ConditionVariable::wait( SyncObject& sync )
    {
        PRIVATE_CLASS_GET(p);
        p->_cv.wait(sync);
    }

    //-----------------------------------------------
    bool ConditionVariable::waitTimeout( SyncObject& sync, s32 timeout )
    {
        PRIVATE_CLASS_GET(p);
        return p->_cv.waitTimeout(sync, timeout);
    }

    //-----------------------------------------------
    void ConditionVariable::signal()
    {
        PRIVATE_CLASS_GET(p);
        p->_cv.signal();
    }

    //-----------------------------------------------
    void ConditionVariable::broadcast()
    {
        PRIVATE_CLASS_GET(p);
        p->_cv.broadcast();
    }

} // end namespace hyc

#endif // end of define __BP_CONDITIONVARIABLE_HPP_20120222173317__

