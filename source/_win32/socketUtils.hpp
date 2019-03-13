//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// YunFramework Gateway
// author:       lipengxiang
// description:  
//

#ifndef __BP_SOCKETUTILS_HPP_20110819140343__
#define __BP_SOCKETUTILS_HPP_20110819140343__


namespace hyc
{
    //-----------------------------------------------
    integer SocketUtils::GetAdapterNum( OUT Array<String>* pArrNames )
    {
        unsigned long int               ulSize = 0;
        PIP_ADAPTER_INFO    pInfo = NULL, pSrc = NULL;

        DWORD temp = 0;
        temp = GetAdaptersInfo( pInfo, &ulSize ); // 第一次调用，获取缓冲区大小
        //         if ( ERROR_SUCCESS != temp )
        //         {
        //             return -1;
        //         }

        // 分配空间
        pInfo = (PIP_ADAPTER_INFO)malloc( ulSize );
        pSrc = pInfo;
        temp = GetAdaptersInfo( pInfo, &ulSize );

        if ( ERROR_SUCCESS != temp )
        {
            free( pSrc );
            return -1;
        }

        // 遍历每一张网卡
        integer iCount = 0;
        while ( pInfo ) {
            pInfo = pInfo->Next;
            ++iCount;
        } // while

        free( pSrc );
        return iCount;
    }

    //-----------------------------------------------
    hyc::MacAddress SocketUtils::GetAdapterAddr( uinteger index )
    {
        ULONG               ulSize = 0;
        PIP_ADAPTER_INFO    pInfo = NULL, pSrc = NULL;
        MacAddress          addr;
        memset( addr.addr, 0, sizeof(addr.addr)/sizeof(addr.addr[0]) );

        DWORD temp = 0;
        temp = GetAdaptersInfo( pInfo, &ulSize ); // 第一次调用，获取缓冲区大小
        //         if ( ERROR_SUCCESS != temp )
        //         {
        //             return addr;
        //         }

        // 分配空间
        pInfo = (PIP_ADAPTER_INFO)malloc( ulSize );
        pSrc = pInfo;
        temp = GetAdaptersInfo( pInfo, &ulSize );

        if ( ERROR_SUCCESS != temp )
        {
            free( pSrc );
            return addr;
        }

        // 遍历每一张网卡
        uinteger iCount = 0;
        while ( pInfo ) {

            if ( index == iCount )
            {
                for ( uinteger i = 0; i < sizeof(addr.addr)/sizeof(addr.addr[0]); ++i )
                {
                    addr.addr[i] = pInfo->Address[i];
                }
                break;
            }

            pInfo = pInfo->Next;
            ++iCount;
        } // while

        free( pSrc );
        return addr;
    }

} // end namespace hyc

#endif // end of define __BP_SOCKETUTILS_HPP_20110819140343__

