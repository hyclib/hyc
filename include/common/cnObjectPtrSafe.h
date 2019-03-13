//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc framework
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_THREAD

#ifndef __BP_CNOBJECTPTRSAFE_H_20110715180853__
#define __BP_CNOBJECTPTRSAFE_H_20110715180853__

#include "../os/atomic.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	线程安全引用计数对象
    //----------------------------------------------------------------------------------------------------
    class HYC_API RefCountSafe
    {
    public:
        RefCountSafe() : _refCountSafe_ref(0)    {}
        RefCountSafe(const RefCountSafe& r) : _refCountSafe_ref(0) {}
        virtual ~RefCountSafe()  {}

        RefCountSafe& operator=(const RefCountSafe&)  { return *this; }

        // 不要手动调用.
        virtual void    addRef()    { assert(_refCountSafe_ref >= 0); ++_refCountSafe_ref; }
        virtual void    release()   { assert(_refCountSafe_ref > 0); if (0 == --_refCountSafe_ref) { onDestroy(); } }
        virtual integer getRef() const { return _refCountSafe_ref; }

    protected:
        /**
         * @brief 销毁函数. 重写此函数可用于自定义的内存释放.
         */
        virtual void   onDestroy()  { delete this; }

    private:
        mutable AtomicBase<s32> _refCountSafe_ref;
    };

} // end namespace hyc

#endif // end of define __BP_CNOBJECTPTRSAFE_H_20110715180853__
#endif // HYC_COMPILE_MODULE_THREAD

