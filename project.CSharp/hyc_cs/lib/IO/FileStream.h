//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc
// author:       lipengxiang
// description:  YunStream.h
//

#pragma once

#include "hyc/include/hyc.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace Hy
{
namespace IO
{
    /// <summary>
    /// 文件流
    /// </summary>
    public ref class FileStream
    {
        hyc::FileStream* _ptr;
    public:
        /// <summary>
        /// seek相对位置
        /// </summary>
        enum class SeekOrigin
        {
            SO_Begin, 
            SO_Current, 
            SO_End
        };
        /// <summary>
        /// 打开文件模式
        /// </summary>
        enum class FileMode
        {
            // 必选项(二选一)
            FM_Read         = 1<<0,     // 读;  若无此标志不可读
            FM_ReadWrite    = 1<<1,     // 读写;  若无此标志不可写
            // 可选项(可组合)
            FM_Binary  = 1<<2,     // 二进制;  若无此标志以文本方式打开
            FM_New     = 1<<3      // 新建; 若无此标志不新建文件(与READ组合无意义,将使用默认)
        };
    public:
        FileStream()    {   _ptr = new hyc::FileStream();   }
        ~FileStream()   {   this->!FileStream();}
        !FileStream()   {   SAFE_DELETE(_ptr);  }

        //-----------------------------------------------
        // 基本操作
        bool        open( System::String^ filePath, INT32 mode )
        {
            return _ptr->open((hyc::tchar*)Marshal::StringToCoTaskMemAnsi(filePath).ToPointer(), mode);
        }

        bool        isOpen()    {   return _ptr->isOpen();  }
        void        clear()     {   _ptr->clear();  }
        void        close()     {   _ptr->close();  }
        void        flush()     {   _ptr->flush();  }
        System::String^ getPath()   { return Marshal::PtrToStringAnsi(IntPtr((void*)_ptr->getPath())); }

        //-----------------------------------------------
        // 设置stream当前位置
        //  - 返回设置后的位置
        bool        seek( INT32 offset, SeekOrigin origin )  { return _ptr->seek(offset, (hyc::FileStream::SeekOrigin)origin); }
        INT32       tell()  { return _ptr->tell(); }

        //-----------------------------------------------
        // 设置流长度; 单位字节
        void        setByteSize( UINT32 newSize )   { _ptr->setByteSize(newSize); }
        INT32       getByteSize()   { return _ptr->getByteSize(); }
        INT32       getLength()     { return _ptr->getLength(); }

        //-----------------------------------------------
        // 读取/写; 返回写入/读取的字节数; -1为错误
        INT32       readByte( [Out] array<System::Byte>^% buf, UINT size )
        {
            if (size > (UINT)buf->Length) return -1;
            GCHandle^ hObject = GCHandle::Alloc(buf, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();
            INT32 s = _ptr->readByte(pObject.ToPointer(), size);
            if(hObject->IsAllocated)
                hObject->Free();
            return s;
        }
        INT32       readStream( [Out] FileStream^% stream, INT32 lengthOfByte )
        {
            return _ptr->readStream( *stream->_ptr, lengthOfByte );
        }
        INT32       writeByte( array<System::Byte>^% buf, UINT size )
        {
            if (size > (UINT)buf->Length) return -1;
            GCHandle^ hObject = GCHandle::Alloc(buf, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();
            INT32 s = _ptr->writeByte(pObject.ToPointer(), size);
            if(hObject->IsAllocated)
                hObject->Free();
            return s;
        }
        INT32       writeStream( FileStream^% stream, INT32 lengthOfByte )
        {
            return _ptr->writeStream( *stream->_ptr, lengthOfByte );
        }

        //-----------------------------------------------
        // 中间截断流; 在流的当前位置开始, 截断length长度的流
        bool        splitStream( INT32 lengthOfByte )
        {
            return _ptr->splitStream( lengthOfByte );
        }

        //-----------------------------------------------
        // 插入流; 在流的当前位置插入pStream的当前位置开始的length长度的流
        bool        insertStream( FileStream^% pStream, INT32 lengthOfByte )
        {
            return _ptr->insertStream( *pStream->_ptr, lengthOfByte );
        }

        //-----------------------------------------------
        // 附加数据到流的末尾; 在流的尾部附加pStream的当前位置开始的length长度的流
        bool        appendStream( FileStream^% pStream, INT32 lengthOfByte )
        {
            return _ptr->appendStream( *pStream->_ptr, lengthOfByte );
        }

        // put out stream
        void writeBool(bool val)   { (*_ptr)<<val; }
        void writeUInt8(hyc::u8 val )   { (*_ptr)<<val; }
        void writeInt8(hyc::s8 val )   { (*_ptr)<<val; }
        void writeUInt16(hyc::u16 val )   { (*_ptr)<<val; }
        void writeInt16(hyc::s16 val )   { (*_ptr)<<val; }
        void writeUInt32(hyc::u32 val )   { (*_ptr)<<val; }
        void writeInt32(hyc::s32 val )   { (*_ptr)<<val; }
        void writeUInt64(hyc::u64 val )   { (*_ptr)<<val; }
        void writeInt64(hyc::s64 val )   { (*_ptr)<<val; }
        void writeFloat(hyc::f32 val )   { (*_ptr)<<val; }
        void writeDouble(hyc::f64 val )   { (*_ptr)<<val; }
        void writeString(System::String^ val )   { (*_ptr)<< (hyc::tchar*)Marshal::StringToHGlobalAnsi(val).ToPointer(); }

        // get in stream
        System::String^ readString()  { hyc::String r; (*_ptr) >> r; System::String^ val = Marshal::PtrToStringAnsi(IntPtr((void*)r.c_str())); return val; }
        System::Boolean readBool()  { bool r; (*_ptr) >> r; return r; }
        UINT8           readUInt8()  { hyc::u8 r; (*_ptr) >> r; return r; }
        INT8            readInt8()  { hyc::s8 r; (*_ptr) >> r; return r; }
        UINT16          readUInt16()  { hyc::u16 r; (*_ptr) >> r; return r; }
        INT16           readInt16()  { hyc::s16 r; (*_ptr) >> r; return r; }
        UInt32          readUInt32()  { hyc::u32 r; (*_ptr) >> r; return r; }
        Int32           readInt32()  { hyc::s32 r; (*_ptr) >> r; return r; }
        UInt64          readUInt64()  { hyc::u64 r; (*_ptr) >> r; return r; }
        Int64           readInt64()  { hyc::s64 r; (*_ptr) >> r; return r; }
        FLOAT           readFloat()  { hyc::f32 r; (*_ptr) >> r; return r; }
        Double          readDouble()  { hyc::f64 r; (*_ptr) >> r; return r; }

    };

} // end namespace of IO
} // end namespace of Hy
