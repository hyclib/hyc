//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  网络工具; 如无线配置等
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_NETUTILS_H_20110503105736__
#define __BP_NETUTILS_H_20110503105736__

namespace hyc
{
#ifdef HYC_COMPILE_MODULE_NET_WLAN
    //----------------------------------------------------------------------------------------------------
    //!	wifi structure
    //----------------------------------------------------------------------------------------------------
    struct Wifi_Network_Info
    {
        String  _ssid;
        bool    _isConnectable; // 是否可连接
        bool    _isSecurity;    // 是否加密
    };
#endif // HYC_COMPILE_MODULE_NET_WLAN

    //----------------------------------------------------------------------------------------------------
    //!	Net utilitys
    //----------------------------------------------------------------------------------------------------
     
	class HYC_API NetUtils
    {
    public:

#ifdef HYC_COMPILE_MODULE_NET_WLAN
        /*---------------------------
          wifi api
        ---------------------------*/
        //static String   WifiGetConnectedSSID();
        static bool     WifiAvailable();    // wifi 是否有效
        static bool     WifiConnect( const tchar* SSID, const tchar* pwd );
        static bool     WifiDelete( const tchar* SSID );
        static bool     WifiGetAvailableNetworkList( OUT Array<Wifi_Network_Info>* infoList );
#endif // HYC_COMPILE_MODULE_NET_WLAN

    };

} // end namespace hyc

#endif // end of define __BP_NETUTILS_H_20110503105736__

#endif // HYC_COMPILE_MODULE_NET
