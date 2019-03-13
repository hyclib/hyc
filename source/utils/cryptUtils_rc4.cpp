//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_CRYPT_RC4
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
    //*	RC4
    //----------------------------------------------------------------------------------------------------


    //-----------------------------------------------
    void CryptUtils::RC4_Crypt( OUT Byte* pData, u32 cLenData, const Byte* pKey, u32 cLenKey )
    {
        const u32 RC4_LEN = 256;
        const u32 RC4_VARIETY = 0;
        s32 RC4_SBOX[RC4_LEN] = {0};
        
        if ( NULL == pData || NULL == pKey || 0 >= cLenKey || 0 >= cLenData )
        {
            return;
        }

        /*---------------------------
        初始化; 打乱 s-box, 确保每个元素都被处理
        j 确保s-box的打乱是随机的
        ---------------------------*/

        u32 i = 0;
        s32 j = 0;
        s32 k = 0;

        for ( i = 0; i < RC4_LEN; ++i )
        {
            RC4_SBOX[i] = i;
        }

        for ( i = 0; i < RC4_LEN; ++i )
        {
            j = ( j + RC4_SBOX[i] + pKey[i%cLenKey] + RC4_VARIETY ) % RC4_LEN;
            Math::Swap( RC4_SBOX[i], RC4_SBOX[j] );
        }

        /*---------------------------
        加密
        ---------------------------*/

        i = j = 0;

        for ( u32 n = 0; n < cLenData; ++n )
        {
            ++i %= RC4_LEN;
            j = (j+RC4_SBOX[i]) % RC4_LEN;

            Math::Swap( RC4_SBOX[i], RC4_SBOX[j] );

            k = RC4_SBOX[ (RC4_SBOX[i]+RC4_SBOX[j]) % RC4_LEN ];    // 子密码

            // 加密
            pData[n] ^= k;
        }
    }

    //-----------------------------------------------
    void CryptUtils::RC4_Decrypt( OUT Byte* pData, u32 cLenData, const Byte* pKey, u32 cLenKey )
    {
        RC4_Crypt( pData, cLenData, pKey, cLenKey );
    }


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_CRYPT_RC4
