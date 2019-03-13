//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_CRYPT_MD5
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
    //*	md5
    //----------------------------------------------------------------------------------------------------


    /*
    * md5 -- compute and check MD5 message digest.
    * this version only can calculate the char string.
    *
    * MD5 (Message-Digest algorithm 5) is a widely used, partially
    * insecure cryptographic hash function with a 128-bit hash value.
    *
    * Author: redraiment
    * Date: Aug 27, 2008
    * Version: 0.1.6
    */

#define MD5_SINGLE_ONE_BIT      0x80
#define MD5_BLOCK_SIZE          512
#define MD5_MOD_SIZE            448
#define MD5_APP_SIZE            64
#define MD5_BITS                8

    // MD5 Chaining Variable
#define MD5_A 0x67452301UL
#define MD5_B 0xEFCDAB89UL
#define MD5_C 0x98BADCFEUL
#define MD5_D 0x10325476UL

    
    typedef struct {
        Byte* message;
        u64 length;
    } MD5_STRING;

    // prototype
    // MD5_F, MD5_G, MD5_H and MD5_I are basic MD5 functions.
    u32 MD5_F( const u32& X, const u32& Y, const u32& Z );
    u32 MD5_G( const u32& X, const u32& Y, const u32& Z );
    u32 MD5_H( const u32& X, const u32& Y, const u32& Z );
    u32 MD5_I( const u32& X, const u32& Y, const u32& Z );
    // rotates x left s bits.
    u32 MD5_ROTATE_LEFT( const u32& x, const u32& s );
    // Pre-processin
    u32 MD5_COUNT_PADDING_BITS ( const u32& length );
    // append padding bits
    MD5_STRING MD5_APPEND_PADDING_BITS ( const Byte * argv, const u32& length );

    static const u32 MD5_X[4][2] = {{0, 1}, {1, 5}, {5, 3}, {0, 7}};

    // Constants for MD5 transform routine.
    static const u32 MD5_ROUTINE[4][4] = {
        { 7, 12, 17, 22 },
        { 5, 9, 14, 20 },
        { 4, 11, 16, 23 },
        { 6, 10, 15, 21 }
    };

    // MD5_F, MD5_G, MD5_H and MD5_I are basic MD5 functions.
    u32 MD5_F( const u32& X, const u32& Y, const u32& Z )    {   return ( X & Y ) | ( ~X & Z );  }
    u32 MD5_G( const u32& X, const u32& Y, const u32& Z )    {   return ( X & Z ) | ( Y & ~Z );  }
    u32 MD5_H( const u32& X, const u32& Y, const u32& Z )    {   return X ^ Y ^ Z;   }
    u32 MD5_I( const u32& X, const u32& Y, const u32& Z )    {   return Y ^ ( X | ~Z );  }

    // rotates x left s bits.
    u32 MD5_ROTATE_LEFT( const u32& x, const u32& s ) {   return ( x << s ) | ( x >> ( 32 - s ) );    }

    // Pre-processin
    u32 MD5_COUNT_PADDING_BITS ( const u32& length )   {   
        //u32 div = length * MD5_BITS / MD5_BLOCK_SIZE;
        u32 mod = length * MD5_BITS % MD5_BLOCK_SIZE;
        u32 c_bits;
        if ( mod == 0 )
            c_bits = MD5_MOD_SIZE;
        else
            c_bits = ( MD5_MOD_SIZE + MD5_BLOCK_SIZE - mod ) % MD5_BLOCK_SIZE;
        return c_bits / MD5_BITS;
    }

    // append padding bits
    MD5_STRING MD5_APPEND_PADDING_BITS ( const Byte * argv, const u32& length )  {
        u32 msg_length = length;
        u32 bit_length = MD5_COUNT_PADDING_BITS ( msg_length );
        u64 app_length = msg_length * MD5_BITS;
        MD5_STRING string;
        string.message = (Byte *)MemoryAlloc::Malloc(msg_length + bit_length + MD5_APP_SIZE / MD5_BITS);
        // Save message
        memcpy( string.message, argv, msg_length );
        // Pad out to mod 64.
        memset ( string.message + msg_length, 0, bit_length );
        string.message [ msg_length ] = (Byte)MD5_SINGLE_ONE_BIT;
        // Append length (before padding).
        memmove ( string.message + msg_length + bit_length, (Byte *)&app_length, sizeof( u64 ) );
        string.length = msg_length + bit_length + sizeof( u64 );
        return string;
    }

    //-----------------------------------------------
    bool CryptUtils::Md5( const Byte* pData, u32 length, OUT Digest_128& digest )
    {
        ZeroMemory( digest.digest, sizeof(digest.digest) );
        if ( NULL == pData || 0 >= length)
            return false;

        MD5_STRING string;
        u32 w[16];
        u32 chain[4];
        u32 state[4];
        u8 r[16];
        u32 ( *auxi[ 4 ])( const u32&, const u32&, const u32& ) = { MD5_F, MD5_G, MD5_H, MD5_I };
        integer roundIdx;
        integer sIdx = 0;
        integer wIdx = 0;
        integer i = 0;
        u32 j = 0;

        // 填充字符串
        string = MD5_APPEND_PADDING_BITS ( pData, length );

        // MD5 initialization.
        chain[0] = MD5_A;
        chain[1] = MD5_B;
        chain[2] = MD5_C;
        chain[3] = MD5_D;

        for ( j = 0; j < string.length; j += MD5_BLOCK_SIZE / MD5_BITS)
        {
            memmove ( (char *)w, string.message + j, MD5_BLOCK_SIZE / MD5_BITS );
            memmove ( state, chain, sizeof(chain) );
            for ( roundIdx = 0; roundIdx < 4; roundIdx++ )
            {
                wIdx = MD5_X[ roundIdx ][ 0 ];
                sIdx = 0;
                for ( i = 0; i < 16; i++ )
                {
                    // FMD5_F, GMD5_G, HMD5_H, and IMD5_I transformations for rounds 1, 2, 3, and 4.
                    // Rotation is separate from addition to prevent recomputation.
                    state[sIdx] = state [ (sIdx + 1) % 4 ] +
                        MD5_ROTATE_LEFT ( state[sIdx] +
                        ( *auxi[ roundIdx ] )
                        ( state[(sIdx+1) % 4], state[(sIdx+2) % 4], state[(sIdx+3) % 4]) +
                        w[ wIdx ] +
                        (u32)floor( (1ULL << 32) * fabs(sin( (double)roundIdx * 16 + i + 1 )) ),
                        MD5_ROUTINE[ roundIdx ][ i % 4 ]);
                    sIdx = ( sIdx + 3 ) % 4;
                    wIdx = ( wIdx + MD5_X[ roundIdx ][ 1 ] ) & 0xF;
                }
            }
            chain[ 0 ] += state[ 0 ];
            chain[ 1 ] += state[ 1 ];
            chain[ 2 ] += state[ 2 ];
            chain[ 3 ] += state[ 3 ];
        }
        memmove ( r + 0, (char *)&chain[0], sizeof(u32) );
        memmove ( r + 4, (char *)&chain[1], sizeof(u32) );
        memmove ( r + 8, (char *)&chain[2], sizeof(u32) );
        memmove ( r + 12, (char *)&chain[3], sizeof(u32) );

        MemoryAlloc::Free( string.message );

        for ( i = 0; i < 16; i++ )
            digest.digest[i] = r[i];

        return true;
    }
    // 
    // 	//-----------------------------------------------
    // 	bool CryptUtils::Md5( const Byte* pData, u32 length, OUT String& digest )
    // 	{
    // 		Digest_128 szDigest;
    // 		bool r = Md5( pData, length, szDigest );
    // 
    // #if defined(_UNICODE) || defined(UNICODE)
    //         static const wchar_t HEX[16] = {
    //             L'0', L'1', L'2', L'3',
    //             L'4', L'5', L'6', L'7',
    //             L'8', L'9', L'a', L'b',
    //             L'c', L'd', L'e', L'f'
    //         };
    // #else
    //         static const char HEX[16] = {
    //             '0', '1', '2', '3',
    //             '4', '5', '6', '7',
    //             '8', '9', 'a', 'b',
    //             'c', 'd', 'e', 'f'
    //         };
    // #endif // defined(_UNICODE) || defined(UNICODE)
    // 
    // 		// bytes to hex string
    // 		if (r)
    // 		{
    //             digest.clear();
    // 			digest.reserve(16*2);
    // 			for (size_t i = 0; i < 16; ++i) {
    // 				integer t = szDigest.digest[i];
    // 				integer a = t / 16;
    // 				integer b = t % 16;
    // 				digest.append(1, HEX[a]);
    // 				digest.append(1, HEX[b]);
    // 			}
    // 		}
    // 
    // 		return r;
    // 	}


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_CRYPT_MD5
