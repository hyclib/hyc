//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  鏁扮粍.
//

#ifndef __BP_ARRAY_H_20101208131648__
#define __BP_ARRAY_H_20101208131648__

#include "../hycPlatform.h"

#include "memoryAlloc.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	Declare Array
    //----------------------------------------------------------------------------------------------------
    template<typename T>
    class Array
    {
    public:
        typedef T*          iterator;
        typedef const T*    const_iterator;

    public:
        Array( u32 memLen = 0 );
        Array( const Array& arr );
        Array( T* elems, u32 elemNum );
        virtual ~Array()        { Array<T>::clear();  MemoryAlloc::Free(m_pMem); m_pMem = NULL; m_memLen = m_curLen = 0; }
        
        virtual void        operator=( const Array& arr )   { Array<T>::clear(); for ( u32 i = 0; i < arr.size(); ++i ) { Array<T>::push_back(arr[i]); } }
        virtual T&          operator[]( u32 index );
        virtual const T&    operator[]( u32 index ) const;
        virtual Array       operator+( const Array& arr )   { Array arrT(*this); arrT.reserve(this->size()+arr.size()); for (u32 i=0;i<arr.size();++i) {arrT.push_back(arr[i]);} return arrT;  }
        virtual Array&      operator+=( const Array& arr )  { Array<T>::reserve(this->size()+arr.size()); for (u32 i=0;i<arr.size();++i) {Array<T>::push_back(arr[i]);} return *this;  }

        virtual const T&    getAt( u32 index ) const;
        virtual T&          getAt( u32 index );
        virtual T&          front()                         { return getAt(0); }
        virtual const T&    front() const                   { return getAt(0); }
        virtual T&          back()                          { return getAt(m_curLen-1); }
        virtual const T&    back() const                    { return getAt(m_curLen-1); }
        virtual T*          begin()                         { return m_pMem; }
        virtual const T*    begin() const                   { return m_pMem; }
        virtual T*          end()                           { return m_pMem+m_curLen; }
        virtual const T*    end() const                     { return m_pMem+m_curLen; }
        virtual u32         capacity() const                { return m_memLen; }
        virtual void        clear()                         { for ( u32 i = 0; i < size(); ++i ) { MemoryAlloc::Destory( (*this)[i] ); } m_curLen = 0; }
        virtual bool        empty() const                   { return 0 >= m_curLen; }
        virtual void        erase( iterator itr );
        virtual void        eraseByIndex( u32 index );
        virtual void        insert( u32 index, const T& val );
        virtual void        push_back( const T& val )       { Array<T>::insert( size(), val ); }
        virtual void        reserve( u32 reserveSize );
        virtual u32         size() const                    { return m_curLen; }
        virtual void        resize( u32 newsize );
        virtual void        resize( u32 newsize, const T& newValue );
        virtual void        flip();
                
    protected:
        u32 m_curLen; u32 m_memLen; T*  m_pMem;
    };

    //----------------------------------------------------------------------------------------------------
    //!	Declare ArrayFind, include method: find
    //! Use Array, T must implement these method:
    //!    1. T::T();
    //!    2. T::T( const T& );
    //!    3. bool T::operator==( const T& ) const;
    //!    4. bool T::operator<( const T& ) const;
    //----------------------------------------------------------------------------------------------------
    template<typename T>
    class ArrayFind : public Array<T>
    {
    public:
        //! Predicate Function Type
        typedef bool(*CallbackPredicate)(const T& e, const T& eInArray);
        //! qfind Function Type
        //! 升序: return -1 ap<bp; 0 ap==bp; 1 ap>bp;
        //! 降序: return 1 ap<bp; 0 ap==bp; -1 ap>bp;
        typedef int (*CallbackQFind)(const void* ap, const void* bp);

    public:
        ArrayFind( u32 memLen = 0 );
        ArrayFind( const Array<T>& arr );
        ArrayFind( const ArrayFind& arr );
        ArrayFind( T* elems, u32 elemNum );
        virtual ~ArrayFind()	{}

        virtual void        operator=( const Array<T>& arr )       { Array<T>::clear(); for ( u32 i = 0; i < arr.size(); ++i ) { Array<T>::push_back(arr[i]); } }
        virtual void        operator=( const ArrayFind& arr )   { Array<T>::clear(); for ( u32 i = 0; i < arr.size(); ++i ) { Array<T>::push_back(arr[i]); } }

        // 查找指定值,返回-1表明未找到.
        virtual s32     find( const T& e, CallbackPredicate cb = _find_compare<T> ) const;

        // 使用二叉查找方式查找指定值,返回-1表明未找到.
        virtual s32     find_q( const T& e, CallbackQFind cb = _find_q_compare<T>) const;

        // 排序(默认升序排列).
        virtual void        sort( CallbackQFind cb = _find_q_compare<T> ) const;
        virtual void        sort_insert( const T& e, CallbackQFind cb = _find_q_compare<T> );

    public:
        template<class E>
        inline static bool _find_compare( const E& e, const E& eInArray ) {
            return e == eInArray;
        }
        template<class E>
        inline static int _find_q_compare( const void* ap, const void* bp ) {
            const E* a = (E*)ap;
            const E* b = (E*)bp;
            if (*a == *b)       return 0;
            else if (*a < *b)   return -1;
            else                return 1;
        }
    };


    //----------------------------------------------------------------------------------------------------
    //*	Implement Array
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    template<typename T>
    inline Array<T>::Array( u32 memLen )
    : m_curLen(0), m_memLen(0), m_pMem(0)  
    {
        Array<T>::reserve( memLen ); 
    }
    
    //-----------------------------------------------
    template<typename T>
    inline Array<T>::Array( const Array<T>& arr ) 
        : m_curLen(0), m_memLen(0), m_pMem(0)  
    {
        Array<T>::reserve( arr.size() ); *this = arr; 
    }
    
    //-----------------------------------------------
    template<typename T>
    inline Array<T>::Array( T* elem, u32 elemNum )
        : m_curLen(0), m_memLen(0), m_pMem(0) {
            assert(elemNum >= 1);
            Array<T>::reserve( elemNum );

            for (u32 i = 0; i < elemNum; i++)
            {
                Array<T>::push_back(elem[i]);
            }
    }

//     //-----------------------------------------------
//     template<typename T>
//     inline Array<T>::Array( const Array& rhs, u32 appendElemNum, T elem1, ... )
//         : m_memLen(0), m_pMem(0), m_curLen(0) {
// 
//             assert(appendElemNum >= 1);
//             Array<T>::reserve( appendElemNum+rhs.size() );
//             Array<T>::push_back(elem1);
//             *this = rhs;
//             va_list arg_ptr;
//             va_start(arg_ptr, elem1);
//             for (u32 i = 1; i < appendElemNum; ++i) {
//                 Array<T>::push_back( va_arg(arg_ptr, T) );
//             }
//             va_end(arg_ptr);
//     }
    
    //-----------------------------------------------
    template<typename T>
    inline T& Array<T>::operator[]( u32 index )
    {
        return getAt(index);
    }
    
    //-----------------------------------------------
    template<typename T>
    inline const T& Array<T>::operator[]( u32 index ) const
    { 
        return getAt(index);
    }
    
    //-----------------------------------------------
    template<typename T>
    inline const T& Array<T>::getAt( u32 index ) const
    { 
        if (index >= m_curLen) { error_all(_T("array out of the range %d %d"), index, m_curLen); throw ExceptionType_OutOfRange; }
        return m_pMem[index];
    }

    //-----------------------------------------------
    template<typename T>
    inline T& Array<T>::getAt( u32 index )
    { 
        if (index >= m_curLen) { error_all(_T("array out of the range %d %d"), index, m_curLen); throw ExceptionType_OutOfRange; }
        return m_pMem[index];
    }

    //-----------------------------------------------
    template<typename T>
    inline void hyc::Array<T>::erase( iterator itr )
    {
        if (itr >= this->end() || itr < this->begin())
        {
            error_all(_T("error iterator"));
            throw ExceptionType_OutOfRange;
        }

        eraseByIndex((u32)(itr-this->begin()));
    }

    //-----------------------------------------------
    template<typename T>
    inline void Array<T>::eraseByIndex( u32 index )
    {
        if ( index < size() )
        {
            MemoryAlloc::Destory( (*this)[index] );
            for ( u32 i = index+1; i < size(); ++i )  // move memory
            {
                MemoryAlloc::Initialize( (*this)[i-1], (*this)[i] );
                MemoryAlloc::Destory( (*this)[i] );
            }
            --m_curLen;
        } // if
        else
        {
            error_all(_T("error index"));
            throw ExceptionType_OutOfRange;
        }
    }

    //-----------------------------------------------
    template<typename T>
    inline void Array<T>::insert( u32 index, const T& val )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma warning(disable: 4018)
#endif
        if ( capacity() <= size() ) Array<T>::reserve( (u32)(capacity()*1.5)+1 );
        if ( size() > index )
        {
            for ( s32 i = size()-1; i >= (s32)index; --i )  // move memory
            {
                MemoryAlloc::Initialize( m_pMem[i+1], m_pMem[i] );
                MemoryAlloc::Destory( m_pMem[i] );
            }
            //MemoryAlloc::MemMove( m_pMem+index+1, m_pMem+index, sizeof(T)*(size()-index) );
        }
        ++m_curLen;
        MemoryAlloc::Initialize( (*this)[index], val );
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma warning(default: 4018)
#endif
    }

    //-----------------------------------------------
    template<typename T>
    inline void hyc::Array<T>::reserve( u32 reserveSize )
    {
        if ( reserveSize > 0 && 0 == capacity() )
        {
            m_pMem = (T*)MemoryAlloc::Malloc(reserveSize*sizeof(T));
            m_memLen = reserveSize;
        }
        else if ( reserveSize>capacity() || (reserveSize<capacity() && reserveSize>=size()) )
        {
            m_memLen = reserveSize;

            if ( 0 == reserveSize ) { MemoryAlloc::Free( m_pMem ); m_pMem = NULL; }
            else if ( NULL == m_pMem )
                m_pMem = (T*)MemoryAlloc::Malloc(reserveSize*sizeof(T));
            else
            {
                T* pMemTemp = (T*)MemoryAlloc::Malloc(reserveSize*sizeof(T));
                for (u32 i = 0; i < size(); ++i)
                {
                    MemoryAlloc::Initialize( pMemTemp[i], m_pMem[i] );
                    MemoryAlloc::Destory( m_pMem[i] );
                }
                MemoryAlloc::Free( m_pMem );
                m_pMem = pMemTemp;
            }
        } // if
    }

    //-----------------------------------------------
    template<typename T>
    inline void Array<T>::resize( u32 newsize )
    {
        resize(newsize, T());
    }

    //-----------------------------------------------
    template<typename T>
    inline void Array<T>::resize( u32 newsize, const T& newValue )
    {
        if (newsize == size())  return;
        if (newsize < size())
        {
            while (newsize < size()) {
                Array<T>::eraseByIndex(size()-1);
            }
        }
        else
        {
            Array<T>::reserve(newsize);
            while (newsize > size()) {
                Array<T>::push_back(newValue);
            }
        }
    }

    //-----------------------------------------------
    template<typename T>
    inline void hyc::Array<T>::flip()
    {
        if (size() > 1)
        {
            T t;
            for (s32 i = size()/2-1; i >= 0; i--)
            {
                t = getAt(i);
                getAt(i) = getAt(size()-i-1);
                getAt(size()-i-1) = t;
            }
        }
    }

    //-----------------------------------------------
    template<typename T>
    inline ArrayFind<T>::ArrayFind( u32 memLen )
    : Array<T>(memLen)  
    {
    }

    //-----------------------------------------------
    template<typename T>
    inline ArrayFind<T>::ArrayFind( const ArrayFind<T>& arr ) 
        : Array<T>(arr)
    {
    }

    //-----------------------------------------------
    template<typename T>
    inline ArrayFind<T>::ArrayFind( const Array<T>& arr ) 
        : Array<T>(arr)
    {
    }
    
    //-----------------------------------------------
    template<typename T>
    inline ArrayFind<T>::ArrayFind( T* elems, u32 elemNum )
    {
        assert(elemNum >= 1);
        Array<T>::reserve( elemNum );
        for (u32 i = 0; i < elemNum; i++)
        {
            this->push_back(elems[i]);
        }
    }

    //-----------------------------------------------
    template<typename T>
    inline s32 ArrayFind<T>::find( const T& e, typename ArrayFind<T>::CallbackPredicate cb ) const
    {
        if (NULL == cb || Array<T>::size() == 0) return -1;
        for (u32 i = 0; i < Array<T>::size(); ++i) {
            if (cb(e, Array<T>::m_pMem[i]))
                return i;
        }
        return -1;
    }

    //-----------------------------------------------
    template<typename T>
    inline s32 hyc::ArrayFind<T>::find_q( const T& e, typename ArrayFind<T>::CallbackQFind cb/* = find_q_compare<T>*/ ) const
    {
        T* p = (T*)bsearch(&e, Array<T>::m_pMem, Array<T>::m_curLen, sizeof(e), cb);
        return p ? (s32)(p-Array<T>::m_pMem) : -1;
    }

    //-----------------------------------------------
    template<typename T>
    inline void hyc::ArrayFind<T>::sort( typename ArrayFind<T>::CallbackQFind cb ) const
    {
        qsort(Array<T>::m_pMem, (size_t)(Array<T>::m_curLen), sizeof(T), cb);
        //std::sort(Array<T>::m_pMem, Array<T>::m_pMem+Array<T>::m_curLen, cb);
    }

    //-----------------------------------------------
    template<typename T>
    inline void hyc::ArrayFind<T>::sort_insert( const T& e, typename ArrayFind<T>::CallbackQFind cb )
    {
        s32 min = 0;
        s32 max = ArrayFind<T>::size()-1;
        s32 i;

        while (min <= max) {
            i = (min+max) / 2;

            if (cb(&(Array<T>::getAt(i)), &e) >= 0) {
                max = i - 1; 
            } else {
                min = i + 1;
            }
        }

        assert(min >= 0 && min <= (s32)ArrayFind<T>::size());
        Array<T>::insert(min, e);
    }

} // end namespace hyc

#endif // end of define __BP_ARRAY_H_20101208131648__
