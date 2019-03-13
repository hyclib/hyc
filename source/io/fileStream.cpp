//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#include "../../include/io/fileStream.h"
#include "../../include/utils/fileUtils.h"
#include "../../include/utils/stringUtils.h"
#include "../../include/container/fixArray.h"

#if CN_PLATFORM == CN_PLATFORM_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <fcntl.h>

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   pragma warning(disable: 4996)  // func_s
#   pragma warning(disable: 4018)  // >=
#endif

namespace hyc
{
    // define file mark
#define FSTREAM     FILE

    // redefine function
#if defined(UNICODE) || defined(_UNICODE)
#   define FOPEN    fopen//_wfopen_s
#else
#   define FOPEN    fopen
#endif // defined(UNICODE) || defined(_UNICODE)


    //----------------------------------------------------------------------------------------------------
    //*	FileStream::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class FileStream::PrivateClass
    {
    public:
        PrivateClass()
        {
            m_file      = NULL;
            m_isOpen    = false;
            m_openMode  = 0;
            //m_isLock    = false;
        }

        // 获得c打开文件模式字符串
        void _getOpenMode( cString& mode, const integer& om )
        {
            switch (om)
            {
            case FM_Read:
                mode = ("rt");
                break;
            case FM_Read|FM_Binary:
                mode = ("rb");
                break;
            case FM_ReadWrite:
                mode = ("rt+");
                break;
            case FM_ReadWrite|FM_New:
                mode = ("wt+");
                break;
            case FM_ReadWrite|FM_Binary:
                mode = ("rb+");
                break;
            case FM_ReadWrite|FM_Binary|FM_New:
                mode = ("wb+");
                break;
// 
//             case FM_Read|FM_ReadWrite:
//                 mode = ("at+");
//                 break;
//             case FM_Read|FM_ReadWrite|FM_New:
//                 mode = ("wt+");
//                 break;
//             case FM_Read|FM_ReadWrite|FM_Binary:
//                 mode = ("ab+");
//                 break;
//             case FM_Read|FM_ReadWrite|FM_Binary|FM_New:
//                 mode = ("wb+");
//                 break;
            default:
                mode = ("r");
                break;
            } // switch
        }

        // 判断模式是否新建文件
        bool _checkMode_new( const integer& mode )
        {
            return 0 != (FM_New & mode);
        }

        // 获得流大小, 并保留位置
        static integer _getSizeAndSavePos( FileStream* pStream )
        {
            if (pStream && pStream->isOpen())
            {
                integer pos = pStream->tell();
                pStream->seek(0, FileStream::SO_End);
                integer len = pStream->tell();
                pStream->seek(pos);
                return len;
            }
            return -1;
        }

        // 数据成员
        FSTREAM* 		m_file;	    ///< 文件对象指针
        String			m_path;	    ///< 文件位置
        bool            m_isOpen;   ///< 文件是否打开
        integer             m_openMode; ///< 打开模式
// #if CN_PLATFORM == CN_PLATFORM_WINDOWS
//         bool            m_isLock;
// #endif
    };
    
    //----------------------------------------------------------------------------------------------------
    //*	FileStream
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    FileStream::FileStream()
    {
//         static bool bSetChinese = false;
//         if (!bSetChinese)
//         {
//             setlocale(LC_ALL,"Chinese-simplified"); // 支持中文目录
//         bSetChinese = true;
//         }

        PRIVATE_CLASS_CREATE;
    }
    
    //-----------------------------------------------
    FileStream::~FileStream()
    {
        close();
// #if CN_PLATFORM == CN_PLATFORM_WINDOWS
//         PRIVATE_CLASS_GET(p);
//         if (p->m_isLock)
//             unlock();
// #endif
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    void FileStream::flush()
    {
        PRIVATE_CLASS_GET(p);
        fflush(p->m_file);
    }

    //-----------------------------------------------
    bool FileStream::open( const tchar* filePath, integer mode /*= FM_ReadWrite */ )
    {
        PRIVATE_CLASS_GET(p);
        
        close();

        // 判断是否新建模式
        if (!p->_checkMode_new(mode)
            && !FileUtils::FileIsExist(filePath))
        {
            //SYSLOG(_T("FileStream::open"), _T("file is not exist"));
            return false;
        }

        // 打开模式
        cString openMode;
        p->_getOpenMode(openMode, mode);
        
        // 打开
#if defined(_UNICODE) || defined(UNICODE)
        cString str;
        StringUtils::Wide2narrow( filePath, StringUtils::GetLength(filePath), str );
        p->m_file = FOPEN(str.c_str(), openMode.c_str());
#else
        p->m_file = FOPEN(filePath, openMode.c_str());
#endif // defined(_UNICODE) || defined(UNICODE)
        if (NULL != p->m_file)
        {
#if CN_PLATFORM == CN_PLATFORM_LINUX
        	// 改变权限.
        	int fd = fileno(p->m_file);
        	fchmod(fd, 777);
#endif

            p->m_path = filePath;
            p->m_openMode = mode;
            p->m_isOpen = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    int FileStream::getFd()
    {
    	PRIVATE_CLASS_GET(p);
    	return fileno(p->m_file);
    }

    //-----------------------------------------------
    bool FileStream::isOpen() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_isOpen;
    }

    //-----------------------------------------------
    void FileStream::close()
    {
        PRIVATE_CLASS_GET(p);
        if (isOpen())
        {
            flush();
            if ( 0 != fclose(p->m_file) )    // 关闭失败
            {
                //SYSLOG(_T("FileStream::close"), _T("failed in close file!"));
            }
            p->m_path.clear();
            p->m_isOpen = false;
        }
    }

    //-----------------------------------------------
    void FileStream::clear()
    {
        seek(0, SO_Begin);
        setByteSize(0);
    }

    //-----------------------------------------------
    const tchar* FileStream::getPath() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_path.c_str();
    }
// 
//     //-----------------------------------------------
//     void FileStream::lock( integer position, integer length )
//     {
//         // TODO: 未实现
//     }
// 
//     //-----------------------------------------------
//     void FileStream::unlock( integer position, integer length )
//     {
//         // TODO: 未实现_fread_nolock
//     }

    //-----------------------------------------------
    bool FileStream::seek( integer offset, SeekOrigin origin /*= SO_Begin */ )
    {
        PRIVATE_CLASS_GET(p);
        s32 sd = SO_Begin;
        switch (origin)
        {
        case SO_Begin:
            sd = SEEK_SET;
            break;
        case SO_Current:
            sd = SEEK_CUR;
            break;
        case SO_End:
            sd = SEEK_END;
            break;
        }
        return 0 == fseek(p->m_file, offset, sd);
    }

    //-----------------------------------------------
    integer FileStream::tell() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return ftell(p->m_file);
    }

    //-----------------------------------------------
    void FileStream::setByteSize( uinteger newSize )
    {
        PRIVATE_CLASS_GET(p);

        if (isOpen())
        {
            // 记录信息
            integer mode = p->m_openMode & ~FM_New;
            String path = getPath();
            integer pos = tell();

            // 关闭
            close();

            // 设置大小并重新
            if (FileUtils::FileSetSize(path.c_str(), newSize))
            {
                if (open(path.c_str(), mode))
                    seek(pos, SO_Begin);
            }
        } // if
    }

    //-----------------------------------------------
    integer FileStream::getByteSize()
    {
        PRIVATE_CLASS_GET(p);
        return p->_getSizeAndSavePos(this);
    }

    //-----------------------------------------------
    integer FileStream::getLength()
    {
#if defined(_UNICODE) || defined(UNICODE)
        return (integer)ceil(getByteSize() / 2.0);
#else
        return getByteSize();
#endif // defined(_UNICODE) || defined(UNICODE)
    }

    //-----------------------------------------------
    integer FileStream::readByte( OUT void* pBuf, integer length )
    {
        PRIVATE_CLASS_GET(p);

        if (NULL == pBuf)
        {
            //SYSLOG(_T("FileStream::readByte"), _T("parameter error"));
            return /*false*/-1;
        }

        if (0 >= length)
        {
            return /*true*/0;
        }

        if (!isOpen())
        {
            //SYSLOG(_T("FileStream::readByte"), _T("file isn't opend"));
            return /*false*/-1;
        }

        //seek(0);
        return fread( pBuf, 1, length, p->m_file );
    }

    //-----------------------------------------------
    integer FileStream::readStream( OUT FileStream& pStream, integer length )
    {
        //-----------------------------------------------
        // check parameter

        if (0 >= length)   return /*true*/0;
        if (!pStream.isOpen() || !this->isOpen())
        {
            //SYSLOG(_T("FileStream::readByte stream"), _T("stream parameter error"));
            return /*false*/-1;
        }

        // 判断长度
        integer curPos = tell();
        integer sLen = getByteSize();
        if (sLen - curPos < length)
        {
            //SYSLOG(_T("FileStream::readByte stream"), _T("len parameter error"));
            return /*false*/-1;
        }

        FixArray<Byte>  buf(length > HYC_COMPILE_FILE_SEGMENT_BUF_SIZE ? HYC_COMPILE_FILE_SEGMENT_BUF_SIZE : length);
        integer pos = 0;
        integer size = 0;
        while (pos < length)
        {
            size = length - pos;
            size = size > buf.size() ? buf.size() : size;

            if (size != this->readByte( &buf[0], size ))
                return -1;

            if (size != pStream.writeByte( &buf[0], size ))
                return -1;

            pos += size;
        }

        return length;
    }

    //-----------------------------------------------
    integer FileStream::read( OUT tchar* pBuf, integer length )
    {
        PRIVATE_CLASS_GET(p);

        if (NULL == pBuf)
        {
            //SYSLOG(_T("FileStream::readByte"), _T("parameter error"));
            return /*false*/-1;
        }

        if (0 >= length)
        {
            return /*true*/0;
        }

        if (!isOpen())
        {
            //SYSLOG(_T("FileStream::readByte"), _T("file isn't opend"));
            return /*false*/-1;
        }

        //seek(0);
#if defined(_UNICODE) || defined(UNICODE)
        char* pChBuf = (char*)MemoryAlloc::Malloc( length*sizeof(wchar_t)+1 );
        memset( pChBuf, 0, length*sizeof(wchar_t)+1 );

        integer pos = tell();
        integer len = fread_s( pChBuf, length*sizeof(wchar_t), 1, length*sizeof(wchar_t), p->m_file );
        seek( pos+len, SO_Begin );

        uinteger tt = length;
        if ( !StringUtils::Narrow2wide( pChBuf, (uinteger)len, pBuf, tt ) )
        {
            MemoryAlloc::Free( pChBuf );
            return /*false*/-1;
        }
        MemoryAlloc::Free( pChBuf );
        return len;
#else
        integer pos = tell();
        integer len = fread( pBuf, 1, length, p->m_file );
        seek( pos+len, SO_Begin );
        return len;
#endif // defined(_UNICODE) || defined(UNICODE)

       
    }

    //-----------------------------------------------
    integer FileStream::readLine( OUT char* pBuf, integer length, OUT bool* isEndLine, const char* delim )
    {
        if (!delim)
            return -1;

        if (isEndLine)
            *isEndLine = false;

        PRIVATE_CLASS_GET(p);
        FSTREAM* stream = p->m_file;

        size_t len = strlen(delim);
        if (len > 2)
            return -1;

        if (1 == len)
        {
#ifndef CN_PLATFORM_MOBILE
            register
#endif
            int c;
#ifndef CN_PLATFORM_MOBILE
            register
#endif
            char *cs;

            cs = (char*)pBuf;
            const char de = delim[0];

            int bufLen = length;

            while(--bufLen>=0 &&(c = getc(stream))!=EOF)
            {
                if (c == de)
                {
                    if (isEndLine)
                        *isEndLine = true;
                    break;
                }
                *cs++ = c;
            }

            return cs-(char*)pBuf;
        }
        else
        {
#ifndef CN_PLATFORM_MOBILE
            register
#endif
            int c;
#ifndef CN_PLATFORM_MOBILE
            register
#endif
            int c1 = 0;
#ifndef CN_PLATFORM_MOBILE
            register
#endif
            char *cs;

            cs = (char*)pBuf;
            const char de1 = delim[0];
            const char de2 = delim[1];

            int bufLen = length;

            while(--bufLen>=0 && (c = getc(stream))!=EOF)
            {
                if (c1 == 0)
                {
                    if (c == de1)
                    {
                        c1 = de1;
                        ++bufLen;
                        continue;
                    }

                    *cs++ = c;
                }
                else
                {
                    if (c == de2)
                    {
                        if (isEndLine)
                            *isEndLine = true;
                        break;
                    }
                    else
                    {
                        *cs++ = c1;
                        
                        if (c == de1)
                        {
                            c1 = de1;
                            ++bufLen;
                            continue;
                        }

                        c1 = 0;

                        if (--bufLen<0)
                        {
                            fseek(stream, -1, SEEK_CUR);
                            break;
                        }

                        *cs++ = c;
                    }
                }
            }

            return cs-(char*)pBuf;
        } // if..else.
    }

    //-----------------------------------------------
    integer FileStream::write( const tchar* pBuf, integer length )
    {
        PRIVATE_CLASS_GET(p);

        if (NULL == pBuf)
        {
            //SYSLOG(_T("FileStream::writeByte"), _T("parameter error"));
            return /*false*/-1;
        }

        if (0 >= length)
        {
            return /*true*/0;
        }

        if (!isOpen())
        {
            //SYSLOG(_T("FileStream::writeByte"), _T("file isn't opend"));
            return /*false*/-1;
        }
#if defined(_UNICODE) || defined(UNICODE)
        char* pChBuf = (char*)MemoryAlloc::Malloc( length*sizeof(wchar_t)+1 );
        memset( pChBuf, 0, length*sizeof(wchar_t)+1 );
        uinteger tt = length*sizeof(wchar_t);
        if ( !StringUtils::Wide2narrow( pBuf, length, pChBuf, tt ) )
        {
            MemoryAlloc::Free( pChBuf );
            return /*false*/-1;
        }

        integer len = StringUtils::GetLength(pChBuf);

        len = fwrite( pChBuf, 1, len, p->m_file );
        MemoryAlloc::Free( pChBuf );
        return len;

#else
        return fwrite( pBuf, 1, length, p->m_file );
#endif // defined(_UNICODE) || defined(UNICODE)
    }

    //-----------------------------------------------
    integer FileStream::writeByte( const void* pBuf, integer length )
    {
        PRIVATE_CLASS_GET(p);

        if (NULL == pBuf)
        {
            //SYSLOG(_T("FileStream::writeByte"), _T("parameter error"));
            return /*false*/-1;
        }

        if (0 >= length)
        {
            return /*true*/0;
        }

        if (!isOpen())
        {
            //SYSLOG(_T("FileStream::writeByte"), _T("file isn't opend"));
            return /*false*/-1;
        }

        return fwrite( pBuf, 1, length, p->m_file );
    }

    //-----------------------------------------------
    integer FileStream::writeStream( FileStream& pStream, integer length )
    {
        //-----------------------------------------------
        // check parameter

        if (0 >= length)   return /*true*/0;
        if (!pStream.isOpen() || !this->isOpen())
        {
            //SYSLOG(_T("FileStream::writeByte stream"), _T("stream parameter error"));
            return /*false*/-1;
        }

        // 判断长度
        integer bufCurPos = pStream.tell();
        integer sLen = pStream.getByteSize();
        if (length > sLen - bufCurPos)
        {
            //SYSLOG(_T("FileStream::writeByte stream"), _T("offset or len parameter error"));
            return /*false*/-1;
        }

        //-----------------------------------------------
        // writeByte
        FixArray<Byte>  buf( length > HYC_COMPILE_FILE_SEGMENT_BUF_SIZE ? HYC_COMPILE_FILE_SEGMENT_BUF_SIZE : length );

        integer pos = 0;
        integer size = 0;
        while (pos < length)
        {
            size = length - pos;
            size = size > buf.size() ? buf.size() : size;

            if (size != pStream.readByte( &buf[0], size ))
                return -1;

            if (size != this->writeByte( &buf[0], size ))
                return -1;

            pos += size;
        }

        return length;     
    }

    //-----------------------------------------------
    bool FileStream::splitStream( integer length )
    {
        //-----------------------------------------------
        // check parameter

        if ( 0 >= length )  return true;
        if ( !isOpen() )
        {
            //SYSLOG(_T("FileStream::splitStream"), _T("file isn't open"));
            return false;
        }

        integer pos = tell();
        integer fLen = getByteSize();

        if ( length > fLen - pos )
        {
            //SYSLOG(_T("FileStream::splitStream"), _T("len or offset parameter error"));
            return false;
        }

        //-----------------------------------------------
        // split

        //  缓冲

        // 计算缓存大小
        integer bufferLen = fLen - pos - length;
        assert( bufferLen > 0 );

        //-----------------------------------------------
        // buf长度为0
        if ( 0 >= bufferLen )
        {
            setByteSize( fLen-length );
            return true;
        }
        //-----------------------------------------------
        // buf较小, 使用内存
        else if ( 1024 * 2 >= bufferLen )
        {
            FixArray<hyc::Byte> buf(bufferLen);
            if ( buf.size() <= 0 )  return false;

            // 读末尾数据
            seek( pos+length, SO_Begin );
            if ( buf.size() != readByte( &buf[0], buf.size() ) )
            {
                return false;
            }

            // 截断
            seek( pos, SO_Begin );
            setByteSize( fLen-length );
            if ( buf.size() != writeByte( &buf[0], buf.size() ) )
            {
                return false;
            }
            
            return true;
        }
        //-----------------------------------------------
        // buf较大, 使用临时文件
        else
        {
            // 计算唯一临时名称
            String bufferName;
            {
                String bufferName2;
                FileUtils::PathSplitName( this->getPath(), bufferName, bufferName2 );
                bufferName = bufferName + _T("_$") + bufferName2 + _T(".tmp");
                s32 i = 0;
                tchar tempBuffer[15] = {0};
                while ( FileUtils::FileIsExist(bufferName.c_str()) )
                {
                    tsprintf( tempBuffer, _T("%d"), i++ );
                    bufferName = bufferName + _T("_$") + bufferName2 + tempBuffer + _T(".tmp");
                } // while
            } //

            // 建立缓冲并暂存数据
            FileStream buffer;
            if ( !buffer.open(bufferName.c_str(), FM_Read|FM_ReadWrite|FM_Binary|FM_New) )
            {
                //SYSLOG(_T("FileStream::splitStream"), _T("open temp buffer error"));
                return false;
            }
            seek( pos+length, SO_Begin );
            if ( bufferLen != buffer.writeStream(*this, bufferLen) )
            {
                //SYSLOG(_T("FileStream::splitStream"), _T("writeByte temp buffer error"));
                return false;
            }

            //  数据连接
            seek( pos, SO_Begin );
            setByteSize( fLen-length );
            buffer.seek( 0, SO_Begin );
            if ( bufferLen != writeStream(buffer, bufferLen) )
            {
                // 清除buffer
                buffer.close();
                FileUtils::FileRemove( bufferName.c_str() );

                //SYSLOG(_T("FileStream::splitStream"), _T("readByte error"));
                return false;
            }

            // 清除buffer
            buffer.close();
            FileUtils::FileRemove( bufferName.c_str() );

            return true;
        } // if..else
    }

    //-----------------------------------------------
    bool FileStream::insertStream( FileStream& pStream, integer length )
    {
        //-----------------------------------------------
        // check condition

        if ( 0 >= length ) return true;

        if (!isOpen() || !pStream.isOpen())
        {
            //SYSLOG(_T("FileStream::insertStream"), _T("file isn't opend"));
            return false;
        }

        integer pos = pStream.tell();
        integer fLen = pStream.getByteSize();
        if (length > fLen-pos)
        {
            //SYSLOG(_T("FileStream::insertStream"), _T("offset or len parameter error"));
            return false;
        }

        integer curPos = tell();
        fLen = getByteSize();

        //-----------------------------------------------
        //  缓冲

        // 计算当前位置后的数据大小
        uinteger bufferLen = fLen - curPos;

        //-----------------------------------------------
        // buf长度为0
        if ( 0 >= bufferLen )
        {
            // 截断
            seek( curPos, SO_Begin );

            if ( length != writeStream( pStream, length ) )
            {
                return false;
            }

            return true;
        }
        //-----------------------------------------------
        // buf较小, 使用内存
        else if ( 1024 * 2 >= bufferLen )
        {
            FixArray<hyc::Byte> buf(bufferLen);
            if ( buf.size() <= 0 )  return false;

            // 读末尾数据
            if ( buf.size() != readByte( &buf[0], buf.size() ) )
            {
                return false;
            }

            // 截断
            seek( curPos, SO_Begin );
            
            if ( length != writeStream( pStream, length ) )
            {
                return false;
            }

            if ( buf.size() != writeByte( &buf[0], buf.size() ) )
            {
                return false;
            }

            return true;
        }
        //-----------------------------------------------
        //  buf较大, 使用临时文件
        else
        {
            // 计算唯一临时名称
            String bufferName;
            {
                String bufferName2;
                FileUtils::PathSplitName( this->getPath(), bufferName, bufferName2 );
                bufferName = bufferName + _T("_$") + bufferName2 + _T(".tmp");
                s32 i = 0;
                tchar tempBuffer[15]={0};
                while ( FileUtils::FileIsExist(bufferName.c_str()) )
                {
                    tsprintf(tempBuffer, _T("%d"), i++);
                    bufferName = bufferName + _T("_$") + bufferName2 + tempBuffer + _T(".tmp");
                } // while
            } // 计算唯一临时名称

            // 建立缓冲并暂存数据
            FileStream buffer;
            if ( !buffer.open(bufferName.c_str(), FM_Read|FM_ReadWrite|FM_Binary|FM_New) )
            {
                //SYSLOG(_T("FileStream::insertStream"), _T("open temp buffer error"));
                return false;
            }
            if ( bufferLen != readStream( buffer, bufferLen ) )
            {
                //SYSLOG(_T("FileStream::insertStream"), _T("writeByte temp buffer error"));
                buffer.close();
                FileUtils::FileRemove( bufferName.c_str() );
                return false;
            }

            //-----------------------------------------------
            //  数据连接

            // 填充数据
            seek( curPos, SO_Begin );
            if ( length != writeStream(pStream, length ) )
            {
                //SYSLOG(_T("FileStream::insertStream"), _T("readByte error"));
                // 清除buffer
                buffer.close();
                FileUtils::FileRemove( bufferName.c_str() );
                return false;
            }
            buffer.seek( 0, SO_Begin );
            if ( bufferLen != writeStream(buffer, bufferLen) )
            {
                //SYSLOG(_T("FileStream::insertStream"), _T("readByte error"));
                // 清除buffer
                buffer.close();
                FileUtils::FileRemove( bufferName.c_str() );
                return false;
            }

            // 清除buffer
            buffer.close();
            FileUtils::FileRemove( bufferName.c_str() );
            return true;
        } // if..else
    }

    //-----------------------------------------------
    bool FileStream::appendStream( FileStream& pStream, integer length )
    {
        //-----------------------------------------------
        // check condition

        if ( 0 >= length )
            return true;

        if ( !isOpen() || !pStream.isOpen() )
        {
            //SYSLOG(_T("FileStream::appendStream"), _T("file isn't opend"));
            return false;
        }

        integer pos = pStream.tell();
        integer fLen = pStream.getByteSize();
        if ( length > fLen-pos )
        {
            //SYSLOG(_T("FileStream::appendStream"), _T("offset or len parameter error"));
            return false;
        }

        //-----------------------------------------------
        // writeByte

        seek( 0, SO_End );

        if ( length != writeStream(pStream, length) )
        {
            //SYSLOG(_T("FileStream::appendStream"), _T("writeByte error"));
            return false;
        }

        return true;    
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const bool& val )
    {
        writeByte( (s8*)&val, sizeof(bool) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const u8& val )
    {
        writeByte( (s8*)&val, sizeof(u8) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const s8& val )
    {
        writeByte( (s8*)&val, sizeof(s8) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const u16& val )
    {
        writeByte( (s8*)&val, sizeof(u16) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const s16& val )
    {
        writeByte( (s8*)&val, sizeof(s16) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const u32& val )
    {
        writeByte( (s8*)&val, sizeof(u32) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const s32& val )
    {
        writeByte( (s8*)&val, sizeof(s32) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const u64& val )
    {
        writeByte( (s8*)&val, sizeof(u64) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const s64& val )
    {
        writeByte( (s8*)&val, sizeof(s64) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const f32& val )
    {
        writeByte( (s8*)&val, sizeof(f32) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const f64& val )
    {
        writeByte( (s8*)&val, sizeof(f64) );
        return *this;
    }

    //-----------------------------------------------
    FileStream& FileStream::operator<<( const tchar* data )
    {
        integer len = StringUtils::GetLength(data);
        writeByte( &len, sizeof(integer) );
        write( data, len );
        return *this;
    }

    //-----------------------------------------------
    void FileStream::operator>>( bool& val )
    {
        readByte( (s8*)&val, sizeof(bool) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( u8& val )
    {
        readByte( (s8*)&val, sizeof(u8) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( s8& val )
    {
        readByte( (s8*)&val, sizeof(s8) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( u16& val )
    {
        readByte( (s8*)&val, sizeof(u16) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( s16& val )
    {
        readByte( (s8*)&val, sizeof(s16) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( u32& val )
    {
        readByte( (s8*)&val, sizeof(u32) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( s32& val )
    {
        readByte( (s8*)&val, sizeof(s32) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( u64& val )
    {
        readByte( (s8*)&val, sizeof(u64) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( s64& val )
    {
        readByte( (s8*)&val, sizeof(s64) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( f32& val )
    {
        readByte( (s8*)&val, sizeof(f32) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( f64& val )
    {
        readByte( (s8*)&val, sizeof(f64) );
    }

    //-----------------------------------------------
    void FileStream::operator>>( String& val )
    {
        val.clear();
        integer len = -1;
        if ( sizeof(integer) != readByte( &len, sizeof(integer) ) )
            return;

        FixArray<tchar>  buf(len+1);
        buf[len] = 0;

        if (len != read( buf.begin(), len ))
            return;

        val = buf.begin();
    }

//     //-----------------------------------------------
//     bool FileStream::lock()
//     {
//         PRIVATE_CLASS_GET(p);
// #if CN_PLATFORM == CN_PLATFORM_WINDOWS
// 
//         HANDLE hFile = ::CreateFile( getPath(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
//         if (INVALID_HANDLE_VALUE == hFile)  return false;
//         bool r = (TRUE == ::LockFile( hFile, 0, 0, 0, 0 )); 
//         ::CloseHandle( hFile );
//         p->m_isLock = r;
//         return r;
// #else
//         return 0 == flock(p->m_file, LOCK_EX);
// #endif
//     }
// 
//     //-----------------------------------------------
//     bool FileStream::unlock()
//     {
//         PRIVATE_CLASS_GET(p);
// #if CN_PLATFORM == CN_PLATFORM_WINDOWS
// 
//         HANDLE hFile = ::CreateFile( getPath(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
//         if (INVALID_HANDLE_VALUE == hFile)  return false;
//         bool r = (TRUE == ::UnlockFile( hFile, 0, 0, 0, 0 )); 
//         ::CloseHandle( hFile );
//         p->m_isLock = !r;
//         return r;
// #else
//         return 0 == flock(p->m_file, LOCK_UN);
// #endif
//     }
// 
} // end namespace hyc
