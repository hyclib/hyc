//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc framework
// author:       lipengxiang
// description:  引用计数对象; 
//               引用计数对象使用规则: 1.使用指针; 2.使用前调用addRef; 3.使用后调用 release
//

#ifndef __BP_CNOBJECTPTR_H_20110707203718__
#define __BP_CNOBJECTPTR_H_20110707203718__

#include "../os/atomic.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	引用计数对象
    //----------------------------------------------------------------------------------------------------
    class HYC_API RefCount
    {
    public:
        RefCount() : _refCount_ref(0)    {}
        RefCount(const RefCount& r) : _refCount_ref(0) {}
        virtual ~RefCount()  {}

        RefCount& operator=(const RefCount&)  { return *this; }

        //! 不要手动调用.
        virtual void    addRef()    { assert(_refCount_ref >= 0); ++_refCount_ref; }
        virtual void    release()   { assert(_refCount_ref > 0); if (--_refCount_ref == 0) { onDestroy(); } }
        virtual integer getRef() const { return _refCount_ref; }

    protected:
        /**
         * @brief 销毁函数. 重写此函数可用于自定义的内存释放.
         */
        virtual void   onDestroy()  { delete this; }

    protected:
        mutable integer     _refCount_ref;
    };
     

    //----------------------------------------------------------------------------------------------------
    //*	指针引用模板
    //   - 指针模板自动会释放内存, 模板类型需继承至RefCount/RefCountSafe
    //   - 使用智能指针时, 尽量需使用ObjectPtr对象, 而不要直接调用RefCount对象自身的addRef/release,否则多线程可能出现问题.
    //----------------------------------------------------------------------------------------------------
    template<class T>
    class ObjectPtr
    {
    public:
        ObjectPtr( const T* refPtr = NULL ) : m_refPtr(NULL)    { if (refPtr) *this = refPtr; }
        ObjectPtr( const ObjectPtr& rhs ) : m_refPtr(NULL)      { *this = rhs; }
        template<class SubT>
        ObjectPtr( const SubT* refPtr ) : m_refPtr(NULL)  { if (refPtr) *this = refPtr; }
        template<class SubT>
        ObjectPtr( const ObjectPtr<SubT>& rhs ) : m_refPtr(NULL) { *this = rhs; }

        virtual ~ObjectPtr()                { release(); }

        // operator
        inline             operator bool() const                    { return !isEmpty(); }
        inline const T*    operator->() const                       { return m_refPtr; }
        inline T*          operator->()                             { return m_refPtr; }
        inline const T&    operator*() const                        { return *m_refPtr; }
        inline T&          operator*()                              { return *m_refPtr; }
        inline ObjectPtr&  operator=( const T* rhs );
        inline ObjectPtr&  operator=( const ObjectPtr& rhs );
        inline bool        operator==( const ObjectPtr& rhs ) const { return get() == rhs.get(); }
        inline bool        operator==( const T* rhs ) const         { return get() == rhs; }
        inline bool        operator!=( const ObjectPtr& rhs ) const { return !((*this)==rhs); }
        inline bool        operator!=( const T* rhs ) const         { return !((*this)==rhs); }

        template<class SubT>
        inline ObjectPtr&  operator=( const SubT* rhs )             { _lock(); _release(); SubT* tt = (SubT*)rhs; m_refPtr = static_cast<T*>(tt); if (m_refPtr) m_refPtr->addRef(); _unlock(); return (*this); }
        template<class SubT>
        inline ObjectPtr&  operator=( const ObjectPtr<SubT>& rhs )  { T* p; rhs._lock(); SubT* pp = (SubT*)rhs.get(); p = static_cast<T*>(pp); if (p) p->addRef(); rhs._unlock(); _lock(); _release(); m_refPtr = p; _unlock(); return (*this); }
        template<class SubT>
        inline bool        operator==( const ObjectPtr<SubT>& rhs ) const { return get() == (T*)rhs.get(); }
        template<class SubT>
        inline bool        operator==( const SubT* rhs ) const      { return get() == (T*)rhs; }
        template<class SubT>
        inline bool        operator!=( const ObjectPtr<SubT>& rhs ) const { return !((*this)==rhs); }
        template<class SubT>
        inline bool        operator!=( const SubT* rhs ) const      { return !((*this)==rhs); }

        /**
         * @brief 释放当前对象的引用.
         */
        inline void        release()   { _lock(); _release(); _unlock(); }
        
        /**
         * @brief 获得所引用的对象指针.
         */
        inline T*          get()       { return m_refPtr; }
        inline const T*    get() const { return m_refPtr; }
        
        /**
         * @brief 判断是否为没有引用任何对象.
         */
        inline bool        isEmpty() const { return 0 == get(); }

        // don't call.
        inline void        _release()        { if (m_refPtr){m_refPtr->release(); m_refPtr = NULL;} }
        inline void        _lock() const     { m_atomicSync.lockAll(); }
        inline void        _unlock() const   { m_atomicSync.unlockAll(); }

    protected:
        T*                          m_refPtr;
        mutable AtomicSyncObject    m_atomicSync;
    };

    //-----------------------------------------------
    template<class T>
    inline ObjectPtr<T>& hyc::ObjectPtr<T>::operator=( const T* rhs )
    { 
        _lock();
        _release();
        m_refPtr = (T*)rhs;
        if (m_refPtr) 
            m_refPtr->addRef();
        _unlock(); 
        return (*this); 
    }

    //-----------------------------------------------
    template<class T>
    inline ObjectPtr<T>& hyc::ObjectPtr<T>::operator=( const ObjectPtr<T>& rhs )
    {
        T* p;
        rhs._lock(); 
        p = (T*)rhs.m_refPtr;
        if (p) 
            p->addRef(); 
        rhs._unlock(); 

        _lock(); 
        _release(); 
        m_refPtr = p;        
        _unlock(); 
        return (*this);
    }



} // end namespace hyc

#endif // end of define __BP_CNOBJECTPTR_H_20110707203718__

