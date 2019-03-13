//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// business logic layer network proxy
// author:       lipengxiang
// description:  YunStream.h
//

#pragma once

#include "hyc/include/hyc.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace Hy
{
namespace Utils
{

    /// <summary>
    /// convert between utf-8, utf-16 and utf-32
    /// </summary>
    public ref class EncodingUtils
    {
    public:

        /*---------------------------
            convert ANSI to UTF8
        ---------------------------*/
        static void  ANSItoUTF8( array<System::Byte>^% data, [Out] array<System::Byte>^% pOutData )
        {
            pOutData = nullptr;
            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            hyc::FixArray<hyc::utf8>    buf(data->Length*3+1);
            hyc::u32 c = hyc::EncodingUtils::StrToUTF8( (char*)pObject.ToPointer(), data->Length, buf.begin(), buf.size() );

            if (hObject->IsAllocated)
                hObject->Free();

            // 拷贝
            pOutData = gcnew array<System::Byte>(c);
            hObject = GCHandle::Alloc(pOutData, GCHandleType::Pinned);
            pObject = hObject->AddrOfPinnedObject();
            memcpy( (hyc::Byte*)pObject.ToPointer(), buf.begin(), buf.size() );

            if (hObject->IsAllocated)
                hObject->Free();
        }

        /*---------------------------
            convert byte to hex
        ---------------------------*/
        static void ByteToHex( array<System::Byte>^% data, [Out] System::String^% str )
        {
            str = _T("");
            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            hyc::String cstr;
            hyc::EncodingUtils::ByteToHex( (hyc::Byte*)pObject.ToPointer(), data->Length, cstr );
            str = Marshal::PtrToStringAnsi( IntPtr((void*)cstr.c_str()) );

            if (hObject->IsAllocated)
                hObject->Free();
        }

        static void HexToByte( System::String^% str, [Out] array<System::Byte>^% data )
        {
            hyc::String cstr = (char*)Marshal::StringToHGlobalAnsi(str).ToPointer();
            hyc::Byte* pData = (hyc::Byte*)hyc::MemoryAlloc::Malloc(cstr.size()/2);
            hyc::EncodingUtils::HexToByte( cstr.c_str(), cstr.size(), pData, cstr.size()/2 );

            System::Array::Resize(data, cstr.size()/2);
            for (int i = 0; i < data->Length; ++i)
            {
                data[i] = pData[i];
            }
            hyc::MemoryAlloc::Free(pData);
        }

        /*---------------------------
            base64; 
            base64编码用于将二进制流转换成字符流用于http的传输
        ---------------------------*/
        static bool IsBase64( char c )
        {
            return hyc::EncodingUtils::IsBase64(c);
        }
        static void Base64_Encode( array<System::Byte>^% data, [Out] System::String^% strBase64 )
        {
            strBase64 = _T("");
            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            hyc::cString cstr;
            hyc::EncodingUtils::Base64_Encode( (hyc::Byte*)pObject.ToPointer(), data->Length, cstr );
            strBase64 = Marshal::PtrToStringAnsi( IntPtr((void*)cstr.c_str()) );

            if (hObject->IsAllocated)
                hObject->Free();
        }
        static void Base64_Decode( System::String^% strBase64, [Out] array<System::Byte>^% data )
        {
            hyc::ByteStream bs(CN_IS_BIG_ENDIAN);
            char* cstr = (char*)Marshal::StringToHGlobalAnsi(strBase64).ToPointer();
            hyc::EncodingUtils::Base64_Decode( 
                cstr, hyc::StringUtils::GetByteSize(cstr), bs );
            System::Array::Resize(data, bs.size());
            for (int i = 0; i < data->Length; ++i)
            {
                data[i] = bs[i];
            }
        }

        /*---------------------------
            url encode
        ---------------------------*/
        static void Url_Encode( System::String^% stream, [Out] System::String^% strUrlEncode )
        {
            hyc::cString str;
            char* cstr = (char*)Marshal::StringToHGlobalAnsi(stream).ToPointer();
            hyc::EncodingUtils::Url_Encode( 
                cstr, hyc::StringUtils::GetByteSize(cstr), 
                str );

            strUrlEncode = Marshal::PtrToStringAnsi(IntPtr((void*)str.c_str()));
        }
        static void Url_Decode( System::String^% strUrlEncode, [Out] System::String^% stream )
        {
            hyc::cString str;
            char* cstr = (char*)Marshal::StringToHGlobalAnsi(strUrlEncode).ToPointer();
            hyc::EncodingUtils::Url_Decode( 
                cstr, hyc::StringUtils::GetByteSize(cstr), 
                str );

            stream = Marshal::PtrToStringAnsi(IntPtr((void*)str.c_str()));
        }
    };

} // end namespace of Utils
} // end namespace of Hy
