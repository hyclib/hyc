//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  原子操作要求操作数是u32内存对齐的.
//

#ifndef __BP_ATOMIC_H_20130903235845__
#define __BP_ATOMIC_H_20130903235845__


namespace hyc
{
    /**
     * @brief cas操作. 比较old与expected, 如果相等, 设置old为desired, 并返回true.
     *          无论操作结果如何, 始终将old原值写入expected
     * @param old       原始值.
     * @param expected  比较值.
     * @param desired   希望设置的值.
     * @return 表明是否相等.
     */
    inline bool atomic_compare_exchange( s16 volatile* old, s16* expected, s16 desired );
    inline bool atomic_compare_exchange( u16 volatile* old, s16* expected, s16 desired );
    inline bool atomic_compare_exchange( s32 volatile* old, s32* expected, s32 desired );
    inline bool atomic_compare_exchange( u32 volatile* old, u32* expected, u32 desired );
    inline bool atomic_compare_exchange( s64 volatile* old, s64* expected, s64 desired );
    inline bool atomic_compare_exchange( u64 volatile* old, u64* expected, u64 desired );

    /**
     * @brief 原子自增减, 返回结果值.
     */
    inline s32 atomic_increment( s32 volatile* old );
    inline u32 atomic_increment( u32 volatile* old );
    inline s64 atomic_increment( s64 volatile* old );
    inline u64 atomic_increment( u64 volatile* old );

    inline s32 atomic_decrement( s32 volatile* old );
    inline u32 atomic_decrement( u32 volatile* old );
    inline s64 atomic_decrement( s64 volatile* old );
    inline u64 atomic_decrement( u64 volatile* old );

#if CN_CPU_ARCH == CN_CPU_ARCHITECTURE_32
    inline bool atomic_compare_exchange( long volatile* old, long* expected, long desired ) { return atomic_compare_exchange((s32 volatile*)old, (s32*)expected, (s32)desired); }
    inline long atomic_increment( long volatile* old ) { return atomic_increment((s32 volatile*)old); }
    inline long atomic_decrement( long volatile* old ) { return atomic_decrement((s32 volatile*)old); }
#endif


    /**
     * @brief 原子同步对象.
     *      在锁定时, cpu将处于忙等状态, 应该用于锁定的操作时间较短的场景.
     *      与线程锁不同, 原子锁同一线程只能lockAll一次之后必须unlockAll, 无引用计数. 连续调用2次lockAll系统将卡死.
     */
    class HYC_API AtomicSyncObject
    {
    public:
        AtomicSyncObject()  : _tid(0)   {}
        ~AtomicSyncObject();

        //! 同一线程中锁定释放一次即全部锁定释放.
        void            lockAll();
        void            unlockAll();

        //! 成功锁定时返回true, 否则为false已被其他线程占用
        bool            tryLock();

    private:
        volatile integer   _tid;
    };

    /**
     * @brief 基础类型的原子模型的基类.
     */
    template<class T>
    class AtomicBase {
    public:
        AtomicBase() {}
        AtomicBase( const T& v ) : _ref(v)  {}

             operator T() const             { return _ref; }
        T    operator++()                   { return atomic_increment((T volatile*)&_ref); }
        T    operator--()                   { return atomic_decrement((T volatile*)&_ref); }
        bool operator>(const T& v) const    { return _ref > v; }
        bool operator>=(const T& v) const   { return _ref >= v; }
        bool operator==(const T& v) const   { return _ref == v; }
        bool operator!=(const T& v) const   { return _ref != v; }
        bool operator<=(const T& v) const   { return _ref <= v; }
        bool operator<(const T& v) const    { return _ref < v; }

        bool operator>(const AtomicBase<T>& v) const    { return _ref > v._ref; }
        bool operator>=(const AtomicBase<T>& v) const   { return _ref >= v._ref; }
        bool operator==(const AtomicBase<T>& v) const   { return _ref == v._ref; }
        bool operator!=(const AtomicBase<T>& v) const   { return _ref != v._ref; }
        bool operator<=(const AtomicBase<T>& v) const   { return _ref <= v._ref; }
        bool operator<(const AtomicBase<T>& v) const    { return _ref < v._ref; }

        AtomicBase& operator=(const T& v)  { _ref = v; return *this; }
        AtomicBase& operator=(const AtomicBase<T>& v)  { _ref = v._ref; return *this; }

        bool compare_exchange( integer* expected, integer desired ) { return atomic_compare_exchange((T volatile*)&_ref, expected, desired); }

    private:
        volatile T _ref;
    };



    //-----------------------------------------------
    inline bool atomic_compare_exchange( s16 volatile* old, s16* expected, s16 desired )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        assert(old && expected);
        LONG r = ::InterlockedCompareExchange16(old, desired, *expected);
        if (r == *expected)
        {
            return true;
        }
        else
        {
            *expected = r;
            return false;
        }
#else
        return __sync_bool_compare_and_swap(old, *expected, desired);
#endif
    }


    //-----------------------------------------------
    inline bool atomic_compare_exchange( u16 volatile* old, u16* expected, u16 desired )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        assert(old && expected);
        LONG r = ::InterlockedCompareExchange16((SHORT volatile*)old, desired, *expected);
        if (r == *expected)
        {
            return true;
        }
        else
        {
            *expected = r;
            return false;
        }
#else
        return __sync_bool_compare_and_swap(old, *expected, desired);
#endif
    }

    //-----------------------------------------------
    inline bool atomic_compare_exchange( s32 volatile* old, s32* expected, s32 desired )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        assert(old && expected);
        LONG r = ::InterlockedCompareExchange((LONG volatile*)old, desired, *expected);
        if (r == *expected)
        {
            return true;
        }
        else
        {
            *expected = r;
            return false;
        }
#else
        return __sync_bool_compare_and_swap(old, *expected, desired);
#endif
    }


    //-----------------------------------------------
    inline bool atomic_compare_exchange( u32 volatile* old, u32* expected, u32 desired )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        assert(old && expected);
        LONG r = ::InterlockedCompareExchange((LONG volatile*)old, desired, *expected);
        if (r == *expected)
        {
            return true;
        }
        else
        {
            *expected = r;
            return false;
        }
#else
        return __sync_bool_compare_and_swap(old, *expected, desired);
#endif
    }

    //-----------------------------------------------
    inline bool atomic_compare_exchange( s64 volatile* old, s64* expected, s64 desired )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        assert(old && expected);
        LONGLONG r = ::InterlockedCompareExchange64((LONGLONG volatile*)old, desired, *expected);
        if (r == *expected)
        {
            return true;
        }
        else
        {
            *expected = r;
            return false;
        }
#else
        return __sync_bool_compare_and_swap(old, *expected, desired);
#endif
    }
    //-----------------------------------------------
    inline bool atomic_compare_exchange( u64 volatile* old, u64* expected, u64 desired )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        assert(old && expected);
        LONGLONG r = ::InterlockedCompareExchange64((LONGLONG volatile*)old, desired, *expected);
        if (r == *expected)
        {
            return true;
        }
        else
        {
            *expected = r;
            return false;
        }
#else
        return __sync_bool_compare_and_swap(old, *expected, desired);
#endif
    }

    //-----------------------------------------------
    inline s32 atomic_increment( s32 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedIncrement((LONG volatile*)old);
#else
        return __sync_add_and_fetch(old, 1);
#endif
    }

    //-----------------------------------------------
    inline u32 atomic_increment( u32 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedIncrement((LONG volatile*)old);
#else
        return __sync_add_and_fetch(old, 1);
#endif
    }

    //-----------------------------------------------
    inline s64 atomic_increment( s64 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedIncrement64((LONGLONG volatile*)old);
#else
        return __sync_add_and_fetch(old, 1);
#endif
    }

    //-----------------------------------------------
    inline u64 atomic_increment( u64 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedIncrement64((LONGLONG volatile*)old);
#else
        return __sync_add_and_fetch(old, 1);
#endif
    }

    //-----------------------------------------------
    inline s32 atomic_decrement( s32 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedDecrement((LONG volatile*)old);
#else
        return __sync_sub_and_fetch(old, 1);
#endif
    }

    //-----------------------------------------------
    inline u32 atomic_decrement( u32 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedDecrement((LONG volatile*)old);
#else
        return __sync_sub_and_fetch(old, 1);
#endif
    }

    //-----------------------------------------------
    inline s64 atomic_decrement( s64 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedDecrement64((LONGLONG volatile*)old);
#else
        return __sync_sub_and_fetch(old, 1);
#endif
    }

    //-----------------------------------------------
    inline u64 atomic_decrement( u64 volatile* old )
    {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        return ::InterlockedDecrement64((LONGLONG volatile*)old);
#else
        return __sync_sub_and_fetch(old, 1);
#endif
    }

} // end namespace hyc

#endif // end of define __BP_ATOMIC_H_20130903235845__
