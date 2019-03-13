//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  文件流操作; 读取的文件不能超过4G
//

#ifndef __BP_FILESTREAM_H_20101202112706__
#define __BP_FILESTREAM_H_20101202112706__

#include "../common/cnObjectPtr.h"

namespace hyc
{
    class FileStream;
    typedef ObjectPtr<FileStream>   FileStreamPtr;

    //-----------------------------------------------
    // only support the file size up to 4 G.
    class HYC_API FileStream : public INoncopyable, public RefCount
    {
        PRIVATE_CLASS_DECLARE;
    public:
        //-------------
        //! seek相对位置.
        typedef enum {
            SO_Begin, 
            SO_Current, 
            SO_End,
        } SeekOrigin;

        //-------------
        //! 打开文件模式.
        typedef enum {
            // 必选项(二选一)
            FM_Read         = 1<<0,     //! 读;  若无此标志不可读.
            FM_ReadWrite    = 1<<1,     //! 读写;  若无此标志不可写.
            // 可选项(可组合)
            FM_Binary  = 1<<2,     //! 二进制;  若无此标志以文本方式打开.
            FM_New     = 1<<3,     //! 新建; 若无此标志不新建文件(与READ组合无意义,将使用默认).
        } FileMode;

    public:
        FileStream();
        ~FileStream();

        // put out stream
        FileStream& operator<<( const bool& val );
        FileStream& operator<<( const u8& val );
        FileStream& operator<<( const s8& val );
        FileStream& operator<<( const u16& val );
        FileStream& operator<<( const s16& val );
        FileStream& operator<<( const u32& val );
        FileStream& operator<<( const s32& val );
        FileStream& operator<<( const u64& val );
        FileStream& operator<<( const s64& val );
        FileStream& operator<<( const f32& val );
        FileStream& operator<<( const f64& val );
        FileStream& operator<<( const tchar* data );

        // get in stream
        void        operator>>( bool& val );
        void        operator>>( u8& val );
        void        operator>>( s8& val );
        void        operator>>( u16& val );
        void        operator>>( s16& val );
        void        operator>>( u32& val );
        void        operator>>( s32& val );
        void        operator>>( u64& val );
        void        operator>>( s64& val );
        void        operator>>( f32& val );
        void        operator>>( f64& val );
        void        operator>>( String& val );

    public:

        //-----------------------------------------------
        // 基本操作.

        /**
         * @brief 打开文件流
         *        文件打开就被本进程锁定.
         */
        bool        open( const tchar* filePath, integer mode = FM_ReadWrite );
        
        /**
         * @brief 判断文件流是否成功打开.
         */
        bool        isOpen() const;
        
        /**
         * @brief 获得文件的句柄.
         */
        int			getFd();

        /**
         * @brief 清除流
         */
        void        clear();
        
        /**
         * @brief 关闭打开的文件流.
         */
        void        close();
        
        /**
         * @brief 强制将缓存的内容写入文件.
         */
        void        flush();
        
        /**
         * @brief 获得文件路径
         */
        const tchar* getPath() const;

        /**
         * @brief 设置stream当前位置.
         */
        bool        seek( integer offset, SeekOrigin origin = SO_Begin );
        
        /**
         * @brief 返回当前的文件位置.
         */
        integer     tell() const;

        /**
         * @brief 设置流字节长度
         */
        void        setByteSize( uinteger newSize );
        
        /**
         * @brief 获得流字节长度
         */
        integer     getByteSize();
        
        /**
         * @brief 获得流的字符长度, ANSI下==流字节长度.
         */
        integer     getLength();

        //-----------------------------------------------
        // 读取/写; 返回写入/读取的字节数

        /**
         * @brief 读取length个字符到pBuf中
         * @return
         *      返回读取的字符数; -1为错误.
         */
        integer     read( OUT tchar* pBuf, integer length );

        /**
         * @brief 读取一行(读取的数据为内存数据无任何编码),换行符delim只能为 "\r\n"或 任意单字符.
         *        读取的内容不包括换行符.
         * @param isEndLine 指明读取的数据是否为一行的结束.
         * @return
         *      返回读取的字符数; -1为错误.
         */
        integer     readLine( OUT char* pBuf, integer length, OUT bool* isEndLine = NULL, const char* delim = "\r\n" );
        
        /**
         * @brief 读取length个字节到pBuf中
         * @return
         *      返回读取的字节数; -1为错误.
         */
        integer     readByte( OUT void* pBuf, integer lengthOfByte );
        template<class T>
        integer     readByte( OUT T& val )  { return readByte(&val, sizeof(T)); }

        /**
         * @brief 写入length个字符到流中
         * @return
         *      返回写入的字符数; -1为错误.
         */
        integer     write( const tchar* pBuf, integer length );
        
        /**
         * @brief 写入length个字节到流中
         * @return
         *      返回写入的字节数; -1为错误.
         */
        integer     writeByte( const void* pBuf, integer lengthOfByte );
        template<class T>
        integer     writeByte( const T& val )  { return writeByte(&val, sizeof(T)); }

        /**
         * @brief 从流的当前位置读取数据并写入stream中
         * @return
         *      返回读取的字节数; -1为错误.
         */
        integer     readStream( OUT FileStream& stream, integer lengthOfByte );
        
        /**
         * @brief 从stream的当前位置开始读数据至当前流中.
         * @return
         *      返回写入的字节数; -1为错误.
         */
        integer     writeStream( FileStream& stream, integer lengthOfByte );
        
        /**
         * @brief 中间截断流; 在流的当前位置开始, 截断length长度的流.
         */
        bool        splitStream( integer lengthOfByte );

        /**
         * @brief 插入流; 在流的当前位置插入pStream的当前位置开始的length长度的流.
         */
        bool        insertStream( FileStream& pStream, integer lengthOfByte );

        /**
         * @brief 附加数据到流的末尾; 在流的尾部附加pStream的当前位置开始的length长度的流.
         */
        bool        appendStream( FileStream& pStream, integer lengthOfByte );
    };
	

} // end namespace hyc

#endif // end of define __BP_FILESTREAM_H_20101202112706__
