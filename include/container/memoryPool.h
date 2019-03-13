//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  内存池; 使用内存池可以减少内存的频繁创建删除
//

#ifndef __BP_MEMORYPOOL_H_20110506220951__
#define __BP_MEMORYPOOL_H_20110506220951__

#include "memoryAlloc.h"
#include <stack>

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	MemoryPool 非线程安全
    //----------------------------------------------------------------------------------------------------

    template<class T>
    class MemoryPool
    {
        typedef std::stack<T*>   PtrStack;
        PtrStack    m_stackItems;
    public:
        MemoryPool()    { }
        virtual ~MemoryPool()   { clear(); }
    public:
        //! 创建一个新内存项, 使用完后使用 recycle回收
        //!  - 被创建的对象必须有默认构造函数
        virtual T*   create()           { T* ptr = NULL; if (m_stackItems.empty()) { ptr = (T*)MemoryAlloc::Malloc( sizeof(T) ); } else { ptr = m_stackItems.top(); m_stackItems.pop(); } MemoryAlloc::Initialize( *ptr ); return ptr; }

        //! 回收使用完的内存
        virtual void recycle( T* ptr )  { if (ptr) { MemoryAlloc::Destory( *ptr ); m_stackItems.push( ptr ); } }

        //! 清除内存
        virtual void clear()            { while ( !m_stackItems.empty() ) { MemoryAlloc::Free( m_stackItems.top() ); m_stackItems.pop(); } }
    };

    //----------------------------------------------------------------------------------------------------
    //!	MemoryPoolSafe 线程安全
    //----------------------------------------------------------------------------------------------------

    template<class T>
    class MemoryPoolSafe : public MemoryPool<T>
    {
        SyncObject  m_sync;
    public:
        MemoryPoolSafe()         { }
        virtual ~MemoryPoolSafe(){ clear(); }
    public:
        //! 创建一个新内存项, 使用完后使用 recycle回收
        //!  - 被创建的对象必须有默认构造函数
        virtual T*   create()           { SyncTokenPtr token = m_sync.getToken(); T* ptr = MemoryPool<T>::create(); return ptr; }

        //! 回收使用完的内存
        virtual void recycle( T* ptr )  { SyncTokenPtr token = m_sync.getToken(); MemoryPool<T>::recycle(ptr); }

        //! 清除内存
        virtual void clear()            { SyncTokenPtr token = m_sync.getToken(); MemoryPool<T>::clear(); }
    };

    //----------------------------------------------------------------------------------------------------
    //!	MemoryPoolSafe 线程安全, 使用外部的锁
    //----------------------------------------------------------------------------------------------------

    template<class T>
    class MemoryPoolSafe2 : public MemoryPool<T>
    {
        SyncObject&  m_sync;
    public:
        MemoryPoolSafe2( SyncObject& sync ) : m_sync(sync)  { }
        virtual ~MemoryPoolSafe2(){ clear(); }
    public:
        //! 创建一个新内存项, 使用完后使用 recycle回收
        //!  - 被创建的对象必须有默认构造函数
        virtual T*   create()           { SyncTokenPtr token = m_sync.getToken(); T* ptr = MemoryPool<T>::create(); return ptr; }

        //! 回收使用完的内存
        virtual void recycle( T* ptr )  { SyncTokenPtr token = m_sync.getToken(); MemoryPool<T>::recycle(ptr); }

        //! 清除内存
        virtual void clear()            { SyncTokenPtr token = m_sync.getToken(); MemoryPool<T>::clear(); }
    };

} // end namespace hyc

#endif // end of define __BP_MEMORYPOOL_H_20110506220951__
