//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// YunFramework Gateway
// author:       lipengxiang
// description:  
//
#if defined( HYC_COMPILE_MODULE_THREAD)

#ifndef __BP_ARRAYSAFE_H_20110906102039__
#define __BP_ARRAYSAFE_H_20110906102039__

#include "array.h"
#include "../os/thread.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	线程安全数组声明.
    //----------------------------------------------------------------------------------------------------
    template<typename T>
    class ArraySafe : public Array<T>
    {
    public:
        ArraySafe( u32 memLen = 0 ) : Array<T>(memLen) {}
        ArraySafe( const ArraySafe& arr ) : Array<T>(arr) {}

        //-----------------------------------------------
        //  操作
        void        operator=( const ArraySafe& arr );
        T&          operator[]( u32 index );
        const T&    operator[]( u32 index ) const;
        ArraySafe   operator+( const ArraySafe& arr );        
        ArraySafe&  operator+=( const ArraySafe& arr );
        
        void        clear()                         { _sync.lock(); Array<T>::clear(); _sync.unlock(); }
        void        eraseByIndex( u32 index )        { _sync.lock(); Array<T>::eraseByIndex(index); _sync.unlock(); }
        void        insert( u32 index, const T& val ){ _sync.lock(); Array<T>::insert(index, val); _sync.unlock(); }
        void        push_back( const T& val )       { _sync.lock(); Array<T>::push_back(val); _sync.unlock(); }
        void        reserve( u32 reserveSize ) { _sync.lock(); Array<T>::reserve(reserveSize); _sync.unlock(); }
        void        resize( u32 newsize )      { _sync.lock(); Array<T>::resize(newsize); _sync.unlock(); }
    protected:
        mutable SyncObject  _sync;
    };

    //----------------------------------------------------------------------------------------------------
    //*	线程安全数组声明. 包含find方法
    //! Use ArraySafeFind, T must implement these method:
    //!    1. T::T();
    //!    2. T::T( const T& );
    //!    3. bool T::operator==( const T& ) const;
    //----------------------------------------------------------------------------------------------------
    template<typename T>
    class ArrayFindSafe : public ArraySafe<T>
    {
    public:
        ArrayFindSafe( u32 memLen = 0 ) : ArraySafe<T>(memLen) {}
        ArrayFindSafe( const ArrayFindSafe& arr ) : ArraySafe<T>(arr) {}
    
        //! 遍历查找
        virtual s32     find( const T& e ) const;
        virtual s32     find( typename ArrayFind<T>::PredicateCallback cb ) const;
    };
    
    //-----------------------------------------------
    template<typename T>
    inline void ArraySafe<T>::operator=( const ArraySafe& arr )   
    {
        _sync.lock();
        (*((Array<T>*)this)) = *((const Array<T>*)&arr);
        _sync.unlock();
    }
    
    //-----------------------------------------------
    template<typename T>
    inline T& ArraySafe<T>::operator[]( u32 index )         
    {
        SyncTokenPtr token = _sync.getToken();
        return Array<T>::m_pMem[index];
    }
    
    //-----------------------------------------------
    template<typename T>
    inline const T& ArraySafe<T>::operator[]( u32 index ) const  
    { 
        SyncTokenPtr token = _sync.getToken();
        return Array<T>::m_pMem[index];
    }
    
    
    //-----------------------------------------------
    template<typename T>
    inline ArraySafe<T> ArraySafe<T>::operator+( const ArraySafe& arr )
    {
        _sync.lock();
        ArraySafe other = *this;
        _sync.unlock();
        other += arr;
        return other;
    }
    
    //-----------------------------------------------
    template<typename T>
    inline ArraySafe<T>& ArraySafe<T>::operator+=( const ArraySafe& arr )
    { 
        SyncTokenPtr token = _sync.getToken();
        (*((Array<T>*)this)) += (*((const Array<T>*)&arr));
        return *this;
    }
    
    //-----------------------------------------------
    //! 遍历查找
    template<typename T1>
    inline s32 ArrayFindSafe<T1>::find( const T1& e ) const
    {
        SyncTokenPtr token = ArraySafe<T1>::_sync.getToken();
        if (ArraySafe<T1>::size() == 0)    return -1;
        for (u32 i = 0; i < ArraySafe<T1>::size(); ++i) {
            if (*this[i] == e) return i;
        }
        return -1;
    }
    
    //-----------------------------------------------
    template<typename T1>
    inline s32 ArrayFindSafe<T1>::find( typename ArrayFind<T1>::PredicateCallback cb ) const
    {
        SyncTokenPtr token = ArraySafe<T1>::_sync.getToken();
        if (NULL == cb || ArraySafe<T1>::size() == 0) return -1;
        for (u32 i = 0; i < ArraySafe<T1>::size(); ++i) {
            if (cb(*this[i]))
                return i;
        }
        return -1;
    }




} // end namespace hyc

#endif // end of define __BP_ARRAYSAFE_H_20110906102039__
#endif // HYC_COMPILE_MODULE_THREAD

