//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  FileUtils的win32平台实现
//

#ifndef __BP_FILEUTILS_HPP_20101202231523__
#define __BP_FILEUTILS_HPP_20101202231523__

#include <shellapi.h>
#include <io.h>
#include <sys/stat.h>

#include "../../include/utils/stringUtils.h"
#include "../../include/io/fileStream.h"

// 定义c的文件查找结构
#if defined(_UNICODE) || defined(UNICODE)
typedef struct _wfinddata_t _wfinddata_t;
#   define FINDDATA     _wfinddata_t
#   define FINDFIRST    _wfindfirst
#   define FINDNEXT     _wfindnext
#   define FINDCLOSE    _findclose
#   define TSTRCMP      wcscmp
#else
typedef struct _finddata_t _finddata_t;
#   define FINDDATA     _finddata_t
#   define FINDFIRST    _findfirst
#   define FINDNEXT     _findnext
#   define FINDCLOSE    _findclose
#   define TSTRCMP      strcmp
#endif // defined(_UNICODE) || defined(UNICODE)

    // define file mark
#define FSTREAM     FILE

#if defined(_UNICODE) || defined(UNICODE)
#   define FREMOVE _wremove
#   define RENAME  _wrename
#   define FOPEN    _wfopen_s
#else
#   define FREMOVE remove
#   define RENAME  rename
#   define FOPEN    fopen_s
#endif

namespace hyc
{

    //-----------------------------------------------
    //  can use in Unix system
    bool FileUtils::FileIsExist( const tchar* pszFile )
    {
        DWORD r = ::GetFileAttributes(pszFile);
        return 0 == (FILE_ATTRIBUTE_DIRECTORY & r)
            && 0xFFFFFFFF != r; 
    }

    //-----------------------------------------------
    bool FileUtils::FileCreate( const tchar* pszFile )
    {
        if (StringUtils::IsEmpty(pszFile))
            return false;

        HANDLE h = ::CreateFile( pszFile, 0, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );

        if ( INVALID_HANDLE_VALUE != h )
        {
            ::CloseHandle( h );
            return true;
        }
        else
            return false;    
    }

    //-----------------------------------------------
    bool FileUtils::FileRemove( const tchar* pszFile )
    {
        return 0 == FREMOVE(pszFile);
    }

    //-----------------------------------------------
    bool FileUtils::FileSetSize( const tchar* pszFile, s64 newSize )
    {
        if ( StringUtils::IsEmpty(pszFile) )
            return false;

        /* Open a file */
        HANDLE hFile = ::CreateFile(
            pszFile, 
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );

        if( hFile == INVALID_HANDLE_VALUE )
        {
            //SYSLOG(_T("UtilFile::FileSetSize"), _T("FileCreate error"));
            return false;
        }

        LARGE_INTEGER li;
        li.QuadPart = newSize;
        ::SetFilePointerEx( hFile, li, &li, FILE_BEGIN );
        ::SetEndOfFile( hFile );

        ::CloseHandle( hFile );

        return true;    
    }

    //-----------------------------------------------
    //  can use in Unix system
    s64 FileUtils::FileGetSize( const tchar* pszFile )
    {
        struct _stat64 statbuf;
        //FSTREAM* stream = NULL;

        //// open a file for update
        //FOPEN(&stream, pszFile, _T("rt"));
        //if ( NULL == stream )
        //    return -1;

        // get information about the file
#if defined(_UNICODE) || defined(UNICODE)
        cString str;
        StringUtils::Wide2narrow( pszFile, StringUtils::GetLength(pszFile), str );
        _stat64(str.c_str(), &statbuf);
#else
        _stat64(pszFile, &statbuf);

#endif // defined(_UNICODE) || defined(UNICODE)

        //fclose(stream);

        // display the information returned
        //if (statbuf.st_mode & S_IFCHR)  printf("Handle refers to a device.\n");
        //if (statbuf.st_mode & S_IFREG)  printf("Handle refers to an ordinary file.\n");
        //if (statbuf.st_mode & S_IREAD)  printf("User has read permission on file.\n");
        //if (statbuf.st_mode & S_IWRITE) printf("User has write permission on file.\n");

        //printf("Drive letter of file: %c\n", 'A'+statbuf.st_dev);
        //printf("Size of file in bytes: %ld\n", statbuf.st_size);
        //printf("Time file last opened: %s\n", ctime(&statbuf.st_ctime));

        return statbuf.st_size;
    }

    //-----------------------------------------------
    //  can use in Unix system
    bool FileUtils::DirIsExist( const tchar* pszDir )
    {
        DWORD r = ::GetFileAttributes(pszDir);
        return 0 != (FILE_ATTRIBUTE_DIRECTORY & r) 
            && 0xFFFFFFFF != r;
    }

    //-----------------------------------------------
    bool FileUtils::DirCreate( const tchar* pszDir )
    {
        return 1 == ::CreateDirectory(pszDir, NULL);
    }

    //-----------------------------------------------
    bool FileUtils::DirRemoveRecursive( const tchar* pszFile )
    {
        tchar	oldDir[MAX_PATH] = {END_STR}, newDir[MAX_PATH] = {END_STR};	// 新旧的目录
        DWORD	dwResult;

        if (StringUtils::IsEmpty(pszFile))
            return false;

        // 获得旧当前目录
        dwResult = ::GetCurrentDirectory( MAX_PATH, oldDir );
        if ( !dwResult || dwResult > MAX_PATH )
            return false;

        // 设置新目录并获得全部路径
        dwResult = SetCurrentDirectory( pszFile );
        if ( !dwResult )
            return false;

        dwResult = GetCurrentDirectory( MAX_PATH, newDir );
        if ( !dwResult || dwResult > MAX_PATH )
            return false;

        // 删除全部文件
        SHFILEOPSTRUCT fileop;
        fileop.hwnd     = NULL;
        fileop.wFunc    = FO_DELETE;
        fileop.fFlags   = FOF_NOCONFIRMATION | FOF_SILENT;
        fileop.pFrom    = newDir;
        fileop.pTo      = NULL;
        fileop.fAnyOperationsAborted    = false;
        fileop.hNameMappings            = NULL;
        fileop.lpszProgressTitle        = TEXT("正在删除文件");
        dwResult = ::SHFileOperation( &fileop );

        ::SetCurrentDirectory( oldDir );

        if ( !dwResult )
            return true;

        return false;
    }

    //-----------------------------------------------
    String FileUtils::DirGetCurrent()
    {
        int n = 0;
        tchar path[MAX_PATH];
        if ( 0 != (n=::GetCurrentDirectory(MAX_PATH, path)) )
        {
            if (n+1 < MAX_PATH)
            {
                path[n] = _T('/');
                path[n+1] = 0;
            }

            return path;
        }
        return _T("");
    }

    //-----------------------------------------------
    bool FileUtils::DirSetCurrent( const tchar* pszDir )
    {
        return 0 != ::SetCurrentDirectory(pszDir);
    }

    //-----------------------------------------------
    bool FileUtils::Move( const tchar* srcPath, const tchar* newPath )
    {
        if (!StringUtils::IsEmpty(srcPath) && !StringUtils::IsEmpty(newPath))
        {
            if (FileIsExist(newPath))
                return false;

            return (TRUE == ::MoveFile(srcPath, newPath));
        }
        return false;
    }



    //-----------------------------------------------
    void FileUtils::DirExplorer( const tchar* pszDir, OUT Array<String>* pArrFiles, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern /*= _T("*") */ )
    {

        if ( NULL == pszDir || (NULL == pArrDirs && NULL == pArrFiles) )   return;

        // 搜索路径构造
        String path = pszDir;
        PathFormat( &path );

        String searchPath = path;

        if ( NULL != pszSearchPattern ) searchPath += pszSearchPattern;
        else searchPath += _T('*');

        // 查找文件
        FINDDATA c_file;
        long hFile;
        if ( -1L != (hFile = FINDFIRST(searchPath.c_str(), &c_file)) )
        {
            // 第一次的查找结果为目录本身, 无需记录

            // 迭代
            do{
                // 判断是否是文件夹
                if ( 0 != (c_file.attrib & _A_SUBDIR) && pArrDirs )
                {
                    if ( 0 != TSTRCMP( _T("."), c_file.name ) && 0 != TSTRCMP( _T(".."), c_file.name ) )    // 除去 "."/".."两个目录
                        pArrDirs->push_back( path + c_file.name );
                }
                else if ( pArrFiles )
                {
                    if ( 0 != TSTRCMP( _T("."), c_file.name ) && 0 != TSTRCMP( _T(".."), c_file.name ) )    // 除去 "."/".."两个目录
                        pArrFiles->push_back( path + c_file.name );
                }

            } while ( 0 == FINDNEXT(hFile, &c_file) );

            FINDCLOSE(hFile);
        } // if
    }
    //-----------------------------------------------
    s64 FileUtils::GetCreateTime( const tchar* pszPath )
    {
        s64 sTime = -1;

        if ( NULL == pszPath )   return sTime;

        // 查找文件
        struct FINDDATA c_file;
        long hFile;
        if ( -1L != (hFile = FINDFIRST(pszPath, &c_file)) )
        {
            sTime = c_file.time_create;
            FINDCLOSE(hFile);
        } // if

        return sTime;
    }

    //-----------------------------------------------
    s64 FileUtils::GetWriteTime( const tchar* pszPath )
    {
        s64 sTime = -1;

        if ( NULL == pszPath )   return sTime;

        // 查找文件
        struct FINDDATA c_file;
        long hFile;
        if ( -1L != (hFile = FINDFIRST(pszPath, &c_file)) )
        {
            sTime = c_file.time_write;
            FINDCLOSE(hFile);
        } // if

        return sTime;
    }

    //-----------------------------------------------
    s64 FileUtils::GetAccessTime( const tchar* pszPath )
    {
        s64 sTime = -1;

        if ( NULL == pszPath )   return sTime;

        // 查找文件
        struct FINDDATA c_file;
        long hFile;
        if ( -1L != (hFile = FINDFIRST(pszPath, &c_file)) )
        {
            sTime = c_file.time_access;
            FINDCLOSE(hFile);
        } // if

        return sTime;
    }

    //-----------------------------------------------
    FileAttribute FileUtils::GetFileAttribute( const tchar* pszPath )
    {
        FileAttribute attri = FileAttribute_Error;

        if ( NULL == pszPath )   return attri;

        // 查找文件
        struct FINDDATA c_file;
        long hFile;
        if ( -1L != (hFile = FINDFIRST(pszPath, &c_file)) )
        {
            attri = (FileAttribute)c_file.attrib;
            FINDCLOSE(hFile);
        } // if

        return attri;
    }

    //-----------------------------------------------
    const String& FileUtils::DirGetRuningDir()
    {
        static String str;
        static bool r = true;
        if (r)
        {
            TCHAR path[MAX_PATH] = {0};
            GetModuleFileName( GetModuleHandle(NULL), path, MAX_PATH-1);
            str = path;
            str = str.substr(0, str.find_last_of('\\')+1);

            r = false;
        }

        return str;
    }

    //-----------------------------------------------
    bool FileUtils::FileCopy( const tchar* pszSrcFile, const tchar* pszDestFile )
    {
        if ( NULL == pszSrcFile || NULL == pszDestFile )   return NULL;
        return 0 != ::CopyFile( pszSrcFile, pszDestFile, FALSE );
//         if ( NULL == pszSrcFile || NULL == pszDestFile )   return NULL;
// 
//         FileStream fs;
//         if ( fs.open( pszSrcFile, FileStream::FM_Binary|FileStream::FM_Read ) )
//         {
//             if (FileUtils::FileIsExist(pszDestFile))
//             {
//                 if (!FileUtils::FileRemove(pszDestFile))
//                     return false;
//             }
// 
//             FileStream fsDest;
//             if (!fsDest.open( pszDestFile, FileStream::FM_Binary|FileStream::FM_New|FileStream::FM_ReadWrite ))
//                 return false;
// 
//             if ( fs.getByteSize() != fs.readStream( fsDest, fs.getByteSize() ) )
//                 return false;
// 
//             return true;
//         }
// 
//         return false;
    }

} // end namespace hyc

#endif // end of define __BP_FILEUTILS_HPP_20101202231523__
