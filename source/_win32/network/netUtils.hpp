//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_NETUTILS_HPP_20110503111106__
#define __BP_NETUTILS_HPP_20110503111106__
#ifdef HYC_COMPILE_MODULE_NET

#ifdef HYC_COMPILE_MODULE_NET_WLAN
#include "netUtils_wifiConfig.hpp"
#endif // HYC_COMPILE_MODULE_NET_WLAN

#include "../../../include/utils/stringUtils.h"

namespace hyc
{
#ifdef HYC_COMPILE_MODULE_NET_WLAN

    // 设置wifi profile
    static bool _WifiSetProfile( HANDLE hClientHandle, const GUID* pInterfaceGuid, PWLAN_AVAILABLE_NETWORK pNetwork, const tchar* key )
    {
    //$(SSID_NAME)
    //$(CONNECT_TYPE)   : ESS(infrastructure network), IBSS(ad-hoc network)
    //$(AUTH_TYPE)      : open/shared/WPA/WPAPSK/WPA2/WPA2PSK
    //$(ENCRYPTION)     : none/WEP/TKIP/AES
    //$(KEY)           

        cString profile;// = (StringUtils::GetLength(key) > 0) ? g_profileXML_wep : g_profileXML_nokey;

        if (pNetwork->bSecurityEnabled)
        {
            switch(pNetwork->dot11DefaultAuthAlgorithm)
            {
            case DOT11_AUTH_ALGO_80211_OPEN:
            case DOT11_AUTH_ALGO_80211_SHARED_KEY:
                profile = NET_WIFI_PROFILE_WEP;
                break;
            case DOT11_AUTH_ALGO_WPA:
            case DOT11_AUTH_ALGO_RSNA:
                profile = NET_WIFI_PROFILE_WPA;
                break;
            case DOT11_AUTH_ALGO_WPA_PSK:
            case DOT11_AUTH_ALGO_RSNA_PSK:
                profile = NET_WIFI_PROFILE_WPAPSK;
                break;
            default:    // 其他不支持
                return false;
            }
        }
        else
        {
            profile = NET_WIFI_PROFILE_NOKEY;
        }

        profile.replace( 0, profile.size(), "$(SSID_NAME)", (char*)pNetwork->dot11Ssid.ucSSID );
        profile.replace( 0, profile.size(), "$(CONNECT_TYPE)", (pNetwork->dot11BssType == dot11_BSS_type_infrastructure) ? "ESS" : "IBSS" );

        //$(AUTH_TYPE)      : open/shared/WPA/WPAPSK/WPA2/WPA2PSK
        switch(pNetwork->dot11DefaultAuthAlgorithm)
        {
        case DOT11_AUTH_ALGO_80211_OPEN:
            profile.replace( 0, profile.size(), "$(AUTH_TYPE)", "open" );
            break;
        case DOT11_AUTH_ALGO_80211_SHARED_KEY:
            profile.replace( 0, profile.size(), "$(AUTH_TYPE)", "shared" );
            break;
        case DOT11_AUTH_ALGO_WPA:
            profile.replace( 0, profile.size(), "$(AUTH_TYPE)", "WPA" );
            break;
        case DOT11_AUTH_ALGO_WPA_PSK:
            profile.replace( 0, profile.size(), "$(AUTH_TYPE)", "WPAPSK" );
            break;
        case DOT11_AUTH_ALGO_RSNA:
            profile.replace( 0, profile.size(), "$(AUTH_TYPE)", "WPA2" );
            break;
        case DOT11_AUTH_ALGO_RSNA_PSK:
            profile.replace( 0, profile.size(), "$(AUTH_TYPE)", "WPA2PSK" );
            break;
        }
        //$(ENCRYPTION)     : none/WEP/TKIP/AES
        switch(pNetwork->dot11DefaultCipherAlgorithm)
        {
        case DOT11_CIPHER_ALGO_NONE:
            profile.replace( 0, profile.size(), "$(ENCRYPTION)", "none" );
            break;
        case DOT11_CIPHER_ALGO_TKIP:
            profile.replace( 0, profile.size(), "$(ENCRYPTION)", "TKIP" );
            break;
        case DOT11_CIPHER_ALGO_WEP:
        case DOT11_CIPHER_ALGO_WEP40:
        case DOT11_CIPHER_ALGO_WEP104:
            profile.replace( 0, profile.size(), "$(ENCRYPTION)", "WEP" );
            break;
        case DOT11_CIPHER_ALGO_CCMP:
            profile.replace( 0, profile.size(), "$(ENCRYPTION)", "AES" );
            break;
        }
        profile.replace( 0, profile.size(), "$(KEY)", key );

        wString wprofile;
        StringUtils::Narrow2wide( profile, wprofile );

        DWORD dwReason;
        if ( ERROR_SUCCESS != ::WlanSetProfile( hClientHandle, pInterfaceGuid, 0, wprofile.c_str(), NULL, TRUE, NULL, &dwReason ) )
            return false;
        return true;
    }

    //-----------------------------------------------
    bool hyc::NetUtils::WifiAvailable()
    {
        // open wifi
        DWORD dwNegotiatedVer;
        HANDLE hClientHandle;
        if ( ERROR_SUCCESS != ::WlanOpenHandle( 1, NULL, &dwNegotiatedVer, &hClientHandle ) )
        {
            return false;
        }

        // get wlan interface
        PWLAN_INTERFACE_INFO_LIST   pList = NULL;
        if ( ERROR_SUCCESS != WlanEnumInterfaces( hClientHandle, NULL, &pList ) )
        {
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        if ( pList->dwNumberOfItems <= 0 )
        {
            // release
            ::WlanFreeMemory( pList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        // release
        ::WlanFreeMemory( pList );
        // close wifi
        ::WlanCloseHandle( hClientHandle, NULL );
        return true;
    }


    //----------------------------------------------
    bool NetUtils::WifiConnect( const tchar* SSID, const tchar* pwd )
    {
        if (0 >= StringUtils::GetLength(SSID) || 
            DOT11_SSID_MAX_LENGTH < StringUtils::GetLength(SSID))
        {
            return false;
        }

        WifiDelete( SSID );

        //--------------------------------
        // open wifi
        DWORD dwNegotiatedVer;
        HANDLE hClientHandle;
        if ( ERROR_SUCCESS != ::WlanOpenHandle( 1, NULL, &dwNegotiatedVer, &hClientHandle ) )
        {
            return false;
        }

        // get wlan interface
        PWLAN_INTERFACE_INFO_LIST   pList = NULL;
        if ( ERROR_SUCCESS != WlanEnumInterfaces( hClientHandle, NULL, &pList ) )
        {
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        if ( pList->dwNumberOfItems <= 0 )
        {
            // release
            ::WlanFreeMemory( pList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }


        //-----------------------------------------------
        // get available network
        PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = NULL;
        PWLAN_AVAILABLE_NETWORK pNetwork = NULL;

        if ( ERROR_SUCCESS != ::WlanGetAvailableNetworkList( hClientHandle, &pList->InterfaceInfo[0].InterfaceGuid,
            WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES, NULL, &pNetworkList ) )
        {
            // release
            ::WlanFreeMemory( pList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        cString  ssid(SSID);
        for (uinteger i = 0; i < pNetworkList->dwNumberOfItems; ++i)
        {
            if ((char*)(pNetworkList->Network+i)->dot11Ssid.ucSSID == ssid)
            {
                pNetwork = pNetworkList->Network+i;

                if ( StringUtils::GetLength(pNetwork->strProfileName) <= 0 && WLAN_AVAILABLE_NETWORK_CONNECTED != pNetwork->dwFlags )
                {
                    if (!_WifiSetProfile( hClientHandle, &pList->InterfaceInfo[0].InterfaceGuid, pNetwork, pwd ))
                    {
                        // release
                        ::WlanFreeMemory( pList );
                        ::WlanFreeMemory( pNetworkList );
                        // close wifi
                        ::WlanCloseHandle( hClientHandle, NULL );
                        return false;
                    }
                }
                break;
            }
        }

        if (NULL == pNetwork)
        {
            // release
            ::WlanFreeMemory( pList );
            ::WlanFreeMemory( pNetworkList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }
        // 已连接
        if (WLAN_AVAILABLE_NETWORK_CONNECTED == pNetwork->dwFlags)
        {
            // release
            ::WlanFreeMemory( pList );
            ::WlanFreeMemory( pNetworkList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return true;
        }

        //--------------------------------
        // connect

        WLAN_CONNECTION_PARAMETERS  wcp;
        ZeroMemory(&wcp, sizeof(WLAN_CONNECTION_PARAMETERS));
        wcp.wlanConnectionMode  = wlan_connection_mode_profile;
        wcp.strProfile          = pNetwork->strProfileName;
        wcp.pDot11Ssid          = &pNetwork->dot11Ssid;
        wcp.pDesiredBssidList   = NULL;
        wcp.dot11BssType        = pNetwork->dot11BssType;
        wcp.dwFlags             = 0;

        DWORD dw = ::WlanConnect( hClientHandle, &pList->InterfaceInfo[0].InterfaceGuid, &wcp, NULL );

        if ( ERROR_SUCCESS != ::WlanConnect( hClientHandle, &pList->InterfaceInfo[0].InterfaceGuid, &wcp, NULL ) )
        {
            // release
            ::WlanFreeMemory( pList );
            ::WlanFreeMemory( pNetworkList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        //--------------------------------
        // release
        ::WlanFreeMemory( pList );
        ::WlanFreeMemory( pNetworkList );
        // close wifi
        ::WlanCloseHandle( hClientHandle, NULL );
        return true;
    }

    //-----------------------------------------------
    bool hyc::NetUtils::WifiGetAvailableNetworkList( OUT Array<Wifi_Network_Info>* pinfoList )
    {
        Array<Wifi_Network_Info>& infoList = *pinfoList;
        infoList.clear();

        //-----------------------------------------------
        // open wifi
        DWORD dwNegotiatedVer;
        HANDLE hClientHandle;
        if ( ERROR_SUCCESS != ::WlanOpenHandle( 1, NULL, &dwNegotiatedVer, &hClientHandle ) )
        {
            return false;
        }

        // get wlan interface
        PWLAN_INTERFACE_INFO_LIST   pList = NULL;
        if ( ERROR_SUCCESS != WlanEnumInterfaces( hClientHandle, NULL, &pList ) )
        {
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        if ( pList->dwNumberOfItems <= 0 )
        {
            // release
            ::WlanFreeMemory( pList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        //-----------------------------------------------
        // get available network
        PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = NULL;
        PWLAN_AVAILABLE_NETWORK pNetwork = NULL;

        if ( ERROR_SUCCESS != ::WlanGetAvailableNetworkList( hClientHandle, &pList->InterfaceInfo[0].InterfaceGuid,
            WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES, NULL, &pNetworkList ) )
        {
            // release
            ::WlanFreeMemory( pList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        Wifi_Network_Info info;
        info._ssid.reserve(DOT11_SSID_MAX_LENGTH);
        for (uinteger i = 0; i < pNetworkList->dwNumberOfItems; ++i)
        {
            pNetwork = (PWLAN_AVAILABLE_NETWORK)(&pNetworkList->Network[i]);
            if ( StringUtils::GetLength(pNetwork->strProfileName) > 0 )
            {
                continue;
            }

#if defined(_UNICODE) || defined(UNICODE)
            StringUtils::Wide2narrow( pNetwork->dot11Ssid.ucSSID, info._ssid);
#else
            info._ssid = (char*)pNetwork->dot11Ssid.ucSSID;
#endif
            info._isConnectable = (1==pNetwork->bNetworkConnectable) ? true : false;
            info._isSecurity = (1==pNetwork->bSecurityEnabled) ? true : false;

            infoList.push_back(info);
        }

        // release
        ::WlanFreeMemory( pList );
        ::WlanFreeMemory( pNetworkList );
        // close wifi
        ::WlanCloseHandle( hClientHandle, NULL );
        return true;
    }

    //-----------------------------------------------
    bool hyc::NetUtils::WifiDelete( const tchar* SSID )
    {
        if (0 >= StringUtils::GetLength(SSID) || 
            DOT11_SSID_MAX_LENGTH < StringUtils::GetLength(SSID))
        {
            return false;
        }

        //--------------------------------
        // open wifi
        DWORD dwNegotiatedVer;
        HANDLE hClientHandle;
        if ( ERROR_SUCCESS != ::WlanOpenHandle( 1, NULL, &dwNegotiatedVer, &hClientHandle ) )
        {
            return false;
        }

        // get wlan interface
        PWLAN_INTERFACE_INFO_LIST   pList = NULL;
        if ( ERROR_SUCCESS != WlanEnumInterfaces( hClientHandle, NULL, &pList ) )
        {
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        if ( pList->dwNumberOfItems <= 0 )
        {
            // release
            ::WlanFreeMemory( pList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        //--------------------------------
        //  delete wifi
        wString wssid;
        wssid.reserve( DOT11_SSID_MAX_LENGTH + 1 );
        StringUtils::Narrow2wide( SSID, StringUtils::GetLength( SSID ), wssid );
        if ( ERROR_SUCCESS != ::WlanDeleteProfile( hClientHandle, &pList->InterfaceInfo[0].InterfaceGuid, wssid.c_str(), NULL ) )
        {
            ::WlanFreeMemory( pList );
            // close wifi
            ::WlanCloseHandle( hClientHandle, NULL );
            return false;
        }

        //--------------------------------
        // release
        ::WlanFreeMemory( pList );
        // close wifi
        ::WlanCloseHandle( hClientHandle, NULL );
        return true;

    }
#endif // HYC_COMPILE_MODULE_NET_WLAN


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
#endif // end of define __BP_NETUTILS_HPP_20110503111106__