//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyui Graphic Engine
// author:       lipengxiang
// description:  
//

#ifndef __BP_YUNSTREAMTYPE_H_20121003232005__
#define __BP_YUNSTREAMTYPE_H_20121003232005__

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //! 用于传输的自定义类型的接口.
    //----------------------------------------------------------------------------------------------------
    class YunStreamType : public RefCountSafe
    {
    public:
        virtual ~YunStreamType()    {}

        /**
         * 序列化类型的数据到buf中, buf可能存在数据, 序列化添加数据到buf末尾.
         */
        virtual bool    serialize( OUT ByteStream& buf ) const = 0;

        /**
         * 反序列化buf到本对象的数据中, 从buf的pos位置开始, 并且有效buf的长度为len.
         */
        virtual bool    deserialize( const ByteStream& buf, INOUT u32& pos, u32 len ) = 0;
    };

} // end namespace hyc

#endif // end of define __BP_YUNSTREAMTYPE_H_20121003232005__
