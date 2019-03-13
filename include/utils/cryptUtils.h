//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  加密工具
//

#ifndef __BP_CRYPTUTILS_H_20101206143207__
#define __BP_CRYPTUTILS_H_20101206143207__

#include "../io/fileStream.h"
#include "./stringUtils.h"
#include "./encodingUtils.h"

namespace hyc
{
    //-----------------------------------------------
    //!  128 bit 消息摘要
    typedef struct Digest_128 {
        bool operator==( const Digest_128& rhs ) const    { for ( u32 i = 0; i < 16; ++i ) { if ( digest[i] != rhs.digest[i] ) return false; } return true; }
        bool operator!=( const Digest_128& rhs ) const    { return !((*this)==rhs); }
        Byte    digest[16]; //! 128 bit
    } Digest_128;

    //-----------------------------------------------
    //!  160 bit 消息摘要
    typedef struct Digest_160 {
        bool operator==( const Digest_160& rhs ) const    { for ( u32 i = 0; i < 20; ++i ) { if ( digest[i] != rhs.digest[i] ) return false; } return true; }
        bool operator!=( const Digest_160& rhs ) const    { return !((*this)==rhs); }
        Byte    digest[20]; //! 160 bit
    } Digest_160;
    
    
    
#ifdef HYC_COMPILE_MODULE_CRYPT_SHA1
    //----------------------------------------------------------------------------------------------------
    //! 用于多次计算sha1.
    //----------------------------------------------------------------------------------------------------
    class HYC_API SHA1Handler : public RefCount {
        friend class CryptUtils;
        PRIVATE_CLASS_DECLARE;
    public:
        SHA1Handler();
        ~SHA1Handler();
    };
    typedef ObjectPtr<SHA1Handler> SHA1HandlerPtr;
#endif

    
    //----------------------------------------------------------------------------------------------------
    //!	CryptUtils
    //----------------------------------------------------------------------------------------------------
    class HYC_API CryptUtils
    {
    public:
        /*---------------------------
        !  SHA1; 160 bit message digest
        ---------------------------*/
#ifdef HYC_COMPILE_MODULE_CRYPT_SHA1
		static bool     SHA1( const Byte* pData, u32 length, OUT Digest_160& digest );
        static bool     SHA1( const String data, OUT String& digest ) { return CryptUtils::SHA1((Byte*)data.c_str(), (u32)StringUtils::GetByteSize(data.c_str()), digest); }
        static bool     SHA1( const Byte* pData, u32 length, OUT String& digest ) {
            Digest_160 digestInfo;
            if (!CryptUtils::SHA1(pData, length, digestInfo))
                return false;
            EncodingUtils::ByteToHex(digestInfo.digest, 20, digest);
            return true;
        }
        
        /**
         * @desc 使用多次update的方式. finish之后将不可再次使用.
         */
        static SHA1HandlerPtr   SHA1_begin();
        static void             SHA1_update( SHA1HandlerPtr pHandler, const Byte* pData, u32 length );
        static bool             SHA1_finish( SHA1HandlerPtr pHandler, OUT Digest_160& digest );
#endif

        /*---------------------------
        !  md5; 128 bit message digest
        ---------------------------*/
#ifdef HYC_COMPILE_MODULE_CRYPT_MD5
        static bool     Md5( const Byte* pData, u32 length, OUT Digest_128& digest );
        static bool     Md5( const String data, OUT String& digest ) { return CryptUtils::Md5((Byte*)data.c_str(), (u32)StringUtils::GetByteSize(data.c_str()), digest); }
        static bool     Md5( const Byte* pData, u32 length, OUT String& digest ) {
            Digest_128 digestInfo;
            if (!CryptUtils::Md5(pData, length, digestInfo))
                return false;
            EncodingUtils::ByteToHex(digestInfo.digest, 16, digest);
            return true;
        }
#endif

        /*---------------------------
        !  crc32; 32 bit message digest
        ---------------------------*/
        static u32 CRC32( const Byte* pData, u32 length );
        static u32 CRC32( const Byte* pData, u32 length, u32 preCrc32 );    // 传入上一个crc32值, 传入0则等同于第一次计算.
        static u32 CRC32_segments( const Array<const Byte*>* pArrData, const Array<u32>* pArrLength );   // 对多段数据合并计算.
        static u32 CRC32_file( const tchar* pszFilePath );  //! 创建小段内存对大文件反复使用.
        static u32 CRC32_fileStream( FileStream& fileStream, u32 pos, u32 count );  //! 创建小段内存读取文件流.

        /*---------------------------
        !  RC4; 原生态的RC4算法
        !  对称加密;
        !  \param pData:  要加密的数据
        !  \param pKey:   密钥;最长支持256个字节长度
        ---------------------------*/
#ifdef HYC_COMPILE_MODULE_CRYPT_RC4
        static void     RC4_Crypt( OUT Byte* pData, u32 cLenData, const Byte* pKey, u32 cLenKey );
        static void     RC4_Decrypt( OUT Byte* pData, u32 cLenData, const Byte* pKey, u32 cLenKey );
#endif
    };
	

} // end namespace hyc

#endif // end of define __BP_CRYPTUTILS_H_20101206143207__
