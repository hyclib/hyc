//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  the utility of file
//

#ifndef __BP_FILEUTILS_H_20101206134608__
#define __BP_FILEUTILS_H_20101206134608__

#include "../container/array.h"

namespace hyc
{
    //-----------------------------------------------
    //!  文件属性
    typedef enum {
        FileAttribute_Error     = -1,
        FileAttribute_Normal    = 0x00,     //! 正常
        FileAttribute_ReadOnly  = 0x01,     //! 只读
        FileAttribute_Hidden    = 0x02,     //! 隐藏
        FileAttribute_System    = 0x04,     //! 系统文件
        FileAttribute_SubDir    = 0x10,     //! 文件夹
        FileAttribute_Arch      = 0x20,     //! 存档
    } FileAttribute;

    //-----------------------------------------------
    //!  文件工具
    class HYC_API FileUtils
    {
    public:
        /*---------------------------
          文件操作
        ---------------------------*/
        static bool     FileAssure( const tchar* pszFile ); //! 保证文件存在, 若不存在新建; 文件存在返回true
        static bool     FileIsExist( const tchar* pszFile );
        static bool     FileCreate( const tchar* pszFile );
        static bool     FileCreateRecursive( const tchar* pszFile );
        static bool     FileRemove( const tchar* pszFile );
        static bool     FileSetSize( const tchar* pszFile, s64 newSize );
        static s64      FileGetSize( const tchar* pszFile );
        static bool     FileCopy( const tchar* pszSrcFile, const tchar* pszDestFile );

        /*---------------------------
          文件夹操作
        ---------------------------*/
        static bool     DirAssure( const tchar* pszDir ); //! 保证目录存在, 若不存在新建; 文件存在返回true
        static bool     DirIsExist( const tchar* pszDir );
        static bool     DirCreate( const tchar* pszDir );
        static bool     DirCreateRecursive( const tchar* pszDir );
        static bool     DirRemoveRecursive( const tchar* pszDir ); //! [!WARNING]将会删除整个文件夹
        static String   DirGetCurrent();
        static bool     DirSetCurrent( const tchar* pszDir ); //! 设置当前process的工作目录
        static const String&   DirGetRuningDir();   //! 获得运行文件所在的位置

        /*---------------------------
          文件夹遍历; (使用通配符匹配)
          遍历文件夹时, 不会将arrFiles/arrDirs清空, 并且结果路径中包含pszDir
        ---------------------------*/
        static void     DirExplorer( const tchar* pszDir, OUT Array<String>* pArrFiles, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern = _T("*") );
        static void     DirExplorerFilesRecursive( const tchar* pszDir, OUT Array<String>* pArrFiles, const tchar* pszSearchPattern = _T("*") );
        static void     DirExplorerDirsRecursive( const tchar* pszDir, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern = _T("*") );
        //! 搜索空文件夹
        static void     DirExplorerDirsRecursiveEmpty( const tchar* pszDir, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern = _T("*") );

        /*---------------------------
          共同操作
        ---------------------------*/
        static bool     Move( const tchar* srcPath, const tchar* newPath );
        static void     PathFormat( OUT String* pszPath );   //! 适配各平台路径分隔符不同
        static void     PathFormat_File( OUT String* pszFilePath );   //! 包含文件名的分隔符格式化 (不以'/'结尾)
        static bool     PathIsRelative( const tchar* pszPath );
        static String   PathBundle( const tchar* path );        //! 使用相对路径来获得运行的绝对路径.

        static s64      GetCreateTime( const tchar* pszPath );  //! 获得创建时间
        static s64      GetWriteTime( const tchar* pszPath );   //! 获得修改时间
        static s64      GetAccessTime( const tchar* pszPath );  //! 获得最后访问时间; (需OS支持, mac os 不支持)
        static FileAttribute    GetFileAttribute( const tchar* pszPath );   //! 获得文件/文件夹属性

        /*---------------------------
          !分割路径为: 路径 / 文件基本名 / 扩展名
        ---------------------------*/
        static void     PathSplitName( const tchar* fullName, OUT String& path, OUT String& baseName, OUT String& extentionName );
        static void     PathSplitName( const tchar* fullName, OUT String& path, OUT String& fileName );

    };

} // end namespace hyc

#endif // end of define __BP_FILEUTILS_H_20101206134608__
