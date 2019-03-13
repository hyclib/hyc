//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hyc test
// author:       lipengxiang
// description:  
//

#ifndef __BP_QUEUE_H_20140616152611__
#define __BP_QUEUE_H_20140616152611__

hyc::concurrent::ConcurrentQueue<u32> con_que;
AtomicBase<u32>     con_num = 0;
#define CON_NUM1    1000
#define CON_NUM2    1000

class ConProductThread1 : public Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        for (u32 i = 0; i < CON_NUM1; i++)
        {
            //Sleep(0);
            con_que.push_back(i);
        }
        return true;
    }
};

class ConProductThread2 : public Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        for (u32 i = CON_NUM1; i < CON_NUM1+CON_NUM1; i++)
        {
            //Sleep(0);
            con_que.push_back(i);
        }
        return true;
    }
};

class ConConsumeThread3 : public Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        u32 i = 0;
        while (true)
        {
            if (con_que.pop_front(i))
            {
                ++con_num;
                print_all_line(_T("con_que %d %d"), getThreadId(), i);
            }

            if (con_num >= (u32)(CON_NUM1+CON_NUM2))
                break;
        }

        return true;
    }
};

void concurrent_queue()
{
    print_all_line(_T("\
                      --------------------------------------------------------\n\
                      concurrent queue start\n\
                      --------------------------------------------------------\n\
                      "));


    ConProductThread1 thread1;
    ConProductThread2 thread2;
    ConConsumeThread3 thread3, thread4, thread5;
    thread1.start();
    thread2.start();
    thread3.start();
    thread4.start();
    thread5.start();

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    print_all_line(_T("\
                      --------------------------------------------------------\n\
                      concurrent queue finish\n\
                      --------------------------------------------------------\n\
                      "));
};

#endif // end of define __BP_QUEUE_H_20140616152611__