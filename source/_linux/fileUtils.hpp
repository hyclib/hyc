//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// business logic layer network proxy
// author:       lipengxiang
// description:  FileUtils的linux实现
//

#ifndef __BP_FILEUTILS_HPP_20110121145907__
#define __BP_FILEUTILS_HPP_20110121145907__

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
//#include <glob.h>
#include <sys/stat.h>
#include "../../include/container/fixArray.h"
#include "../../include/io/fileStream.h"
#include "fileUtils_helper.hpp"

namespace hyc
{
	//-----------------------------------------------
	//  can use in Unix system
	bool FileUtils::FileIsExist( const tchar* pszFile )
	{
		if (DirIsExist( pszFile ))	return false;
		return 0 == access( pszFile, F_OK );
	}

    //-----------------------------------------------
    bool FileUtils::FileCreate( const tchar* pszFile )
    {
        if (StringUtils::IsEmpty(pszFile))
            return false;

        int fd = creat( pszFile,
        		S_IRWXU | S_IRUSR | S_IWUSR | S_IXUSR |
        		S_IRWXG | S_IRGRP | S_IWGRP | S_IXGRP |
        		S_IRWXO | S_IROTH | S_IWOTH | S_IXOTH );

        if (fd > 0)
        {
        	return 0 == close(fd);
        }

        return false;
    }

    //-----------------------------------------------
    bool FileUtils::FileRemove( const tchar* pszFile )
    {
    	if (StringUtils::IsEmpty(pszFile))
    		return false;

    	return 0 == unlink( pszFile );
    }

    //-----------------------------------------------
    bool FileUtils::FileSetSize( const tchar* pszFile, s64 newSize )
    {
        if ( StringUtils::IsEmpty(pszFile) )
            return false;

        return 0 == truncate( pszFile, newSize );
    }

    //-----------------------------------------------
    //  can use in Unix system
    s64 FileUtils::FileGetSize( const tchar* pszFile )
    {
        struct stat statbuf;
        if (-1 == stat(pszFile, &statbuf))	return -1;
        return statbuf.st_size;
    }

    //-----------------------------------------------
    //  can use in Unix system
    bool FileUtils::DirIsExist( const tchar* pszDir )
    {
    	struct stat statbuf;
        if (-1 == stat(pszDir, &statbuf))
        	return false;

        return 0 != S_ISDIR( statbuf.st_mode );
    }

    //-----------------------------------------------
    bool FileUtils::DirCreate( const tchar* pszDir )
    {
    	return 0 == mkdir( pszDir,
    			S_IRWXU | S_IRUSR | S_IWUSR | S_IXUSR |
    			S_IRWXG | S_IRGRP | S_IWGRP | S_IXGRP |
    			S_IRWXO | S_IROTH | S_IWOTH | S_IXOTH );
    }

    //-----------------------------------------------
    bool FileUtils::DirRemoveRecursive( const tchar* pszFile )
    {
        if (StringUtils::IsEmpty(pszFile))
            return false;

        String str = _T("rm -rf ");
        str += pszFile;
        return -1 != system( str.c_str() );
    }

    //-----------------------------------------------
    String FileUtils::DirGetCurrent()
    {
        long maxbufsize = pathconf( ".", _PC_PATH_MAX );
        if (maxbufsize == -1)	return _T("");

        FixArray<tchar> buf(maxbufsize+1);
        buf[maxbufsize] = 0;

        getcwd( buf.begin(), (size_t)maxbufsize );
        return buf.begin();
    }

    //-----------------------------------------------
    bool FileUtils::DirSetCurrent( const tchar* pszDir )
    {
    	return 0 == chdir( pszDir );
    }

    //-----------------------------------------------
    bool FileUtils::Move( const tchar* srcPath, const tchar* newPath )
    {
        if (!StringUtils::IsEmpty(srcPath) && !StringUtils::IsEmpty(newPath))
        {
            if (FileIsExist(newPath))
                return false;

            return 0 == rename( srcPath, newPath );
        }
        return false;
    }

    //-----------------------------------------------
    void FileUtils::DirExplorer( const tchar* pszDir, OUT Array<String>* pArrFiles, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern /*= _T("*") */ )
    {
        if ( NULL == pszDir || (NULL == pArrDirs && NULL == pArrFiles) )   return;

        String dirpath = pszDir;
        PathFormat(&dirpath);

        DIR* dirp = NULL;
        struct stat statbuf;
        struct dirent* dp = NULL;

        if ( -1 == stat(dirpath.c_str(), &statbuf) )
        	return;

        _Wildcard* pWildcard = NULL;
        if (!(
        		(!pszSearchPattern || 1 == StringUtils::GetLength(pszSearchPattern))
        		&& (_T('*') == pszSearchPattern[0])
        	)
        	)
        {
        	pWildcard = new _Wildcard(pszSearchPattern);
        }

        String dirpath2;
        dirp = opendir( dirpath.c_str() );
        while ( NULL != (dp=readdir(dirp)) ) {

        	if ( (0 == dp->d_name[1] && '.' == dp->d_name[0])
        		|| (0 == dp->d_name[2] && '.' == dp->d_name[0] && '.' == dp->d_name[1])
        		)
        	{
        		continue;
        	}

        	dirpath2 = dirpath + dp->d_name;
        	if ( -1 == stat(dirpath2.c_str(), &statbuf) )
        	{
        		continue;
        	}
        	if (pWildcard && !pWildcard->Check(dp->d_name))
        	{
        		continue;
        	}

        	if (S_ISDIR(statbuf.st_mode))
        	{
        		if (pArrDirs)	pArrDirs->push_back(dirpath2);
        	}
        	else
            {
        		if (pArrFiles)	pArrFiles->push_back(dirpath2);
            }

        } // while

        closedir(dirp);
        SAFE_DELETE(pWildcard);
    }

    //-----------------------------------------------
    s64 FileUtils::GetWriteTime( const tchar* pszPath )
    {
    	struct stat statbuf;
        if (-1 != stat(pszPath, &statbuf))
        {
        	return statbuf.st_mtime;
        }

        return -1;
    }

    //-----------------------------------------------
    s64 FileUtils::GetAccessTime( const tchar* pszPath )
    {
    	struct stat statbuf;
		if (-1 != stat(pszPath, &statbuf))
		{
			return statbuf.st_atime;
		}

		return -1;
    }

    //-----------------------------------------------
    s64 FileUtils::GetCreateTime( const tchar* pszPath )
    {
    	struct stat statbuf;
    	if (-1 != stat(pszPath, &statbuf))
    	{
    		return statbuf.st_ctime;
    	}

    	return -1;
    }

    //-----------------------------------------------
    FileAttribute FileUtils::GetFileAttribute( const tchar* pszPath )
    {
        FileAttribute attri = FileAttribute_Error;

        struct stat statbuf;
        if (-1 != stat(pszPath, &statbuf))
        {
        	if (S_ISREG(statbuf.st_mode))	attri = FileAttribute_Arch;
           	else if (S_ISDIR(statbuf.st_mode))	attri = FileAttribute_SubDir;
            else
            {
                attri = FileAttribute_System;
            }

            u32 power = statbuf.st_mode & 0x777;
#if CN_PLATFORM != CN_PLATFORM_ANDROID
            if (!(S_IWRITE & power))
            {
                attri = FileAttribute_ReadOnly;
            }
#endif

//            	else if (S_ISCHR(statbuf.st_mode) || S_ISBLK(statbuf.st_mode))	attri = FileAttribute_System;
//            	else if (S_ISFIFO(statbuf.st_mode))	attri = FileAttribute_FIFO;
//            	else
//            		attri = FileAttribute_System;
        }

        return attri;
    }

    //-----------------------------------------------
    const String& FileUtils::DirGetRuningDir()
    {
    	static String str;
        static char path[255] = {0};
        if (str.size() > 0)	return str;

        ZeroMemory(path, sizeof(path));
        getcwd(path, 255);
        str = path;
        return str;
    }

    //-----------------------------------------------
    bool FileUtils::FileCopy( const tchar* pszSrcFile, const tchar* pszDestFile )
    {
        if ( NULL == pszSrcFile || NULL == pszDestFile )   return NULL;
        
        FileStream fs;
        if ( fs.open( pszSrcFile, FileStream::FM_Binary|FileStream::FM_Read ) )
        {
            if (FileUtils::FileIsExist(pszDestFile))
            {
                if (!FileUtils::FileRemove(pszDestFile))
                    return false;
            }

            FileStream fsDest;
            if (!fsDest.open( pszDestFile, FileStream::FM_Binary|FileStream::FM_New|FileStream::FM_ReadWrite ))
                return false;

            if ( fs.getByteSize() != fs.readStream( fsDest, fs.getByteSize() ) )
                return false;

            return true;
        }

        return false;
    }


} // end namespace hyc

#endif // end of define __BP_FILEUTILS_HPP_20110121145907__
