//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc framework
// author:       lipengxiang
// description:  智能引用对象;
//                自动管理内容
//

#ifndef __BP_CNOBJECTREF_H_20110322172525__
#define __BP_CNOBJECTREF_H_20110322172525__


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	引用对象模板; 需在线程中使用, 建议用 RefCountSafe
    //   - 相较于ObjectPtr, 更加灵活, 可以自己管理 addRef/release; 无需手动管理
    //----------------------------------------------------------------------------------------------------
    template<class T>
    class ObjectRef
    {
    public:
        explicit ObjectRef( T* refPtr = 0 ) { m_refPtr = refPtr; m_pRefCount = new (uinteger)(1); assert(m_pRefCount); }
        ObjectRef( const ObjectRef& rhs )   { m_pRefCount = 0; rhs._inc((*this)); }
        virtual ~ObjectRef()                { release(); }

        // operator
                    operator bool() const                    { return !isEmpty(); }
        const T*    operator->() const                       { return m_refPtr; }
        T*          operator->()                             { return m_refPtr; }
        const T&    operator*() const                        { return *m_refPtr; }
        T&          operator*()                              { return *m_refPtr; }
        ObjectRef&  operator=( const ObjectRef& rhs )        { rhs._inc((*this)); return (*this); }
        bool        operator==( const ObjectRef& rhs ) const { return get() == rhs.get(); }
        bool        operator!=( const ObjectRef& rhs ) const { return !((*this)==rhs); }

        /**
         * @brief 释放当前对象的引用.
         */
        void        release()   { _dec(); }
        
        /**
         * @brief 获得所引用的对象指针.
         */
        T*          get()       { return m_refPtr; }
        const T*    get() const { return m_refPtr; }
        
        /**
         * @brief 判断是否为没有引用任何对象.
         */
        bool        isEmpty() const { return 0 == get(); }

    protected:
        virtual void _inc( ObjectRef& ref ) const {
            ref.release();
            ref.m_refPtr = m_refPtr;
            ref.m_pRefCount = m_pRefCount;
            ++(*m_pRefCount);
        }
        virtual void _dec() const {
            if (!m_pRefCount)   return;
            --(*m_pRefCount);
            if (0 == (*m_pRefCount)) {
                SAFE_DELETE(m_refPtr);
                SAFE_DELETE(m_pRefCount);
            }
        }

        mutable uinteger*    m_pRefCount;    
        mutable T*      m_refPtr;
    };

    //----------------------------------------------------------------------------------------------------
    //*	安全引用对象模板; 数组方式删除
    //----------------------------------------------------------------------------------------------------
    template<class T>
    class ObjectArrayRef : public ObjectRef<T>
    {
    public:
        explicit ObjectArrayRef( T* refPtr = 0 ) : ObjectRef<T>(refPtr) { }

    protected:
        virtual void _dec() const {
            if (!ObjectRef<T>::m_pRefCount)   return;
            --(*ObjectRef<T>::m_pRefCount);
            if (0 == (*ObjectRef<T>::m_pRefCount)) {
                SAFE_DELETE_ARRAY(ObjectRef<T>::m_refPtr);
                SAFE_DELETE(ObjectRef<T>::m_pRefCount);
            }
        }
    };

     
     

} // end namespace hyc

#endif // end of define __BP_CNOBJECTREF_H_20110322172525__