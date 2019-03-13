//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hyc test
// author:       lipengxiang
// description:  
//

#ifndef __BP_OBJECTPTR_H_20140617103543__
#define __BP_OBJECTPTR_H_20140617103543__


class CommPtrObj : public RefCountSafe
{
public:
    
};

ObjectPtr<CommPtrObj>   objPtr;

class CommPtrThread1 : public Thread
{
public:
    bool onInvoke(void* lpParam)
    {
        objPtr->addRef();

        Sleep(10);

        objPtr->release();

        return true;
    }
};

class CommPtrThread2 : public Thread
{
public:
    bool onInvoke(void*)
    {
        ObjectPtr<CommPtrObj> ptr = objPtr;
        
        Sleep(10);

        return true;
    }
};

void common_objectPtr()
{
    print_all_line(_T("\
                      ---------------------------------------------\n\
                      objectPtr start\n\
                      ---------------------------------------------\n"));

    objPtr = new CommPtrObj;    // new出来的对象, 随智能指针对象的析构而释放, 无需手动delete.

    Array<ThreadPtr>    arr;

    for (u32 i = 0; i < 1000; i++)
    {
        arr.push_back(new CommPtrThread1);
    }

    for (u32 i = 0; i < arr.size(); i++)
    {
        arr[i]->start();
    }

    for (u32 i = 0; i < arr.size(); i++)
    {
        arr[i]->join();
    }

    print_all_line(_T("%d"), objPtr->getRef());
    hy_assertTrue(objPtr->getRef() == 1);

    print_all_line(_T("\
                      ---------------------------------------------\n\
                      objectPtr end\n\
                      ---------------------------------------------\n"));
}

#endif // end of define __BP_OBJECTPTR_H_20140617103543__
