//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// YunFramework Gateway
// author:       lipengxiang
// description:  
//


#ifndef __BP_DYNLIBUTILS_H_20110906103505__
#define __BP_DYNLIBUTILS_H_20110906103505__

#if !defined(CN_PLATFORM_MOBILE)


namespace hyc
{
	//----------------------------------------------------------------------------------------------------
	//!	DynLib 动态连接库
	//----------------------------------------------------------------------------------------------------
    class HYC_API DynLib : public RefCount
    {
        PRIVATE_CLASS_DECLARE;
    public:
        //! 传入动态库名称以完成动态库的加载
        DynLib( const String& filename );
        ~DynLib();

        //! 判断是否加载成功
        bool            isLoaded() const;

        //! 获得动态连接库的名称
        const String&   getName() const;

        //! 获得符号位置;
        void*           getSymbol( const String& symbol ) const;
    };
    typedef ObjectPtr<DynLib>   DynLibPtr;
	 

} // end namespace hyc

#endif

#endif // end of define __BP_DYNLIBUTILS_H_20110906103505__

