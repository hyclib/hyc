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
    /// 128位消息摘要
    /// </summary>
    public ref class Digest_128
    {
    public:
        array<System::Byte>^ digest;
        Digest_128()
        {
            digest = gcnew array<System::Byte>(16);
        }

        // 比较
        virtual bool Equals(Object^ obj) override
        {
            if (obj == nullptr || GetType() != obj->GetType())
            {
                if (obj->GetType() == digest->GetType())
                {
                    array<System::Byte>^ p = static_cast<array<System::Byte>^>(obj);
                    for (int i = 0; i < digest->Length; ++i)
                    {
                        if (digest[i] != p[i])  return false;
                    }
                    return true;
                }
                return false;
            }
            Digest_128^ p = static_cast<Digest_128^>(obj);
            if (nullptr == digest && nullptr != p->digest
                || nullptr != digest && nullptr == p->digest)
                return false;
            for (int i = 0; i < digest->Length; ++i)
            {
                if (digest[i] != p->digest[i])  return false;
            }
            return true;
        }
        virtual int GetHashCode() override
        {
            return Object::GetHashCode();
        }
        static bool operator==(Digest_128 p1, Digest_128 p2)
        {
            if (nullptr == p1.digest && nullptr != p2.digest
                || nullptr != p1.digest && nullptr == p2.digest)
                return false;
            for (int i = 0; i < p1.digest->Length; ++i)
            {
                if (p1.digest[i] != p2.digest[i])  return false;
            }
            return true;
        }
        UINT    GetBitLength()
        {
            return 128;
        }
        UINT    GetByteLength()
        {
            return 128/8;
        }
        System::Byte operator[](UINT indexIn16)
        {
            return digest[indexIn16];
        }
        void     SetByte(UINT indexIn16, System::Byte v)
        {
            digest[indexIn16] = v;
        }
        UINT8    GetBit(UINT indexIn128)
        {
            UINT i = indexIn128/8;
            UINT j = indexIn128%8;
            System::Byte b = digest[i];
            return hyc::ByteUtils::ReadBit(b, j);
        }
        void     SetBit(UINT indexIn128, UINT8 v)
        {
            UINT i = indexIn128/8;
            UINT j = indexIn128%8;
            hyc::u8 vv = digest[i];
            hyc::ByteUtils::WriteBit(vv, j, v);
            digest[i] = vv;
        }
        virtual System::String^ ToString() override
        {
            if (nullptr == digest)
                return _T("");
            GCHandle^ hObject = GCHandle::Alloc(digest, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();
            hyc::String str;
            hyc::EncodingUtils::ByteToHex( (hyc::Byte*)pObject.ToPointer(), digest->Length, str );
            if (hObject->IsAllocated)
                hObject->Free();
            return Marshal::PtrToStringAnsi( IntPtr((void*)str.c_str()) );
        }
    };

    /// <summary>
    /// 160位消息摘要
    /// </summary>
    public ref class Digest_160
    {
    public:
        array<System::Byte>^ digest;
        Digest_160()
        {
            digest = gcnew array<System::Byte>(20);
        }

        // 比较
        virtual bool Equals(Object^ obj) override
        {
            if (obj == nullptr || GetType() != obj->GetType())
            {
                if (obj->GetType() == digest->GetType())
                {
                    array<System::Byte>^ p = static_cast<array<System::Byte>^>(obj);
                    for (int i = 0; i < digest->Length; ++i)
                    {
                        if (digest[i] != p[i])  return false;
                    }
                    return true;
                }
                return false;
            }
            Digest_160^ p = static_cast<Digest_160^>(obj);
            if (nullptr == digest && nullptr != p->digest
                || nullptr != digest && nullptr == p->digest)
                return false;
            for (int i = 0; i < digest->Length; ++i)
            {
                if (digest[i] != p->digest[i])  return false;
            }
            return true;
        }
        virtual int GetHashCode() override
        {
            return Object::GetHashCode();
        }
        static bool operator==(Digest_160 p1, Digest_160 p2)
        {
            if (nullptr == p1.digest && nullptr != p2.digest
                || nullptr != p1.digest && nullptr == p2.digest)
                return false;
            for (int i = 0; i < p1.digest->Length; ++i)
            {
                if (p1.digest[i] != p2.digest[i])  return false;
            }
            return true;
        }
        UINT    GetBitLength()
        {
            return 160;
        }
        UINT    GetByteLength()
        {
            return 160/8;
        }
        System::Byte operator[](UINT indexIn20)
        {
            return digest[indexIn20];
        }
        void     SetByte(UINT indexIn20, System::Byte v)
        {
            digest[indexIn20] = v;
        }
        UINT8    GetBit(UINT indexIn160)
        {
            UINT i = indexIn160/8;
            UINT j = indexIn160%8;
            System::Byte b = digest[i];
            return hyc::ByteUtils::ReadBit(b, j);
        }
        void     SetBit(UINT indexIn160, UINT8 v)
        {
            UINT i = indexIn160/8;
            UINT j = indexIn160%8;
            hyc::u8 vv = digest[i];
            hyc::ByteUtils::WriteBit(vv, j, v);
            digest[i] = vv;
        }
        virtual System::String^ ToString() override
        {
            if (nullptr == digest)
                return _T("");
            GCHandle^ hObject = GCHandle::Alloc(digest, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();
            hyc::String str;
            hyc::EncodingUtils::ByteToHex( (hyc::Byte*)pObject.ToPointer(), digest->Length, str );
            if (hObject->IsAllocated)
                hObject->Free();
            return Marshal::PtrToStringAnsi( IntPtr((void*)str.c_str()) );
        }
    };


    /// <summary>
    /// 加密工具
    /// </summary>
    public ref class CryptUtils
    {
    public:

        /*---------------------------
        SHA1; 160 bit message digest
        ---------------------------*/
        static bool     SHA1( array<System::Byte>^% data, [Out] Digest_160^% digest )
        {
            if (nullptr == data)
            {
                return false;
            }
            hyc::Digest_160 _digest;
            if (nullptr == digest)
            {
                digest = gcnew Digest_160();
            }

            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            bool r = hyc::CryptUtils::SHA1( (const hyc::Byte*)pObject.ToPointer(), data->Length, _digest );
            if (r)
            {
                for (hyc::u32 i = 0; i < sizeof _digest.digest; ++i)
                {
                    digest->SetByte(i, _digest.digest[i]);
                }
            }
            else
            {
                for (hyc::u32 i = 0; i < sizeof _digest.digest; ++i)
                {
                    digest->SetByte(i, (System::Byte)0);
                }
            }

            if (hObject->IsAllocated)
                hObject->Free();

            return r;
        }

        /*---------------------------
        md5; 128 bit message digest
        ---------------------------*/
        static bool     Md5( array<System::Byte>^% data, [Out] Digest_128^% digest )
        {
            if (nullptr == data)
            {
                return false;
            }
            hyc::Digest_128 _digest;
            if (nullptr == digest)
            {
                digest = gcnew Digest_128();
            }

            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            bool r = hyc::CryptUtils::Md5( (const hyc::Byte*)pObject.ToPointer(), data->Length, _digest );
            if (r)
            {
                for (hyc::u32 i = 0; i < sizeof _digest.digest; ++i)
                {
                    digest->SetByte(i, _digest.digest[i]);
                }
            }
            else
            {
                for (hyc::u32 i = 0; i < sizeof _digest.digest; ++i)
                {
                    digest->SetByte(i, (System::Byte)0);
                }
            }

            if (hObject->IsAllocated)
                hObject->Free();

            return r;

        }


        /// <summary>
        /// 计算crc32值
        /// </summary>
        /// <returns></returns>
        static hyc::u32  CRC32( array<System::Byte>^% data )
        {
            if (nullptr == data)
            {
                return 0;
            }

            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            hyc::u32 r = hyc::CryptUtils::CRC32( (const hyc::Byte*)pObject.ToPointer(), data->Length );

            if (hObject->IsAllocated)
                hObject->Free();

            return r;
        }

        static hyc::u32 CRC32_file( System::String^ filePath )
        {
            return hyc::CryptUtils::CRC32_file( (char*)Marshal::StringToHGlobalAnsi(filePath).ToPointer() );
        }

        /*---------------------------
        RC4; 原生态的RC4算法
        对称加密;
        pData:  要加密的数据
        pKey:   密钥;最长支持256个字节长度
        ---------------------------*/
        static void     RC4_Crypt( [Out] array<System::Byte>^% data, int size, System::String^ key )
        {
            if (data->Length < size)
            {
                throw gcnew Exception("越界");
                return;
            }
            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            hyc::tchar* pStr = (hyc::tchar*)Marshal::StringToHGlobalAnsi(key).ToPointer();
            hyc::CryptUtils::RC4_Crypt((hyc::Byte*)pObject.ToPointer(), size, 
                (hyc::Byte*)pStr, hyc::StringUtils::GetByteSize(pStr));

            if (hObject->IsAllocated)
                hObject->Free();
        }
        static void     RC4_Decrypt( [Out] array<System::Byte>^% data, int size, System::String^ key )
        {
            if (data->Length < size)
            {
                throw gcnew Exception("越界");
                return;
            }
            GCHandle^ hObject = GCHandle::Alloc(data, GCHandleType::Pinned);
            IntPtr pObject = hObject->AddrOfPinnedObject();

            hyc::tchar* pStr = (hyc::tchar*)Marshal::StringToHGlobalAnsi(key).ToPointer();
            hyc::CryptUtils::RC4_Decrypt((hyc::Byte*)pObject.ToPointer(), size, 
                (hyc::Byte*)pStr, hyc::StringUtils::GetByteSize(pStr));

            if (hObject->IsAllocated)
                hObject->Free();
        }

    };

} // end namespace of Utils
} // end namespace of Hy
