//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#include "../../include/utils/fileUtils.h"
#include "../../include/utils/stringUtils.h"
#include <algorithm>

//-----------------------------------------------
//	windows
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   include "../_win32/fileUtils.hpp"

//-----------------------------------------------
//  Linux
#elif CN_PLATFORM == CN_PLATFORM_LINUX || CN_PLATFORM == CN_PLATFORM_ANDROID
#   include "../_linux/fileUtils.hpp"

//-----------------------------------------------
#else
#   pragma error("unimplement")
#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS


//----------------------------------------------------------------------------------------------------
//! 鍚勫钩鍙扮殑鍏叡瀹炵幇
//----------------------------------------------------------------------------------------------------

namespace hyc {


    //-----------------------------------------------
    String FileUtils::PathBundle( const tchar* pszPath )
    {
        uinteger len = StringUtils::GetLength(pszPath);
        uinteger len2 = FileUtils::DirGetRuningDir().size();
        if (len < len2)
        {
            return FileUtils::DirGetRuningDir() + pszPath;
        }

        const tchar* pszDir = FileUtils::DirGetRuningDir().c_str();

        
        uinteger i = 0;
        if (len > 8)
        {
            if ((pszPath[0] == _T('f') || pszPath[0] == _T('F'))
                && (pszPath[1] == _T('i') || pszPath[1] == _T('I'))
                && (pszPath[2] == _T('l') || pszPath[2] == _T('L'))
                && (pszPath[3] == _T('e') || pszPath[3] == _T('E'))
                && pszPath[4] == _T(':')
                && pszPath[5] == _T('/')
                && pszPath[6] == _T('/')
                && pszPath[7] == _T('/')
                )
            {
                len -= 8;
                i = 8;
                if (pszDir[0] == _T('/'))
                {
                    len++;
                    i = 7;
                }
            }
        }
        
        len = Math::Min(len, len2);
        
        for (uinteger j = 0; j < len; j++,i++) {
            if (pszPath[i] != pszDir[j]) {
                return FileUtils::DirGetRuningDir() + pszPath;
            }
        }

        return pszPath;
    }

    //-----------------------------------------------
    bool FileUtils::DirCreateRecursive( const tchar* pszDir )
    {
        if (StringUtils::IsEmpty(pszDir))
            return false;
        
        String str;
        String temp = pszDir;
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        temp.replace( 0, temp.size(), _T('/'), _T('\\') );
        if ( temp[temp.size()-1] != _T('\\') )
        {
            temp += _T('\\');
        }
#else
        temp.replace( 0, temp.size(), _T('\\'), _T('/') );
        if ( temp[temp.size()-1] != _T('/') )
        {
            temp += _T('/');
        }
#endif
        
        u32 ll, ll2 = 1;
        while (true)
        {
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
            ll = temp.find(_T('\\'), ll2);
#else
            ll = temp.find(_T('/'), ll2);
#endif
            
            if ( ll != String::npos )
            {
                ll2 = ll+1;
                str = temp.substr(0,ll);
                if ( !DirIsExist(str.c_str()) )
                {
                    if (!DirCreate(str.c_str()))	return false;
                }
            }
            else
                break;
        }
        
        return true;
    }
    
    //-----------------------------------------------
    bool FileUtils::FileCreateRecursive( const tchar* pszFile )
    {
        if (StringUtils::IsEmpty(pszFile))
            return false;

        // 瀛樺湪杩斿洖.
        if (FileUtils::FileIsExist(pszFile))
            return true;

        // 鏌ユ壘鐩綍, 淇濊瘉鐩綍瀛樺湪.
        String path, filename;
        FileUtils::PathSplitName( pszFile, path, filename );
        DirAssure( path.c_str() );

        return FileCreate(pszFile);	    
    }

    //-----------------------------------------------
    bool FileUtils::FileAssure( const tchar* pszFile )
    {
        if (!StringUtils::IsEmpty(pszFile))
        {
            if (!FileUtils::FileIsExist(pszFile))
            {
                FileUtils::FileCreateRecursive(pszFile);
                return false;
            }
            return true;
        }
        return false;
    }
    //-----------------------------------------------
    bool FileUtils::DirAssure( const tchar* pszDir )
    {
        if (!StringUtils::IsEmpty(pszDir))
        {
            if (!FileUtils::DirIsExist(pszDir))
            {
                FileUtils::DirCreateRecursive(pszDir);
                return false;
            }
            return true;
        }
        return false;
    }
    
    //-----------------------------------------------
    void FileUtils::DirExplorerFilesRecursive( const tchar* pszDir, OUT Array<String>* pArrFiles, const tchar* pszSearchPattern /*= _T("*") */ )
    {
        if ( NULL == pszDir || NULL == pArrFiles )   return;
        
        // 鎼滅储.
        Array<String>   arrSearchDir;
        arrSearchDir.push_back( pszDir );
        
        for ( u32 i = 0; i < arrSearchDir.size(); ++i )
        {
            DirExplorer( arrSearchDir[i].c_str(), pArrFiles, &arrSearchDir, pszSearchPattern );
        }
    }
    
    //-----------------------------------------------
    void FileUtils::DirExplorerDirsRecursive( const tchar* pszDir, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern /*= _T("*") */ )
    {
        if ( NULL == pszDir || NULL == pArrDirs )   return;
        
        u32 uCurPos = pArrDirs->size();  // 淇濆瓨褰撳墠澶у皬.
        
        DirExplorer( pszDir, NULL, pArrDirs, pszSearchPattern );
        
        for ( u32 i = uCurPos; i < pArrDirs->size(); ++i )
        {
            DirExplorer( (*pArrDirs)[i].c_str(), NULL, pArrDirs, pszSearchPattern );
        }
    }
    
    //-----------------------------------------------
    void FileUtils::PathFormat( OUT String* pszPath )
    {
        if ( NULL == pszPath || 0 == pszPath->size() )    return;

        pszPath->replace( 0, pszPath->size()-1, _T('\\'), _T('/') );
        if ( _T('/') != pszPath->c_str()[pszPath->size()-1] )   (*pszPath) += _T('/');
    }

    //-----------------------------------------------
    void FileUtils::PathFormat_File( OUT String* pszPath )
    {
        if ( NULL == pszPath )    return;

        pszPath->replace( 0, pszPath->size()-1, _T('\\'), _T('/') );
    }

    //-----------------------------------------------
    bool FileUtils::PathIsRelative( const tchar* pszPath )
    {
        if ( StringUtils::IsEmpty(pszPath) )    return true;
        if ( StringUtils::GetLength(pszPath) < 2 )  return true;
        if (_T(':') == pszPath[1])  return false;

        return true;
    }

    //-----------------------------------------------
    void FileUtils::PathSplitName( const tchar* fullName, OUT String& path, OUT String& baseName, OUT String& extentionName )
    {
        if (StringUtils::IsEmpty(fullName))
        {
            path.clear();
            baseName.clear();
            extentionName.clear();
            return;
        }

        String name = fullName;

        path.clear();
        baseName.clear();
        extentionName.clear();

        // Replace \ with / first
        name.replace(0, name.size(), _T('\\'), _T('/'));
        // split based on final /
        u32 i = name.find_last_of(_T('/'));

        if (String::npos == i) // can't find
        {
            path.clear();

            // file name not contain path
            i = name.find(_T('.'));
            if (String::npos == i) // no ext
            {
                baseName = name;
                extentionName.clear();
            }
            else
            {
                baseName = name.substr(0, i);   // no contain '.'
                extentionName = name.substr(i+1, name.size()-i-1);
            } // if..else
        }
        else
        {
            path = name.substr(0, i+1);

            // file name
            u32 j = name.find_last_of(_T('.'));
            if (j > i && j != String::npos) // exist
            {
                baseName = name.substr(i+1, j-i-1);
                extentionName = name.substr(j+1, name.size()-j-1);
            }
            else
            {
                baseName = name.substr(i+1, name.size()-i-1);
                extentionName.clear();
            } // if..else
        } // if..else
    }

    //-----------------------------------------------
    void FileUtils::PathSplitName( const tchar* fullName, OUT String& path, OUT String& fileName )
    {

        if (StringUtils::IsEmpty(fullName))
        {
            path.clear();
            fileName.clear();

            return;
        }
        String name = fullName;
        path.clear();
        fileName.clear();

        // Replace \ with / first
        name.replace( 0, name.size(), _T('\\'), _T('/') );
        // split based on final /
        u32 i = name.find_last_of(_T('/'));

        if (i == String::npos) // can't find
        {
            fileName = name;
            path.clear();
        }
        else
        {
            path = name.substr(0, i+1);
            fileName = name.substr(i+1, name.size()-i-1);
        } // if..else
    }

    //-----------------------------------------------
    void FileUtils::DirExplorerDirsRecursiveEmpty( const tchar* pszDir, OUT Array<String>* pArrDirs, const tchar* pszSearchPattern /*= _T("*") */ )
    {
        if (!pArrDirs)  return;
        Array<String>   arrTempFiles;

        Array<String>   arrDirs;
        DirExplorerDirsRecursive( pszDir, &arrDirs, pszSearchPattern );

        for (u32 i = 0; i < arrDirs.size(); ++i)
        {
            arrTempFiles.clear();
            DirExplorer( (pszDir+arrDirs[i]).c_str(), &arrTempFiles, &arrTempFiles );
            if (arrTempFiles.size() <= 0)
                pArrDirs->push_back(arrDirs[i]);
        }
    }
    
} // end of namespace hyc

