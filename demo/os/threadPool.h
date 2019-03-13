//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_THREADPOOL_H_20130402214409__
#define __BP_THREADPOOL_H_20130402214409__

class WorkItem : ThreadPoolWorkItem
{
public:
    WorkItem() : _isExit(false) {}

    // 一定要加上这样的函数,保证线程能够自然终止.
    bool _isExit;
    void exit() { _isExit = true; }

    // 线程工作.
    virtual uinteger     onInvoke()
    {
        while (true) {

            if (_isExit)    break;

            // do something.
        }

        return 0;
    }
};

void os_threadPool()
{
    ThreadPool tp;
    ThreadPool::QueueWorkItem(new WorkItem(), ThreadPoolMode_LongWork);
}

#endif // end of define __BP_THREADPOOL_H_20130402214409__