//
//  _mobile_device.mm
//  hyc
//
//  Created by 李 鹏翔 on 12-5-14.
//  Copyright (c) 2012年 华耀信息科技有限公司. All rights reserved.
//

#import "../preheader.h"
#import "../../include/os/thread.h"
#import "../../include/_platform/iOS.h"
using namespace hyc;
#ifdef HYC_COMPILE_MODULE_MOBILE


#import <UIKit/UIKit.h>
#import "_mobile_global.h"
using namespace hymobile;

#define NS_SAFE_RELEASE(x)  //if (nil != x) { [x release]; x = nil; }


__OrientationNotification* g_mobile_ori_notification    = nil;
__ConnectionInstance*      g_mobile_connection          = nil;
hymobile::MotionInstance*  g_mobile_motion              = NULL;


namespace hymobile {
    
    //-----------------------------------------------
    DeviceOrientationDelegate::DeviceOrientationDelegate()
    {
        if (g_mobile_ori_notification == nil)
            g_mobile_ori_notification = [[__OrientationNotification alloc] init];
        [g_mobile_ori_notification add:this];
    }
    
    //-----------------------------------------------
    DeviceOrientationDelegate::~DeviceOrientationDelegate()
    {
        if (g_mobile_ori_notification != nil)
        {
            [g_mobile_ori_notification remove:this];
        }
    }
    
    //-----------------------------------------------
    void Device::Initial()
    {
        NSLog(@"\r\n[Device::Initial]\r\n");
    }
    
    //-----------------------------------------------
    void Device::Finally()
    {
        NS_SAFE_RELEASE(g_mobile_ori_notification);
        NS_SAFE_RELEASE(g_mobile_connection);
        SAFE_DELETE(g_mobile_motion);
 
        NSLog(@"\r\n[Device::Finally]\r\n");
    }
    
    //-----------------------------------------------
    DeviceOrientation Device::GetOrientation()
    {
        switch ([[UIDevice currentDevice] orientation]) 
        {
            case UIDeviceOrientationFaceDown:
                return DeviceOrientation_FaceDown;
                break;
            case UIDeviceOrientationFaceUp:
                return DeviceOrientation_FaceUp;
                break;
            case UIDeviceOrientationLandscapeLeft:
                return DeviceOrientation_LandscapeLeft;
                break;
            case UIDeviceOrientationLandscapeRight:
                return DeviceOrientation_LandscapeRight;
                break;
            case UIDeviceOrientationPortrait:
                return DeviceOrientation_Portrait;
                break;
            case UIDeviceOrientationPortraitUpsideDown:
                return DeviceOrientation_PortraitUpsideDown;
                break;
            default:
                return DeviceOrientation_Unknown;
                break;
        }
    }
    
    //-----------------------------------------------
    void Device::SetOrientation(DeviceOrientation ori, bool animatedc)
    {
        UIInterfaceOrientation oric;
        switch (ori) {
            case DeviceOrientation_FaceDown:
                oric = UIInterfaceOrientationPortrait;
                break;
            case DeviceOrientation_FaceUp:
                oric = UIInterfaceOrientationPortrait;
                break;
            case DeviceOrientation_LandscapeLeft:
                oric = UIInterfaceOrientationLandscapeLeft;
                break;
            case DeviceOrientation_LandscapeRight:
                oric = UIInterfaceOrientationLandscapeRight;
                break;
            case DeviceOrientation_Portrait:
                oric = UIInterfaceOrientationPortrait;
                break;
            case DeviceOrientation_PortraitUpsideDown:
                oric = UIInterfaceOrientationPortraitUpsideDown;
                break;
            default:
                oric = UIInterfaceOrientationPortrait;
                break;
        }

        [PlatformIOS::AppSingleton() setStatusBarOrientation:oric animated:animatedc];
    }
    
    f64 Device::GetOrientationChangeDelay()
    {
        if (nil != g_mobile_ori_notification)
            return g_mobile_ori_notification->g_oriChangeDelay;
        return -1;
    }
    void Device::SetOrientationChangeDelay( const f64& delay )
    {
        if (nil != g_mobile_ori_notification)
            g_mobile_ori_notification->g_oriChangeDelay = delay;
    }
    
    //-----------------------------------------------
    const String& Device::GetModel()
    {
        static String model = PlatformIOS::ConvertToString([[UIDevice currentDevice] localizedModel]);
        return model;
    }
    
    //-----------------------------------------------
    const String& Device::GetName()
    {
        static String deviceName =PlatformIOS::ConvertToString([[UIDevice currentDevice] name]);
        return deviceName;
    }
    
    //-----------------------------------------------
    const DevicePlatform& Device::GetPlatform()
    {
        static DevicePlatform devicePlatform = DevicePlatform_iOS;
        return devicePlatform;
    }
    
    //-----------------------------------------------
    const String& Device::GetVersion()
    {
        static String deviceVer = PlatformIOS::ConvertToString([[UIDevice currentDevice] systemVersion]);
        return deviceVer;
    }
    
    
} // end namespace hymobile

#endif

