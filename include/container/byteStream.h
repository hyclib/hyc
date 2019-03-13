//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// business logic layer network proxy
// author:       lipengxiang
// description:  字节流
//

#ifndef __BP_BYTESTREAM_H_20110221161634__
#define __BP_BYTESTREAM_H_20110221161634__

#include "../common/cnInterface.h"
#include "../common/cnObjectPtr.h"
#include "../common/cnObjectPtrSafe.h"
#include "../utils/byteUtils.h"
#include "memoryAlloc.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	字节流.
    //----------------------------------------------------------------------------------------------------

    class HYC_API ByteStream : public INoncopyable, public RefCountSafe, public MemoryAllocObject
    {
    public:
        static const u32 npos = -1;   // 不可能达到的长度.
    public:
        /**
         * @brief get the flash 7 bit value byte count.
         */
        static u8   Get7BitValueSize(u64 value);

    public:
        /**
         * @brief 构造字节流.
         * @param maxCapacity 当容量超过这个标志时, 将不会按照*1.5的方式分配.
         */
        ByteStream( u32 maxCapacity = 1024*10 );
        /**
         * @brief 使用外部流来构造字节流, 所有的操作将都直接操作外部流.
         * @param buffer      外部的流, 析构不释放, ByteStream仅对其内容操作.
         */
        ByteStream( Byte* buffer, u32 size );
        virtual ~ByteStream();

        virtual void        operator+=( const char* pStr );
        virtual void        operator+=( const cString& str );
        virtual Byte&       operator[]( u32 n )         { return getAt(n); }
        virtual const Byte& operator[]( u32 n ) const   { return getAt(n); }
        virtual Byte&       getAt( u32 n );
        virtual const Byte& getAt( u32 n ) const;

        /**
         * @brief 比较流.
         */
        virtual s32         compare( u32 off, u32 count, const Byte* data ) const;

        /**
         * @brief 添加流.
         */
        virtual void        add( const void* pAppendBuf, u32 size );
        virtual void        push_back( Byte v );

        /**
         * @brief 与other交换流.
         */
        virtual void        swapBuffer( ByteStream& other );

        /**
         * @brief 查找.
         */
        virtual u32         find(Byte c, u32 off = 0) const;
        virtual u32         find_last_of(Byte c, u32 off = npos) const;

        /**
         * @brief 获得第一个元素的指针.
         */
        virtual Byte*       begin();
        virtual const Byte* begin() const;
        
        /**
         * @brief 获得最后一个元素的指针+1.
         */
        virtual Byte*       end();
        virtual const Byte* end() const;
        
        virtual u32         size() const;
        virtual u32         capacity() const;
        virtual bool        empty() const;
        virtual void        clear();
        
        /**
         * @brief 创建及只保留n个元素.
         *        保留原有内容并为新内容赋值0. position不变.
         */
        virtual void        resize( u32 n );
        
        /**
         * @brief 预分配n 大小的空间.
         */
        virtual void        reserve( u32 n );

        /**
         * @brief read & write position.
         */
        inline u32          getPosition() const        { return _position; }
        inline void         setPosition(u32 pos) const { _position = pos; }
        inline void         incPosition(s32 inc) const { _position += inc; }
        
        /**
         * @brief 流的当前位置是否在流的有效范围内.
         */
        inline u32          available() const  { return size()>_position ? size()-_position : 0; }
        
        /**
         * @brief 流的当前位置数据指针.
         */
        inline Byte*        current()          { return begin()+_position; }
        inline const Byte*  current() const    { return begin()+_position; }

        /**
         * @brief 在流的当前位置进行写操作.
         *        每次写操作都将递增当前位置.
         */
        inline void writeU8( u8 v )     { reserve(_position+sizeof(v)); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeU16( u16 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton(v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeU32( u32 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton(v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeU64( u64 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton(v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeS8( s8 v )     { reserve(_position+sizeof(v)); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeS16( s16 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton(v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeS32( s32 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton(v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeS64( s64 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton((u64)v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeF32( f32 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton(v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeF64( f64 v )   { reserve(_position+sizeof(v)); v = ByteUtils::Hton(v); memcpy(begin()+_position, &v, sizeof(v)); _position += sizeof(v); if (size() < _position) { resize(_position); } }
        inline void writeRaw( const Byte* v, u32 sizeData ) { reserve(_position+sizeData); memcpy((begin()+_position), v, sizeData); _position += sizeData; if (size() < _position) { resize(_position); } }
        void        write7BitValue( u32 v );            // write a 7 bits struct to 8 bits struct, 可以存储最大u32.
        void        write7BitLongValue( u64 v );        // 只能存储56bit的数据.
        void        writeStringPure( const String& v ); // write buffer, 不包括结尾0 (不改变编码).
        void        writeStringNative( const String& v );     // len+buffer+0 的方式写入(不改变编码).
        void        writeStringUtf8( const String& v );       // len+utf8_buffer+0 的方式写入(不改变编码).

        /** 
         * @brief 从流的当前位置进行读操作
         *        每次读操作都将递增当前位置.
         * @exception ExceptionType_OutOfRange
         */
        inline u8   readU8() const   { if (available()<sizeof(u8)) { throw ExceptionType_OutOfRange; } u8 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return v; }
        inline u16  readU16() const  { if (available()<sizeof(u16)) { throw ExceptionType_OutOfRange; } u16 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh(v); }
        inline u32  readU32() const  { if (available()<sizeof(u32)) { throw ExceptionType_OutOfRange; } u32 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh(v); }
        inline u64  readU64() const  { if (available()<sizeof(u64)) { throw ExceptionType_OutOfRange; } u64 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh(v); }
        inline s8   readS8() const   { if (available()<sizeof(s8)) { throw ExceptionType_OutOfRange; } s8 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return v; }
        inline s16  readS16() const  { if (available()<sizeof(s16)) { throw ExceptionType_OutOfRange; } s16 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh((u16)v); }
        inline s32  readS32() const  { if (available()<sizeof(s32)) { throw ExceptionType_OutOfRange; } s32 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh((u32)v); }
        inline s64  readS64() const  { if (available()<sizeof(s64)) { throw ExceptionType_OutOfRange; } s64 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh((u64)v); }
        inline f32  readF32() const  { if (available()<sizeof(f32)) { throw ExceptionType_OutOfRange; } f32 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh(v); }
        inline f64  readF64() const  { if (available()<sizeof(f64)) { throw ExceptionType_OutOfRange; } f64 v = 0; memcpy(&v, begin()+_position, sizeof(v)); _position += sizeof(v); return ByteUtils::Ntoh(v); }
        inline void readRaw( OUT Byte* v, u32 size ) const    { if (available()<size) { throw ExceptionType_OutOfRange; } memcpy(v, begin()+_position, size); _position += size; }
        u32         read7BitValue() const;
        u64         read7BitLongValue() const;
        bool        readStringPure( OUT String& str, u32 sizeStr ) const; // 读取字符串(使用系统编码).
        bool        readStringNative( OUT String& str ) const;            // 读取字符串(使用系统编码).
        bool        readStringUtf8( OUT String& str ) const;              // 读取字符串(使用utf8编码).

        //! 用于记录网络包发送时的发送位置, 一次发送不完再次发送(增加Windows Platform IOCP效率).
        //! (外部不应该使用这个值).
        Byte*       _curPos;
        
    protected:
        Byte*       _buf;
        u32         _size;
        u32         _capacity;
        u32         _maxCapacity;
        bool        _extBuffer; // 使用外部流.
        mutable u32 _position;
    };
    typedef ObjectPtr<ByteStream>   ByteStreamPtr;

} // end namespace hyc

#endif // end of define __BP_BYTESTREAM_H_20110221161634__
