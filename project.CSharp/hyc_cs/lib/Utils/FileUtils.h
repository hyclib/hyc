//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// business logic layer network proxy
// author:       lipengxiang
// description:  YunStream.h
//

#pragma once

#include "hyc/include/hyc.h"
#include "../_Common/String.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace Hy
{
namespace Utils
{

    /// <summary>
    /// 文件工具
    /// </summary>
    public ref class FileUtils
    {
    public:

        static bool Move( System::String^ srcPath, System::String^ newPath )
        {
            return hyc::FileUtils::Move( (char*)Marshal::StringToHGlobalAnsi(srcPath).ToPointer(),
                (char*)Marshal::StringToHGlobalAnsi(newPath).ToPointer());
        }

        /// <summary>
        /// 适配各平台路径分隔符不同
        /// </summary>
        /// <returns></returns>
        static void PathFormat( [Out] System::String^% pszPath )
        {
            Hy::String^ str = gcnew Hy::String();
            (*str) = pszPath;
            hyc::FileUtils::PathFormat( (hyc::String*)str->getPtr().ToPointer() );
            pszPath = str->ToString();
            delete str;
        }

        /*---------------------------
        分割路径为: 路径 / 文件基本名 / 扩展名
        ---------------------------*/
        static void PathSplitName( System::String^% fullName, 
            [Out] System::String^% path, 
            [Out] System::String^% baseName,
            [Out] System::String^% extentionName )
        {
            hyc::String cpath, cbaseName, cextentionName;
            hyc::FileUtils::PathSplitName( 
                (hyc::tchar*)Marshal::StringToCoTaskMemAnsi(fullName).ToPointer(),
                cpath, cbaseName, cextentionName);
            path = Marshal::PtrToStringAnsi(IntPtr((void*)cpath.c_str()));
            baseName = Marshal::PtrToStringAnsi(IntPtr((void*)cbaseName.c_str()));
            extentionName = Marshal::PtrToStringAnsi(IntPtr((void*)cextentionName.c_str()));
        }

        static void PathSplitName( System::String^% fullName, 
            [Out] System::String^% path, 
            [Out] System::String^% fileName )
        {
            hyc::String cpath, cfileName;
            hyc::FileUtils::PathSplitName( 
                (hyc::tchar*)Marshal::StringToCoTaskMemAnsi(fullName).ToPointer(),
                cpath, cfileName);
            path = Marshal::PtrToStringAnsi(IntPtr((void*)cpath.c_str()));
            fileName = Marshal::PtrToStringAnsi(IntPtr((void*)cfileName.c_str()));
        }

    };

} // end namespace of Utils
} // end namespace of Hy
