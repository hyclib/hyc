//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc framework
// author:       lipengxiang
// description:  
//

#ifndef __BP_THREADPOOL_HPP_20110317113422__
#define __BP_THREADPOOL_HPP_20110317113422__


namespace hyc
{
    //--------------------------------
    DWORD WINAPI __ThreadPoolWorkItem( LPVOID param )
    {
        uinteger rt = -1;
        if (param)
        {
            hyc::ThreadPoolWorkItem* pWorkItem = (hyc::ThreadPoolWorkItem*)param;
            pWorkItem->onStart();

            while (true)
            {
                if (pWorkItem->isExit() || pWorkItem->onInvoke())
                    break;
            }

            pWorkItem->onFinish();

            pWorkItem->getCVJoin().broadcast();

            pWorkItem->release();
        }

        return rt;
    }

    //----------------------------------------------------------------------------------------------------
    //*	ThreadPool::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class ThreadPool::PrivateClass
    {
    public:
        static ULONG _getMode(ThreadPoolMode mode)
        {
            ULONG _m = WT_EXECUTEDEFAULT;
            switch (mode)
            {
            case ThreadPoolMode_IOThread:
                _m = WT_EXECUTEINIOTHREAD;
                break;
            case ThreadPoolMode_LongWork:
                _m = WT_EXECUTEINLONGTHREAD;
                break;
            }
            return _m;
        }
    };

    //----------------------------------------------------------------------------------------------------
    //*	ThreadPool
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    ThreadPool::ThreadPool()
    {
        m_pPrivateClass = NULL;
        //PRIVATE_CLASS_CREATE;
    }

    //-----------------------------------------------
    ThreadPool::~ThreadPool()
    {
        //PRIVATE_CLASS_DESTROY;
    }

//     //-----------------------------------------------
//     void ThreadPool::QueueWorkItem( ThreadPoolWorkItemProc pWorkItem, void* param, ThreadPoolMode mode /*= ThreadPoolMode_Default */ )
//     {
//         if (pWorkItem)
//         {
//             ::QueueUserWorkItem( pWorkItem, param, ThreadPool::PrivateClass::_getMode(mode) );
//         }
//     }

    //-----------------------------------------------
    void ThreadPool::QueueWorkItem( ThreadPoolWorkItem* pWorkItem, ThreadPoolMode mode /*= ThreadPoolMode_Default */ )
    {
        if (pWorkItem)
        {
            pWorkItem->addRef();
            ::QueueUserWorkItem( __ThreadPoolWorkItem, pWorkItem, ThreadPool::PrivateClass::_getMode(mode) );
        }

    }


} // end namespace hyc

#endif // end of define __BP_THREADPOOL_HPP_20110317113422__