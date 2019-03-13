//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_CNINTERFACE_H_20110704115240__
#define __BP_CNINTERFACE_H_20110704115240__


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	不可复制接口
    //! 继承此接口的类不可以直接被复制
    //----------------------------------------------------------------------------------------------------
	class HYC_API INoncopyable
    {
    public:
        INoncopyable()  {}
        virtual ~INoncopyable()  {}
    private:
        INoncopyable( const INoncopyable& );
        void operator=( const INoncopyable& );
    };


} // end namespace hyc

#endif // end of define __BP_CNINTERFACE_H_20110704115240__

