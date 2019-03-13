//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// common libaray
// author:       lipengxiang
// description:  FileUtilsµƒiphone µœ÷
//

#include "../preheader.h"
#include "../../include/utils/fileUtils.h"
//#include "../../include/container/map.h"
#include "../../include/_platform/iOS.h"

//#include <sys/dir.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


namespace hyc
{
#define FileMgr [NSFileManager defaultManager]

    inline NSString* GetPath( const tchar* pszPath )
    {
        //String path = FileUtils::PathBundle(pszPath);
        return PlatformIOS::ConvertToNSString(pszPath);
    }

    //-----------------------------------------------
    bool FileUtils::FileIsExist( const tchar* pszFile )
    {
        if ( NULL == pszFile )  return false;
        
        BOOL isDir = false;
        BOOL r = [FileMgr fileExistsAtPath:PlatformIOS::ConvertToNSString(pszFile) isDirectory: &isDir];
        return r && !isDir;
    }
    
    //-----------------------------------------------
    bool FileUtils::FileCreate( const tchar* pszFile )
    {
        if ( NULL == pszFile )  return false;
        if ( FileIsExist(pszFile) ) return false;
        
        // create file
        return [FileMgr createFileAtPath:GetPath(pszFile) contents:nil attributes:nil];
    }
    
    //-----------------------------------------------
    bool FileUtils::FileRemove( const tchar* pszFile )
    {
        if (NULL == pszFile) {
            return false;
        }
        
        if (!FileIsExist(pszFile))    return false;
        
        return YES == [FileMgr removeItemAtPath: GetPath(pszFile) error:NULL];
    }
    
    //-----------------------------------------------
    bool FileUtils::FileSetSize( const tchar* pszFile, s64 newSize )
    {
        if ( !FileIsExist( pszFile ) )  return false;

        // ¥Úø™
#if defined(_UNICODE) || defined(UNICODE)
        cString str;
        StringUtils::Wide2narrow( filePath, StringUtils::GetLength(pszFile), str );
        int hFile = open(str.c_str(), O_RDWR);
#else
        int hFile = open(pszFile, O_RDWR);
#endif // defined(_UNICODE) || defined(UNICODE)
        
        if (-1 == hFile)
        {
            return false;
        }
        
        s32 r = ftruncate(hFile, newSize);        
        return (-1 != r) && (-1 != close(hFile));
    }
    
    //-----------------------------------------------
    s64 FileUtils::FileGetSize( const tchar* pszFile )
    {
        if ( !FileIsExist( pszFile ) )  return -1;
        
        NSDictionary* attr = [FileMgr attributesOfItemAtPath:GetPath(pszFile) error:NULL];
        return (nil==attr) ? (-1) : ( [[attr objectForKey:NSFileSize] intValue] );
    }
    
    //------------------------------------------------
    bool FileUtils::DirIsExist( const tchar* pszDir )
    {
        if (NULL == pszDir) return false;
        
        
        BOOL isDir = false;
        bool r = [FileMgr fileExistsAtPath:GetPath(pszDir) isDirectory: &isDir];
        
        return r && isDir;
    }
    
    //------------------------------------------------
    bool FileUtils::DirCreate( const tchar* pszDir )
    {
        if (NULL == pszDir) return false;
        
        return [FileMgr createDirectoryAtPath:GetPath(pszDir) withIntermediateDirectories:NO attributes:NULL error:NULL];
    }
    
    /*/-----------------------------------------------
    
    bool FileUtils::DirCreateRecursive( const tchar* pszDir )
    {
        if (NULL == pszDir) return false;
        
        NSString* path = PlatformIOS::ConvertToNSString(pszDir);
        return [FileMgr createDirectoryAtPath:path withIntermediateDirectories:YES attributes:NULL error:NULL];
    }*/
    
    //-----------------------------------------------
    bool FileUtils::DirRemoveRecursive( const tchar* pszDir )
    {
        if (NULL == pszDir) return false;
        if (!DirIsExist(pszDir))   return false;
        
        return YES == [FileMgr removeItemAtPath:GetPath(pszDir) error:NULL];
    }
    
    //-----------------------------------------------
    String FileUtils::DirGetCurrent()
    {
        //getcwd
        return PlatformIOS::ConvertToString( [FileMgr currentDirectoryPath] );
    }
    
    //-----------------------------------------------
    bool FileUtils::DirSetCurrent( const tchar* pszDir )
    {
        if (NULL == pszDir) return false;
        // change current dir
        return [FileMgr changeCurrentDirectoryPath:[GetPath(pszDir) stringByExpandingTildeInPath]];
    }
    
    //-----------------------------------------------
    void FileUtils::DirExplorer( const tchar* pszDir, OUT Array<String>* pArrFiles, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern /*= _T("*") */ )
    {
        if ( NULL == pszDir )   return;       
        
        // create file enumerator
        NSString* nspath = GetPath(pszDir);
        NSDirectoryEnumerator* dirEnumerater = [FileMgr enumeratorAtPath:nspath];
        
        // wildcard
        NSString* wildcard = (NULL==pszSearchPattern) ? @"SELF like '*'" : 
                            [[@"SELF like '" stringByAppendingString:PlatformIOS::ConvertToNSString(pszSearchPattern)] stringByAppendingString:@"'"];
        
        // predicate
        NSPredicate* predicate = [NSPredicate predicateWithFormat:wildcard];
        
        String path = PlatformIOS::ConvertToString(nspath);
        PathFormat(&path);
        
        // —≠ª∑ªÒµ√Œƒº˛√˚
        if (nil != dirEnumerater && nil != predicate) {
            String strtemp;
            NSString* filePath;
            
            while ( filePath = [dirEnumerater nextObject] ) {
                
                if ( 1 < [dirEnumerater level] )
                {
                    [dirEnumerater skipDescendents];
                }                
                else if ( [predicate evaluateWithObject:filePath] )
                {
                    strtemp = path + PlatformIOS::ConvertToString(filePath);
                    if ( pArrDirs && DirIsExist( strtemp.c_str() ) )    pArrDirs->push_back( strtemp );
                    else if ( pArrFiles ) pArrFiles->push_back( strtemp );
                }                
            } // while            
        } // if
    }
    
    //-----------------------------------------------
    bool FileUtils::Move( const tchar* srcPath, const tchar* newPath )
    {
        if (NULL == srcPath || NULL == newPath) return false;        
        return YES == [FileMgr moveItemAtPath:GetPath(srcPath) toPath:GetPath(newPath) error:NULL];
    }
    
    //-----------------------------------------------
    s64 FileUtils::GetCreateTime( const tchar* pszPath )
    {
        if (NULL == pszPath) {
            return -1;
        }
        
        NSDictionary* attr = [FileMgr attributesOfItemAtPath:GetPath(pszPath) error:NULL];
        return (nil==attr) ? (-1) : ( [[attr objectForKey:NSFileCreationDate] timeIntervalSince1970] );
    }
    
    //------------------------------------------------
    s64 FileUtils::GetWriteTime( const tchar* pszPath )
    {
        if (NULL == pszPath) {
            return -1;
        }
        
        NSDictionary* attr = [FileMgr attributesOfItemAtPath:GetPath(pszPath) error:NULL];
        return (nil==attr) ? (-1) : ( [[attr objectForKey:NSFileModificationDate] timeIntervalSince1970] );        
    }
    
    //-------------------------------------------------
    s64 FileUtils::GetAccessTime( const tchar* pszPath )
    {
        return -1;          
    }
    
    //-------------------------------------------------
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
            if (!(S_IWRITE & power))
            {
                attri = FileAttribute_ReadOnly;
            }
            
            //            	else if (S_ISCHR(statbuf.st_mode) || S_ISBLK(statbuf.st_mode))	attri = FileAttribute_System;
            //            	else if (S_ISFIFO(statbuf.st_mode))	attri = FileAttribute_FIFO;
            //            	else
            //            		attri = FileAttribute_System;
        }
        
        return attri;
        
    }
    
    //-------------------------------------------------
    const String& FileUtils::DirGetRuningDir()
    {
        return PlatformIOS::GetDirBundleApp();
    }
    
    //-----------------------------------------------
    bool FileUtils::FileCopy( const tchar* pszSrcFile, const tchar* pszDestFile )
    {
        if (NULL == pszSrcFile || NULL == pszDestFile)
            return false;
        return YES == [FileMgr copyItemAtPath:GetPath(pszSrcFile) toPath:GetPath(pszDestFile) error:NULL];
    }
	

} // end namespace hyc
