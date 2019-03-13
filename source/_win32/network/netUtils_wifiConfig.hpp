//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_NETUTILS_WIFICONFIG_HPP_20110509174438__
#define __BP_NETUTILS_WIFICONFIG_HPP_20110509174438__


namespace hyc
{

    //--------------------------------
    // 安全类型: no key
    // 
#define NET_WIFI_PROFILE_NOKEY \
"<?xml version=\"1.0\"?>\
 <WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
    <name>$(SSID_NAME)</name>\
    <SSIDConfig>\
        <SSID>\
            <name>$(SSID_NAME)</name>\
        </SSID>\
    </SSIDConfig>\
    <connectionType>$(CONNECT_TYPE)</connectionType>\
    <connectionMode>auto</connectionMode>\
    <autoSwitch>true</autoSwitch>\
    <MSM>\
        <security>\
            <authEncryption>\
                <authentication>$(AUTH_TYPE)</authentication>\
                <encryption>$(ENCRYPTION)</encryption>\
                <useOneX>false</useOneX>\
            </authEncryption>\
        </security>\
    </MSM>\
</WLANProfile>"

    //--------------------------------
    // 安全类型: WEP
    // 安全选项: open / share
    // 密钥格式: 16进制 / ASCII码
#define NET_WIFI_PROFILE_WEP \
"<?xml version=\"1.0\"?>\
 <WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
    <name>$(SSID_NAME)</name>\
    <SSIDConfig>\
        <SSID>\
            <name>$(SSID_NAME)</name>\
        </SSID>\
    </SSIDConfig>\
    <connectionType>$(CONNECT_TYPE)</connectionType>\
    <connectionMode>auto</connectionMode>\
    <autoSwitch>true</autoSwitch>\
    <MSM>\
        <security>\
            <authEncryption>\
                <authentication>$(AUTH_TYPE)</authentication>\
                <encryption>$(ENCRYPTION)</encryption>\
                <useOneX>false</useOneX>\
            </authEncryption>\
            <sharedKey>\
                <keyType>networkKey</keyType>\
                <protected>false</protected>\
                <keyMaterial>$(KEY)</keyMaterial>\
            </sharedKey>\
            <keyIndex>0</keyIndex>\
        </security>\
    </MSM>\
</WLANProfile>"

    //--------------------------------
    // 安全类型: WPA / WPA2
    // 安全选项: WPA / WPA2
    // 密钥格式: TKIP / AEC
#define NET_WIFI_PROFILE_WPA \
"<?xml version=\"1.0\"?>\
<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
    <name>$(SSID_NAME)</name>\
    <SSIDConfig>\
        <SSID>\
            <name>$(SSID_NAME)</name>\
        </SSID>\
    </SSIDConfig>\
    <connectionType>$(CONNECT_TYPE)</connectionType>\
    <connectionMode>auto</connectionMode>\
    <autoSwitch>true</autoSwitch>\
    <MSM>\
        <security>\
            <authEncryption>\
                <authentication>$(AUTH_TYPE)</authentication>\
                <encryption>$(ENCRYPTION)</encryption>\
                <useOneX>false</useOneX>\
            </authEncryption>\
            <sharedKey>\
                <keyType>passPhrase</keyType>\
                <protected>false</protected>\
                <keyMaterial>$(KEY)</keyMaterial>\
            </sharedKey>\
        </security>\
    </MSM>\
</WLANProfile>"

    //--------------------------------
    // 安全类型: WPA-PSK / WPA2-PSK
    // 安全选项: WPA-PSK / WPA2-PSK
    // 密钥格式: TKIP / AEC
#define NET_WIFI_PROFILE_WPAPSK \
"<?xml version=\"1.0\"?>\
 <WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
    <name>$(SSID_NAME)</name>\
    <SSIDConfig>\
        <SSID>\
            <name>$(SSID_NAME)</name>\
        </SSID>\
    </SSIDConfig>\
    <connectionType>$(CONNECT_TYPE)</connectionType>\
    <connectionMode>auto</connectionMode>\
    <autoSwitch>true</autoSwitch>\
    <MSM>\
        <security>\
            <authEncryption>\
                <authentication>$(AUTH_TYPE)</authentication>\
                <encryption>$(ENCRYPTION)</encryption>\
                <useOneX>false</useOneX>\
            </authEncryption>\
            <sharedKey>\
                <keyType>passPhrase</keyType>\
                <protected>false</protected>\
                <keyMaterial>$(KEY)</keyMaterial>\
            </sharedKey>\
        </security>\
    </MSM>\
</WLANProfile>"


} // end namespace hyc

#endif // end of define __BP_NETUTILS_WIFICONFIG_HPP_20110509174438__