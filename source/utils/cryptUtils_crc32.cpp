//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:
//

#include "../preheader.h"

#include "../../include/utils/cryptUtils.h"
#include "../../include/utils/stringUtils.h"
#include "../../include/utils/fileUtils.h"
#include "../../include/io/fileStream.h"
#include "../../include/container/fixArray.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   pragma warning(disable: 4018)  // signed/unsigned mismatch
#endif

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	CRC32
    //----------------------------------------------------------------------------------------------------
    
    static const s32 CRC32_table[]={
        static_cast<s32>(0x00000000L), static_cast<s32>(0x77073096L), static_cast<s32>(0xee0e612cL), static_cast<s32>(0x990951baL), static_cast<s32>(0x076dc419L),
        static_cast<s32>(0x706af48fL), static_cast<s32>(0xe963a535L), static_cast<s32>(0x9e6495a3L), static_cast<s32>(0x0edb8832L), static_cast<s32>(0x79dcb8a4L),
        static_cast<s32>(0xe0d5e91eL), static_cast<s32>(0x97d2d988L), static_cast<s32>(0x09b64c2bL), static_cast<s32>(0x7eb17cbdL), static_cast<s32>(0xe7b82d07L),
        static_cast<s32>(0x90bf1d91L), static_cast<s32>(0x1db71064L), static_cast<s32>(0x6ab020f2L), static_cast<s32>(0xf3b97148L), static_cast<s32>(0x84be41deL),
        static_cast<s32>(0x1adad47dL), static_cast<s32>(0x6ddde4ebL), static_cast<s32>(0xf4d4b551L), static_cast<s32>(0x83d385c7L), static_cast<s32>(0x136c9856L),
        static_cast<s32>(0x646ba8c0L), static_cast<s32>(0xfd62f97aL), static_cast<s32>(0x8a65c9ecL), static_cast<s32>(0x14015c4fL), static_cast<s32>(0x63066cd9L),
        static_cast<s32>(0xfa0f3d63L), static_cast<s32>(0x8d080df5L), static_cast<s32>(0x3b6e20c8L), static_cast<s32>(0x4c69105eL), static_cast<s32>(0xd56041e4L),
        static_cast<s32>(0xa2677172L), static_cast<s32>(0x3c03e4d1L), static_cast<s32>(0x4b04d447L), static_cast<s32>(0xd20d85fdL), static_cast<s32>(0xa50ab56bL),
        static_cast<s32>(0x35b5a8faL), static_cast<s32>(0x42b2986cL), static_cast<s32>(0xdbbbc9d6L), static_cast<s32>(0xacbcf940L), static_cast<s32>(0x32d86ce3L),
        static_cast<s32>(0x45df5c75L), static_cast<s32>(0xdcd60dcfL), static_cast<s32>(0xabd13d59L), static_cast<s32>(0x26d930acL), static_cast<s32>(0x51de003aL),
        static_cast<s32>(0xc8d75180L), static_cast<s32>(0xbfd06116L), static_cast<s32>(0x21b4f4b5L), static_cast<s32>(0x56b3c423L), static_cast<s32>(0xcfba9599L),
        static_cast<s32>(0xb8bda50fL), static_cast<s32>(0x2802b89eL), static_cast<s32>(0x5f058808L), static_cast<s32>(0xc60cd9b2L), static_cast<s32>(0xb10be924L),
        static_cast<s32>(0x2f6f7c87L), static_cast<s32>(0x58684c11L), static_cast<s32>(0xc1611dabL), static_cast<s32>(0xb6662d3dL), static_cast<s32>(0x76dc4190L),
        static_cast<s32>(0x01db7106L), static_cast<s32>(0x98d220bcL), static_cast<s32>(0xefd5102aL), static_cast<s32>(0x71b18589L), static_cast<s32>(0x06b6b51fL),
        static_cast<s32>(0x9fbfe4a5L), static_cast<s32>(0xe8b8d433L), static_cast<s32>(0x7807c9a2L), static_cast<s32>(0x0f00f934L), static_cast<s32>(0x9609a88eL),
        static_cast<s32>(0xe10e9818L), static_cast<s32>(0x7f6a0dbbL), static_cast<s32>(0x086d3d2dL), static_cast<s32>(0x91646c97L), static_cast<s32>(0xe6635c01L),
        static_cast<s32>(0x6b6b51f4L), static_cast<s32>(0x1c6c6162L), static_cast<s32>(0x856530d8L), static_cast<s32>(0xf262004eL), static_cast<s32>(0x6c0695edL),
        static_cast<s32>(0x1b01a57bL), static_cast<s32>(0x8208f4c1L), static_cast<s32>(0xf50fc457L), static_cast<s32>(0x65b0d9c6L), static_cast<s32>(0x12b7e950L),
        static_cast<s32>(0x8bbeb8eaL), static_cast<s32>(0xfcb9887cL), static_cast<s32>(0x62dd1ddfL), static_cast<s32>(0x15da2d49L), static_cast<s32>(0x8cd37cf3L),
        static_cast<s32>(0xfbd44c65L), static_cast<s32>(0x4db26158L), static_cast<s32>(0x3ab551ceL), static_cast<s32>(0xa3bc0074L), static_cast<s32>(0xd4bb30e2L),
        static_cast<s32>(0x4adfa541L), static_cast<s32>(0x3dd895d7L), static_cast<s32>(0xa4d1c46dL), static_cast<s32>(0xd3d6f4fbL), static_cast<s32>(0x4369e96aL),
        static_cast<s32>(0x346ed9fcL), static_cast<s32>(0xad678846L), static_cast<s32>(0xda60b8d0L), static_cast<s32>(0x44042d73L), static_cast<s32>(0x33031de5L),
        static_cast<s32>(0xaa0a4c5fL), static_cast<s32>(0xdd0d7cc9L), static_cast<s32>(0x5005713cL), static_cast<s32>(0x270241aaL), static_cast<s32>(0xbe0b1010L),
        static_cast<s32>(0xc90c2086L), static_cast<s32>(0x5768b525L), static_cast<s32>(0x206f85b3L), static_cast<s32>(0xb966d409L), static_cast<s32>(0xce61e49fL),
        static_cast<s32>(0x5edef90eL), static_cast<s32>(0x29d9c998L), static_cast<s32>(0xb0d09822L), static_cast<s32>(0xc7d7a8b4L), static_cast<s32>(0x59b33d17L),
        static_cast<s32>(0x2eb40d81L), static_cast<s32>(0xb7bd5c3bL), static_cast<s32>(0xc0ba6cadL), static_cast<s32>(0xedb88320L), static_cast<s32>(0x9abfb3b6L),
        static_cast<s32>(0x03b6e20cL), static_cast<s32>(0x74b1d29aL), static_cast<s32>(0xead54739L), static_cast<s32>(0x9dd277afL), static_cast<s32>(0x04db2615L),
        static_cast<s32>(0x73dc1683L), static_cast<s32>(0xe3630b12L), static_cast<s32>(0x94643b84L), static_cast<s32>(0x0d6d6a3eL), static_cast<s32>(0x7a6a5aa8L),
        static_cast<s32>(0xe40ecf0bL), static_cast<s32>(0x9309ff9dL), static_cast<s32>(0x0a00ae27L), static_cast<s32>(0x7d079eb1L), static_cast<s32>(0xf00f9344L),
        static_cast<s32>(0x8708a3d2L), static_cast<s32>(0x1e01f268L), static_cast<s32>(0x6906c2feL), static_cast<s32>(0xf762575dL), static_cast<s32>(0x806567cbL),
        static_cast<s32>(0x196c3671L), static_cast<s32>(0x6e6b06e7L), static_cast<s32>(0xfed41b76L), static_cast<s32>(0x89d32be0L), static_cast<s32>(0x10da7a5aL),
        static_cast<s32>(0x67dd4accL), static_cast<s32>(0xf9b9df6fL), static_cast<s32>(0x8ebeeff9L), static_cast<s32>(0x17b7be43L), static_cast<s32>(0x60b08ed5L),
        static_cast<s32>(0xd6d6a3e8L), static_cast<s32>(0xa1d1937eL), static_cast<s32>(0x38d8c2c4L), static_cast<s32>(0x4fdff252L), static_cast<s32>(0xd1bb67f1L),
        static_cast<s32>(0xa6bc5767L), static_cast<s32>(0x3fb506ddL), static_cast<s32>(0x48b2364bL), static_cast<s32>(0xd80d2bdaL), static_cast<s32>(0xaf0a1b4cL),
        static_cast<s32>(0x36034af6L), static_cast<s32>(0x41047a60L), static_cast<s32>(0xdf60efc3L), static_cast<s32>(0xa867df55L), static_cast<s32>(0x316e8eefL),
        static_cast<s32>(0x4669be79L), static_cast<s32>(0xcb61b38cL), static_cast<s32>(0xbc66831aL), static_cast<s32>(0x256fd2a0L), static_cast<s32>(0x5268e236L),
        static_cast<s32>(0xcc0c7795L), static_cast<s32>(0xbb0b4703L), static_cast<s32>(0x220216b9L), static_cast<s32>(0x5505262fL), static_cast<s32>(0xc5ba3bbeL),
        static_cast<s32>(0xb2bd0b28L), static_cast<s32>(0x2bb45a92L), static_cast<s32>(0x5cb36a04L), static_cast<s32>(0xc2d7ffa7L), static_cast<s32>(0xb5d0cf31L),
        static_cast<s32>(0x2cd99e8bL), static_cast<s32>(0x5bdeae1dL), static_cast<s32>(0x9b64c2b0L), static_cast<s32>(0xec63f226L), static_cast<s32>(0x756aa39cL),
        static_cast<s32>(0x026d930aL), static_cast<s32>(0x9c0906a9L), static_cast<s32>(0xeb0e363fL), static_cast<s32>(0x72076785L), static_cast<s32>(0x05005713L),
        static_cast<s32>(0x95bf4a82L), static_cast<s32>(0xe2b87a14L), static_cast<s32>(0x7bb12baeL), static_cast<s32>(0x0cb61b38L), static_cast<s32>(0x92d28e9bL),
        static_cast<s32>(0xe5d5be0dL), static_cast<s32>(0x7cdcefb7L), static_cast<s32>(0x0bdbdf21L), static_cast<s32>(0x86d3d2d4L), static_cast<s32>(0xf1d4e242L),
        static_cast<s32>(0x68ddb3f8L), static_cast<s32>(0x1fda836eL), static_cast<s32>(0x81be16cdL), static_cast<s32>(0xf6b9265bL), static_cast<s32>(0x6fb077e1L),
        static_cast<s32>(0x18b74777L), static_cast<s32>(0x88085ae6L), static_cast<s32>(0xff0f6a70L), static_cast<s32>(0x66063bcaL), static_cast<s32>(0x11010b5cL),
        static_cast<s32>(0x8f659effL), static_cast<s32>(0xf862ae69L), static_cast<s32>(0x616bffd3L), static_cast<s32>(0x166ccf45L), static_cast<s32>(0xa00ae278L),
        static_cast<s32>(0xd70dd2eeL), static_cast<s32>(0x4e048354L), static_cast<s32>(0x3903b3c2L), static_cast<s32>(0xa7672661L), static_cast<s32>(0xd06016f7L),
        static_cast<s32>(0x4969474dL), static_cast<s32>(0x3e6e77dbL), static_cast<s32>(0xaed16a4aL), static_cast<s32>(0xd9d65adcL), static_cast<s32>(0x40df0b66L),
        static_cast<s32>(0x37d83bf0L), static_cast<s32>(0xa9bcae53L), static_cast<s32>(0xdebb9ec5L), static_cast<s32>(0x47b2cf7fL), static_cast<s32>(0x30b5ffe9L),
        static_cast<s32>(0xbdbdf21cL), static_cast<s32>(0xcabac28aL), static_cast<s32>(0x53b39330L), static_cast<s32>(0x24b4a3a6L), static_cast<s32>(0xbad03605L),
        static_cast<s32>(0xcdd70693L), static_cast<s32>(0x54de5729L), static_cast<s32>(0x23d967bfL), static_cast<s32>(0xb3667a2eL), static_cast<s32>(0xc4614ab8L),
        static_cast<s32>(0x5d681b02L), static_cast<s32>(0x2a6f2b94L), static_cast<s32>(0xb40bbe37L), static_cast<s32>(0xc30c8ea1L), static_cast<s32>(0x5a05df1bL),
        static_cast<s32>(0x2d02ef8dL)
    };
    
    
    //-----------------------------------------------
    u32 CryptUtils::CRC32( const Byte* pData, u32 length )
    {
        u32 i, crc;
        
        crc = 0xFFFFFFFF;
        for (i = 0; i < length; ++i)
            crc = CRC32_table[(crc ^ pData[i]) & 0xff] ^ (crc >> 8);
        return crc^0xFFFFFFFF;
    }

    //-----------------------------------------------
    u32 CryptUtils::CRC32( const Byte* pData, u32 length, u32 preCrc32 )
    {
        if (preCrc32 == 0) {
            return CRC32(pData, length);
        }
        
        u32 i = 0;
        preCrc32 ^= 0xFFFFFFFF;
        for (i = 0; i < length; ++i)
            preCrc32 = CRC32_table[(preCrc32 ^ pData[i]) & 0xff] ^ (preCrc32 >> 8);
        return preCrc32^0xFFFFFFFF;
    }
    
    //-----------------------------------------------
    u32 CryptUtils::CRC32_segments( const Array<const Byte*>* pArrData, const Array<u32>* pArrLength )
    {
        assert(pArrData->size() == pArrLength->size());
        
        u32 i, crc, length;
        const Byte* pData;
        
        crc = 0xFFFFFFFF;
        
        for (u32 j = 0; j < pArrData->size(); j++)
        {
            pData = pArrData->getAt(j);
            length = pArrLength->getAt(j);
            for (i = 0; i < length; ++i)
                crc = CRC32_table[(crc ^ pData[i]) & 0xff] ^ (crc >> 8);
        }
        return crc^0xFFFFFFFF;
    }
    
    //-----------------------------------------------
    hyc::u32 CryptUtils::CRC32_file( const tchar* pszFilePath )
    {
        if ( NULL == pszFilePath || !FileUtils::FileIsExist(pszFilePath) )
            return 0;
        
        s64 cSize = FileUtils::FileGetSize( pszFilePath );
        if (0 >= cSize)
            return 0;
        
        FileStream fs;
        if (!fs.open( pszFilePath, FileStream::FM_Binary|FileStream::FM_Read ))
            return 0;
        
        FixArray<Byte>  buf( HYC_COMPILE_FILE_SEGMENT_BUF_SIZE > cSize ? (u32)cSize : HYC_COMPILE_FILE_SEGMENT_BUF_SIZE );
        
        u32 i, crc;
        crc = 0xFFFFFFFF;
        
        s64 pos = 0;
        s64 len;
        while (pos < cSize)
        {
            len = (cSize-pos);
            len = len > buf.size() ? (s64)buf.size() : len;
            
            if ( len != fs.readByte( &buf[0], (u32)len ) )
                return 0;
            
            for (i = 0; i < len; ++i)
                crc = CRC32_table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
            
            pos += len;
        } // while
        
        return crc^0xFFFFFFFF;
    }
    
    //-----------------------------------------------
    hyc::u32 CryptUtils::CRC32_fileStream( FileStream& fileStream, u32 pos, u32 count )
    {
        if ( !fileStream.isOpen() || count <= 0 )
            return 0;

        integer fpos = fileStream.tell();
        
        s64 cSize = (s64)fileStream.getByteSize();
        if (0 == cSize || cSize - pos < count)
            return 0;
        
        if (!fileStream.seek( pos, FileStream::SO_Begin ))
            return 0;
        
        FixArray<Byte>  buf( HYC_COMPILE_FILE_SEGMENT_BUF_SIZE > count ? count : HYC_COMPILE_FILE_SEGMENT_BUF_SIZE );
        
        u32 i, crc;
        crc = 0xFFFFFFFF;
        
        u32 pos1 = 0;
        while (pos1 < count)
        {
            u32 len = (count-pos1);
            len = len > buf.size() ? (u32)buf.size() : len;
            
            if ( len != fileStream.readByte( &buf[0], len ) )
            {
                fileStream.seek(fpos, FileStream::SO_Begin);
                return 0;
            }
            
            for (i = 0; i < len; ++i)
                crc = CRC32_table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
            
            pos1 += len;
        } // while
        
        fileStream.seek(fpos, FileStream::SO_Begin);
        return crc^0xFFFFFFFF;
    }
    
} // end namespace hyc
