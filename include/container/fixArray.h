//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc framework
// author:       lipengxiang
// description:  
//

#ifndef __BP_FIXARRAY_H_20110331170532__
#define __BP_FIXARRAY_H_20110331170532__

#include "memoryAlloc.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	固定长度数组;
    //! 可以代替类似: char* pBuf = new char[n]; 来分配数组, 而无需担心忘记删除问题
    //----------------------------------------------------------------------------------------------------
    template<typename T>
    class FixArray
    {
        enum { MAX_LEN = 1024 * 1024 * 10 };
    public:
        FixArray( uinteger len = 0 ) : m_len(len), m_ppT(0)         { if (m_len>MAX_LEN) m_len = MAX_LEN; if (m_len > 0){ m_ppT = new T[m_len]; } }
        FixArray( const FixArray& rhs ) : m_len(rhs.m_len), m_ppT(0){ if (m_len > 0){ m_ppT = new T[m_len]; } for (uinteger i = 0; i < rhs.size(); ++i) { (*this)[i] = rhs[i]; }  }
        virtual ~FixArray()                                         { SAFE_DELETE_ARRAY(m_ppT); }

        T&          operator[]( integer index )         { assert(index>=0); return m_ppT[index]; }
        const T&    operator[]( integer index ) const   { assert(index>=0); return m_ppT[index]; }
        T*          begin()                         { return m_ppT; }
        T*          begin() const                   { return m_ppT; }
        T*          end()                           { return m_ppT+size(); }
        T*          end() const                     { return m_ppT+size(); }
        uinteger    size() const                    { return m_len; }

        /**
         * @brief resize, but don't copy old data.
         */
        void        resize( uinteger csize );

        /**
         * @brief resize and copy old data.
         */
        void        resizeAndCopy( uinteger csize );

    private:
        uinteger m_len;
        T*  m_ppT;
    };

    //-----------------------------------------------
    template<typename T>
    inline void hyc::FixArray<T>::resize( uinteger csize )
    {
        if (csize == m_len) return;
        T* pNew = NULL;
        if (csize>MAX_LEN) csize = MAX_LEN;  if (csize > 0) { pNew = new T[csize]; }
        SAFE_DELETE_ARRAY(m_ppT); m_len = csize; m_ppT = pNew;
    }

    //-----------------------------------------------
    template<typename T>
    inline void hyc::FixArray<T>::resizeAndCopy( uinteger csize )
    {
        if (csize == m_len) return;
        T* pNew = NULL;
        if (csize>MAX_LEN) csize = MAX_LEN;  if (csize > 0) { pNew = new T[csize]; }

        for (s32 i = Math::Min(csize, m_len)-1; i >= 0; i--) {
            pNew[i] = m_ppT[i];
        }

        SAFE_DELETE_ARRAY(m_ppT); m_len = csize; m_ppT = pNew;
    }



} // end namespace hyc

#endif // end of define __BP_FIXARRAY_H_20110331170532__
