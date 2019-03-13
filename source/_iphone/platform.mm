/*
 *  iOS.mm
 *  common
 *
 *  Created by pengxiang li on 2/22/11.
 *  Copyright 2011 hy. All rights reserved.
 *
 */


#include <sys/utsname.h>
#include "../preheader.h"
#include "../../include/container/array.h"
#include "../../include/utils/stringUtils.h"
#include "../../include/_platform/iOS.h"
#include "../../include/utils/fileUtils.h"

namespace hyc {
    
#pragma mark string函数
    
    //---------------------------------------
    String PlatformIOS::ConvertToString( NSString* str, Encoding encoding )
    {
        if (str == nil || str.length == 0)
            return NULL;
        
#if defined(_UNICODE) || defined(UNICODE)
        wString wstr;
        StringUtils::StrToStr([str cStringUsingEncoding:encoding], wstr);
        return wstr;
#else
        return [str cStringUsingEncoding:encoding];
#endif // defined(_UNICODE) || defined(UNICODE)        
    }
    
    //---------------------------------------
    NSString* PlatformIOS::ConvertToNSString( const char* pStr, Encoding encoding )
    {
        if (StringUtils::IsEmpty(pStr))
            return nil;
        
        return [NSString stringWithCString:pStr encoding:encoding];
    }
    
    //---------------------------------------
    NSString* PlatformIOS::ConvertToNSString( const wchar_t* pStr, Encoding encoding )
    {
        if (StringUtils::IsEmpty(pStr))
            return nil;
        
        cString str;
        StringUtils::StrToStr(pStr, str);
        
        if (str.empty())
            return @"";
        
        return [NSString stringWithCString:str.c_str() encoding:encoding];
    }
    
    //-----------------------------------------------
    NSString* PlatformIOS::GetLocalizedString(NSString *key)
    {
        return NSLocalizedString(key,nil);
    }

#pragma mark 特殊程序位置
    
    //---------------------------------------
    const String& PlatformIOS::GetDirVideo()
    {
        static String dir = _T("/private/var/mobile/Media/iphone video Recorder/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirPicture()
    {
        static String dir = _T("/private/var/mobile/Media/DCIM/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirMusic()
    {
        static String dir = _T("/private/var/mobile/Media/iTunes_Control/Music/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirEBook()
    {
        static String dir = _T("/private/var/root/Media/EBooks/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirRingtones()
    {
        static String dir = _T("/Library/Ringtones/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirSysAddressBook()
    {
        static String dir = _T("/private/var/mobile/Library/AddressBook/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirRecorder()
    {
        static String dir = _T("/private/var/mobile/Media/iphone Recorder/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirWallpaper()
    {
        static String dir = _T("/Library/Wallpaper/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirSysAudioUISounds()
    {
        static String dir = _T("/System/Library/Audio/UISounds/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirSysSMS()
    {
        static String dir = _T("/private/var/mobile/Library/SMS/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirSysFontCache()
    {
        static String dir = _T("/System/Library/Fonts/Cache/");
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirSysDocument()
    {
        static String dir = _T("/private/var/mobile/documents/");
        return dir;
    }
    
    /*/---------------------------------------
    String PlatformIOS::GetDirMovie()
    {
        static String dir;
        
        if (dir.empty()) {
            // get the sandbox path of program
            NSArray* patchs = NSSearchPathForDirectoriesInDomains(NSMoviesDirectory, NSLocalDomainMask, YES);
            
            // [patchs objectAtIndex:0]获得 Documents 路径
            NSString* imagesDirectory = [[patchs objectAtIndex:0] stringByAppendingString:@"/"];
            
#if defined(_UNICODE) || defined(UNICODE)
            dir = [imagesDirectory cStringUsingEncoding:NSUTF8StringEncoding];
#else
            dir = [imagesDirectory cStringUsingEncoding:NSASCIIStringEncoding];
#endif // defined(_UNICODE) || defined(UNICODE)
        } // if
        
        return dir;
    }*/
        
    //---------------------------------------
    const String& PlatformIOS::GetDirBundle()
    {
        static String dir;
        if ( dir.empty() ) {
        
#if defined(_UNICODE) || defined(UNICODE)
            dir = [[[NSBundle mainBundle] bundlePath] cStringUsingEncoding:NSUTF8StringEncoding];
#else
            dir = [[[NSBundle mainBundle] bundlePath] cStringUsingEncoding:NSASCIIStringEncoding];
#endif // defined(_UNICODE) || defined(UNICODE)
            
            u32 pos = dir.find_last_of(_T('/'));
            dir.erase(pos, dir.size()-pos);
            dir += _T('/');
            
        } // if
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirBundleApp()
    {
        static String dir;
        if ( dir.empty() ) {
            
#if defined(_UNICODE) || defined(UNICODE)
            dir = [[[NSBundle mainBundle] bundlePath] cStringUsingEncoding:NSUTF8StringEncoding];
#else
            dir = [[[NSBundle mainBundle] bundlePath] cStringUsingEncoding:NSASCIIStringEncoding];
#endif // defined(_UNICODE) || defined(UNICODE)

            FileUtils::PathFormat(&dir);
        } // if
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirBundleDocuments()
    {
        static String dir;
        if (dir.empty()) {
            //dir = GetDirBundle() + _T("Documents/");
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            dir = ConvertToString([paths objectAtIndex:0]);
            
            FileUtils::PathFormat(&dir);
            //[paths release];
        }
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirBundleLibrary()
    {
        static String dir;
        if (dir.empty()) {
            dir = GetDirBundle() + _T("Library/");
        }
        return dir;
    }
    
    //---------------------------------------
    const String& PlatformIOS::GetDirBundleTmp()
    {
        static String dir;
        if (dir.empty()) {
            dir = ConvertToString(NSTemporaryDirectory());
            
            FileUtils::PathFormat(&dir);
        }
        return dir;
    }

#pragma mark 其他
    
    //---------------------------------------
    String PlatformIOS::GetDeviceInfo( const DeviceInfo& info )
    {
        switch (info)
        {
            case DeviceInfo_Name:
                return PlatformIOS::ConvertToString([[UIDevice currentDevice] name]);
                break;
            case DeviceInfo_SystemVersion:
                return PlatformIOS::ConvertToString([[UIDevice currentDevice] systemVersion]);
                break;
            case DeviceInfo_LocalizedModel:
                return PlatformIOS::ConvertToString([[UIDevice currentDevice] localizedModel]);
                break;
            case DeviceInfo_SystemName:
                return PlatformIOS::ConvertToString([[UIDevice currentDevice] systemName]);
                break;
            case DeviceInfo_Model:
                return PlatformIOS::ConvertToString([[UIDevice currentDevice] model]);
                break;
                
            default:
                return String(_T(""));
                break;
        }
    }
        
    //----------------------------------------
    UIApplication* PlatformIOS::AppSingleton()
    {
        return [UIApplication sharedApplication];        
    }
    
    //----------------------------------------
    void PlatformIOS::AppIgnoreTouch( bool ignore )
    {
        UIApplication* pApp = AppSingleton();
        BOOL r = pApp.isIgnoringInteractionEvents;
        if (
            (YES == r && !ignore) ||
            (NO == r && ignore)
            )
        {
            if (r)
            {
                [pApp beginIgnoringInteractionEvents];
            }
            else
            {
                [pApp endIgnoringInteractionEvents];                
            }
        }
    }
    
    //-----------------------------------------------
    void PlatformIOS::AppIconBadgeNumber(u32 number)
    {
        AppSingleton().applicationIconBadgeNumber = number;
    }
    
    //-----------------------------------------------
    void PlatformIOS::AppIdleTimer(bool enable)
    {
        AppSingleton().idleTimerDisabled = enable ? NO : YES;
    }
    
    //-----------------------------------------------
    void PlatformIOS::AppOpenUrl(const String &url)
    {
        NSString* nsurl = ConvertToNSString(url.c_str());
        [ AppSingleton() openURL: [NSURL URLWithString:nsurl] ];
    }
    
    //-----------------------------------------------
    UIApplicationState PlatformIOS::AppState()
    {
        return AppSingleton().applicationState;
    }
    
    //----------------------------------------
    void PlatformIOS::AppFullScreen( bool bIsFull )
    {
        [AppSingleton() setStatusBarHidden:bIsFull];
    }
    
    //----------------------------------------
    bool PlatformIOS::AppIsFullScreen()
    {
        return AppSingleton().isStatusBarHidden;
    }

    //----------------------------------------
    void PlatformIOS::ViewIgnoreTouch( UIView* pView, bool ignore )
    {
        if (pView != nil)
        {
            pView.userInteractionEnabled = ignore ? YES : NO; 
        }
    }
    
    //----------------------------------------
    void PlatformIOS::ViewEnableMultiTouch( UIView* pView, bool enable )
    {
        if (pView != nil)
        {
            pView.multipleTouchEnabled = enable ? YES : NO; 
        }        
    }
    
    //----------------------------------------
    void PlatformIOS::ViewExclusiveTouch( UIView* pView, bool exclusive )
    {
        if (pView != nil)
        {
            pView.exclusiveTouch = exclusive ? YES : NO; 
        }
    }
    
    //-----------------------------------------------
    void PlatformIOS::ViewAlert(NSString *title, NSString* message, NSString *button)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:button
                                              otherButtonTitles:nil];
        
        [alert show];
        //[alert release];
    }
    
    //----------------------------------------
    CGPoint& PlatformIOS::CoordinatesView2View( UIView* pFrom, UIView* pTo, CGPoint& ptFrom, OUT CGPoint& ptTo )
    {
        if (pFrom != nil)
        {
            ptTo = [pFrom convertPoint:ptFrom toView:pTo];
        }
        else if (pTo != nil)
        {
            ptTo = [pTo convertPoint:ptFrom fromView:pFrom];
        }
        return ptTo;
    }
    
    //-----------------------------------------------
    PlatformIOS::DeviceType PlatformIOS::DeviceVerType()
    {
        static DeviceType g_type;
        static bool checked = false;
        if (checked)
            return g_type;
        
        checked = true;
        
        struct utsname systemInfo;
        uname(&systemInfo);
        NSString *platform = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
        
        if ([platform isEqualToString:@"iPhone1,1"]) {
            //platform = @"iPhone";
            g_type = DeviceType_iphone1;
        } else if ([platform isEqualToString:@"iPhone1,2"]) {
            //platform = @"iPhone 3G";
            g_type = DeviceType_iphone3g;
        } else if ([platform isEqualToString:@"iPhone2,1"]) {
            //platform = @"iPhone 3GS";
            g_type = DeviceType_iphone3gs;
        } else if ([platform isEqualToString:@"iPhone3,1"]||[platform isEqualToString:@"iPhone3,2"]||[platform isEqualToString:@"iPhone3,3"]) {
            //platform = @"iPhone 4";
            g_type = DeviceType_iphone4;
        } else if ([platform isEqualToString:@"iPhone4,1"]) {
            //platform = @"iPhone 4S";
            g_type = DeviceType_iphone4s;
        } else if ([platform isEqualToString:@"iPhone5,1"]||[platform isEqualToString:@"iPhone5,2"]) {
            //platform = @"iPhone 5";
            g_type = DeviceType_iphone5;
        }else if ([platform isEqualToString:@"iPhone5,3"]||[platform isEqualToString:@"iPhone5,4"]) {
            //platform = @"iPhone 5C";
            g_type = DeviceType_iphone5c;
        }else if ([platform isEqualToString:@"iPhone6,2"]||[platform isEqualToString:@"iPhone6,1"]) {
            //platform = @"iPhone 5S";
            g_type = DeviceType_iphone5s;
        }else if ([platform isEqualToString:@"iPhone7,1"]) {
            //platform = @"iPhone 6p";
            g_type = DeviceType_iphone6p;
        }else if ([platform isEqualToString:@"iPhone7,2"]) {
            //platform = @"iPhone 6";
            g_type = DeviceType_iphone6;
        }else if ([platform hasPrefix:@"iPhone"]) {
            g_type = DeviceType_iphone_new;
        }
        
        
        
        else if ([platform isEqualToString:@"iPod5,1"]) {
            //platform = @"iPod touch 5";
            g_type = DeviceType_ipod5;
        }else if ([platform isEqualToString:@"iPod4,1"]) {
            //platform = @"iPod touch 4";
            g_type = DeviceType_ipod4;
        }else if ([platform isEqualToString:@"iPod3,1"]) {
            //platform = @"iPod touch 3";
            g_type = DeviceType_ipod3;
        }else if ([platform isEqualToString:@"iPod2,1"]) {
            //platform = @"iPod touch 2";
            g_type = DeviceType_ipod2;
        }else if ([platform isEqualToString:@"iPod1,1"]) {
            //platform = @"iPod touch";
            g_type = DeviceType_ipod1;
        }else if ([platform hasPrefix:@"iPod"]) {
            g_type = DeviceType_ipod_new;
        }
        
        
        
        else if ([platform isEqualToString:@"iPad5,3"]||[platform isEqualToString:@"iPad5,4"]) {
            g_type = DeviceType_ipad_air2;
        } else if ([platform isEqualToString:@"iPad4,7"]||[platform isEqualToString:@"iPad4,8"]||[platform isEqualToString:@"iPad4,9"]) {
            g_type = DeviceType_ipad_mini3;
        } else if ([platform isEqualToString:@"iPad4,4"]||[platform isEqualToString:@"iPad4,5"]||[platform isEqualToString:@"iPad4,6"]) {
            g_type = DeviceType_ipad_mini2;
        } else if ([platform isEqualToString:@"iPad4,1"]||[platform isEqualToString:@"iPad4,2"]||[platform isEqualToString:@"iPad4,3"]) {
            g_type = DeviceType_ipad_air;
        } else if ([platform isEqualToString:@"iPad3,4"]||[platform isEqualToString:@"iPad3,5"]||[platform isEqualToString:@"iPad3,6"]) {
            g_type = DeviceType_ipad4;
        } else if ([platform isEqualToString:@"iPad3,3"]||[platform isEqualToString:@"iPad3,2"]||[platform isEqualToString:@"iPad3,1"]) {
            g_type = DeviceType_ipad3;
        }else if ([platform isEqualToString:@"iPad2,5"]||[platform isEqualToString:@"iPad2,6"]||[platform isEqualToString:@"iPad2,7"]) {
            g_type = DeviceType_ipad_mini1;
        } else if ([platform isEqualToString:@"iPad2,1"]||[platform isEqualToString:@"iPad2,2"]||[platform isEqualToString:@"iPad2,3"]||[platform isEqualToString:@"iPad2,4"]) {
            g_type = DeviceType_ipad2;
        }else if ([platform isEqualToString:@"iPad1,1"]||[platform isEqualToString:@"iPad1,2"]) {
            g_type = DeviceType_ipad1;
        }else if ([platform hasPrefix:@"iPad"]) {
            g_type = DeviceType_ipad_new;
        }
        
        
        else {
            g_type = DeviceType_other;
            if ([UIScreen mainScreen].bounds.size.height == 480
                && [UIScreen mainScreen].bounds.size.width == 320)
                g_type = DeviceType_iphone4s;
            else if ([UIScreen mainScreen].bounds.size.height == 568
                     && [UIScreen mainScreen].bounds.size.width == 320)
                g_type = DeviceType_iphone5s;
            else if ([UIScreen mainScreen].bounds.size.height == 667
                && [UIScreen mainScreen].bounds.size.width == 375)
                g_type = DeviceType_iphone6;
            else if ([UIScreen mainScreen].bounds.size.height == 736
                && [UIScreen mainScreen].bounds.size.width == 414)
                g_type = DeviceType_iphone6p;
        }
//        } else if ([platform isEqualToString:@"iPad3,3"]||[platform isEqualToString:@"iPad3,4"]||[platform isEqualToString:@"iPad3,5"]||[platform isEqualToString:@"iPad3,6"]) {
//            platform = @"ipad 3";
//        }
        
        return g_type;
    }
    
/**- (void)exitApplication {

    [UIView beginAnimations:@"exitApplication" context:nil];

    [UIView setAnimationDuration:0.5];

    [UIView setAnimationDelegate:self];

    // [UIView setAnimationTransition:UIViewAnimationCurveEaseOut forView:self.view.window cache:NO];

    [UIView setAnimationTransition:UIViewAnimationCurveEaseOut forView:self.window cache:NO];

    [UIView setAnimationDidStopSelector:@selector(animationFinished:finished:context:)];

    //self.view.window.bounds = CGRectMake(0, 0, 0, 0);

    self.window.bounds = CGRectMake(0, 0, 0, 0);

    [UIView commitAnimations];

}



- (void)animationFinished:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context {

    if ([animationID compare:@"exitApplication"] == 0) {

        exit(0);

    }

}*/

    
} // end namespace of hyc
