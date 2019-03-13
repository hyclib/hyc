
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// create at:    2014-04-16 10:44:36
// description:  
//

#ifndef __BP_QUEUE_HPP_20140416104436__
#define __BP_QUEUE_HPP_20140416104436__

#include "../../os/atomic.h"
#include "core/alloc.hpp"
#include "core/node.hpp"

namespace hyc
{
namespace concurrent
{

    /**
     * 并发队列.
     * !!! 析构函数发生时需保证没有任何线程对此对象做其他操作.
     */
    template< class T, class Elem = Node<T>, class alloc = hyc::concurrent::Allocator<Elem> >
    class ConcurrentQueue
    {
    public:
        ConcurrentQueue();
        ~ConcurrentQueue();

        /**
         * 返回值表明插入的元素是否是第一个元素.
         */
        bool    push_front( const T& t );
        bool    push_back( const T& t );

        /**
         * 如果存在数据则将值赋给t,并返回true.
         */
        bool    pop_front( OUT T& t );

        /**
         * 获取顶部元素, 存在数据则将值赋给t,并返回true.
         */
        bool    top_front( OUT T& t );

        /**
         * 判断队列是否为空.
         */
        bool    empty() const;

    private:
        Elem*   pHead;
        Elem*   pTail;
        mutable AtomicSyncObject    sync;
    };


    //-----------------------------------------------
    template<class T, class Elem, class alloc>
    inline ConcurrentQueue<T,Elem,alloc>::ConcurrentQueue()
    {
    	pTail = pHead = alloc::allocate(1);
        new (pHead)Elem();
    }

    //-----------------------------------------------
    template<class T, class Elem, class alloc>
    inline ConcurrentQueue<T,Elem,alloc>::~ConcurrentQueue()
    {
        sync.lockAll();
        {
            Elem* p = pHead;
            while (p) {
                Elem* t = p;
                p = p->pNext;
                t->Elem::~Elem();
                alloc::deallocate(t, 1);
            }
            pHead = pTail = NULL;
        }
        sync.unlockAll();
    }

    //-----------------------------------------------
    template<class T, class Elem, class alloc>
    inline bool ConcurrentQueue<T,Elem,alloc>::push_back( const T& t )
    {
        bool r = false;
        Elem* p = alloc::allocate(1);
        new (p)Elem(t, NULL);

        sync.lockAll();
        {
            //r = empty();
            r = (pHead == pTail);
            pTail->insertNext(p);
            pTail = p;
        }
        sync.unlockAll();
        return r;
    }


    //-----------------------------------------------
    template<class T, class Elem, class alloc>
    inline bool ConcurrentQueue<T,Elem,alloc>::push_front( const T& t )
    {
        bool r = false;
        Elem* p = alloc::allocate(1);
        new (p)Elem(t, NULL);

        sync.lockAll();
        {
            //r = empty();
            r = (pHead == pTail);
            pHead->insertNext(p);
            if (r)
            {
            	pTail = p;
            }
        }
        sync.unlockAll();
        return r;
    }

    //-----------------------------------------------
    template<class T, class Elem, class alloc>
    inline bool ConcurrentQueue<T,Elem,alloc>::pop_front( OUT T& t )
    {
        bool r = false;
        Elem* p = NULL;
        sync.lockAll();
        {
            if (pHead != pTail)
            {
                assert(pHead->pNext);
                t = pHead->pNext->value;
                r = true;

                p = pHead->linkNext();
                if (!pHead->pNext)
                    pTail = pHead;
            }
        }
        sync.unlockAll();

        if (p)
        {
        	p->Elem::~Elem();
        	alloc::deallocate(p, 1);
        }

        return r;
    }


    //-----------------------------------------------
    template< class T, class Elem /*= Node<T>*/, class alloc /*= hyc::concurrent::Allocator<Elem> */>
    inline bool hyc::concurrent::ConcurrentQueue<T, Elem, alloc>::top_front(OUT T& t)
    {
        bool r = false;
        sync.lockAll();
        {
            if (pHead != pTail)
            {
                assert(pHead->pNext);
                t = pHead->pNext->value;
                r = true;
            }
        }
        sync.unlockAll();

        return r;
    }

    //-----------------------------------------------
    template<class T, class Elem, class alloc>
    inline bool ConcurrentQueue<T,Elem,alloc>::empty() const
    {
        bool r;
        sync.lockAll();
        {
            r = (pHead == pTail);
        }
        sync.unlockAll();
        return r;
    }

} // end namespace concurrent
} // end namespace hyc

#endif // end of define __BP_QUEUE_HPP_20140416104436__
