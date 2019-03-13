//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  内存分配
//

#ifndef __BP_MEMORYALLOC_H_20101208155744__
#define __BP_MEMORYALLOC_H_20101208155744__

#ifdef CN_LEAK_DETECTOR
#   undef new
#endif // CN_LEAK_DETECTOR

#include <new>

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	内存分配器; 用于分配一段内存; (不会进行,内存释放检查)
    //  分配与释放都由common模块引发, 避免了跨模块申请/释放内存溢出
    //  (内部使用nedmalloc来分配内存)
    //----------------------------------------------------------------------------------------------------
    class HYC_API MemoryAlloc
    {
    public:
        //-----------------------------------------------
        // 内存分配操作
        static void*    Malloc( uinteger size );
        static void     Free( void* ptr );
        static void*    Realloc( void* ptr, uinteger newSize );
        static void     MemMove( void* pDest, const void* pSrc, uinteger size );

#ifdef _DEBUG
        static void*    Malloc( uinteger size, const char* lpszFileName, int nLine );
#endif // _DEBUG


        //-----------------------------------------------
        // 构造内存块
        template<typename T>
        static void     Initialize( T& ptr )   { new (&ptr)T(); }
        template<typename T>
        static void     Initialize( T* ptr )   {}
        template<typename T>
        static void     Initialize( T& ptr, const T& other )   { new(&ptr) T(other); }
        template<typename T>
        static void     Initialize( T*& ptr, T* other )   { ptr = other; }

        //-----------------------------------------------
        // 析构内存块
        template<typename T>
        static void     Destory( T& ptr ){ (&ptr)->~T(); }
        template<typename T>
        static void     Destory( T* ptr ){}
    };

    //----------------------------------------------------------------------------------------------------
    //*	特化
    //----------------------------------------------------------------------------------------------------

    template<>
    inline void    MemoryAlloc::Initialize<u8>( u8& ptr )    {}
    template<>
    inline void    MemoryAlloc::Initialize<u16>( u16& ptr )  {}
    template<>
    inline void    MemoryAlloc::Initialize<u32>( u32& ptr )  {}
    template<>
    inline void    MemoryAlloc::Initialize<u64>( u64& ptr )  {}
    template<>
    inline void    MemoryAlloc::Initialize<s8>( s8& ptr )    {}
    template<>
    inline void    MemoryAlloc::Initialize<s16>( s16& ptr )  {}
    template<>
    inline void    MemoryAlloc::Initialize<s32>( s32& ptr )  {}
    template<>
    inline void    MemoryAlloc::Initialize<s64>( s64& ptr )  {}
    template<>
    inline void    MemoryAlloc::Initialize<f32>( f32& ptr )  {}
    template<>
    inline void    MemoryAlloc::Initialize<f64>( f64& ptr )  {}
    
    template<>
    inline void    MemoryAlloc::Initialize<u8>( u8& ptr, const u8& other )        { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<u16>( u16& ptr, const u16& other )     { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<u32>( u32& ptr, const u32& other )     { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<u64>( u64& ptr, const u64& other )     { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<s8>( s8& ptr, const s8& other )        { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<s16>( s16& ptr, const s16& other )     { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<s32>( s32& ptr, const s32& other )     { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<s64>( s64& ptr, const s64& other )     { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<f32>( f32& ptr, const f32& other )     { ptr = other; }
    template<>
    inline void    MemoryAlloc::Initialize<f64>( f64& ptr, const f64& other )     { ptr = other; }

    template<>
    inline void    MemoryAlloc::Destory<u8>( u8& ptr )    {}
    template<>
    inline void    MemoryAlloc::Destory<u16>( u16& ptr )  {}
    template<>
    inline void    MemoryAlloc::Destory<u32>( u32& ptr )  {}
    template<>
    inline void    MemoryAlloc::Destory<u64>( u64& ptr )  {}
    template<>
    inline void    MemoryAlloc::Destory<s8>( s8& ptr )    {}
    template<>
    inline void    MemoryAlloc::Destory<s16>( s16& ptr )  {}
    template<>
    inline void    MemoryAlloc::Destory<s32>( s32& ptr )  {}
    template<>
    inline void    MemoryAlloc::Destory<s64>( s64& ptr )  {}
    template<>
    inline void    MemoryAlloc::Destory<f32>( f32& ptr )  {}
    template<>
    inline void    MemoryAlloc::Destory<f64>( f64& ptr )  {}


    //----------------------------------------------------------------------------------------------------
    //!	内存分配对象, 继承至此对象, 将会使用高效的内存分配
    //----------------------------------------------------------------------------------------------------
    class HYC_API MemoryAllocObject
    {
    public:
        explicit MemoryAllocObject()
        { }

        virtual ~MemoryAllocObject()
        { }

        void* operator new(size_t size)
        {
            return MemoryAlloc::Malloc((uinteger)size);
        }

        /// placement operator new
        void* operator new(size_t sz, void* ptr)
        {
            (void) sz;
            return ptr;
        }

        void* operator new[](size_t size)
        {
            return MemoryAlloc::Malloc((uinteger)size);
        }

        void operator delete( void* ptr )
        {
            MemoryAlloc::Free(ptr);
        }
        
        // Corresponding operator for placement delete (second param same as the first)
        void operator delete( void* ptr, void* )
        {
            MemoryAlloc::Free(ptr);
        }

        void operator delete[]( void* ptr )
        {
            MemoryAlloc::Free(ptr);
        }


#ifdef _DEBUG
        void* operator new(size_t size, int blockflags, const char* lpszFileName, int nLine)
        {
            return MemoryAlloc::Malloc((uinteger)size, lpszFileName, nLine);
        }
        void* operator new[](size_t size, int blockflags, const char* lpszFileName, int nLine)
        {
            return MemoryAlloc::Malloc((uinteger)size, lpszFileName, nLine);
        }
        void operator delete( void* ptr, int blockflags, const char* lpszFileName, int nLine )
        {
            MemoryAlloc::Free(ptr);
        }
        void operator delete( void* ptr, void*, int blockflags, const char* lpszFileName, int nLine )
        {
            MemoryAlloc::Free(ptr);
        }
        void operator delete[]( void* ptr, int blockflags, const char* lpszFileName, int nLine )
        {
            MemoryAlloc::Free(ptr);
        }
#endif // _DEBUG
    };

} // end namespace hyc


#ifdef CN_LEAK_DETECTOR
#   define new LEAK_DETECTOR_ON
#endif // CN_LEAK_DETECTOR


#endif // end of define __BP_MEMORYALLOC_H_20101208155744__