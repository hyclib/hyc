//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  iphone/ipad/mac platform API
//

#ifndef __BP_IOS_H_20110222181210__
#define __BP_IOS_H_20110222181210__

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "iOS/KeychainItemWrapper.h"
#import "iOS/utils_camera.h"
#import "iOS/utils_image.h"
#import "iOS/utils_time.h"
#import "iOS/utils_view.h"
#import "iOS/utils_string.h"

namespace hyc
{    
    //----------------------------------------------------------------------------------------------------
    //!	platform independent API
    //----------------------------------------------------------------------------------------------------
    class HYC_API PlatformIOS {
    public:
        //! Device information
        typedef enum {
            DeviceInfo_Name,
            DeviceInfo_LocalizedModel,
            DeviceInfo_SystemVersion,
            DeviceInfo_SystemName,
            DeviceInfo_Model,
        } DeviceInfo;
        
        //! Text encoding
        typedef enum {
            Encoding_ASCII      = NSASCIIStringEncoding,    /* 0..127 only */
            Encoding_Japanese   = NSJapaneseEUCStringEncoding,
            Encoding_UTF8       = NSUTF8StringEncoding,
            Encoding_UTF16      = NSUTF16StringEncoding,
            Encoding_UTF32      = NSUTF32StringEncoding,
            Encoding_Unicode    = NSUnicodeStringEncoding,
        } Encoding;
        
        //! Device type.
        typedef enum {
            DeviceType_other = 0,
            
            DeviceType_ipod1,
            DeviceType_ipod2,
            DeviceType_ipod3,
            DeviceType_ipod4,
            DeviceType_ipod5,
            DeviceType_ipod_new,
            
            DeviceType_iphone1 = 100,
            DeviceType_iphone3g,
            DeviceType_iphone3gs,
            DeviceType_iphone4,
            DeviceType_iphone4s,
            DeviceType_iphone5,
            DeviceType_iphone5c,
            DeviceType_iphone5s,
            DeviceType_iphone6,
            DeviceType_iphone6p,
            DeviceType_iphone_new,
            
            DeviceType_ipad1 = 200,
            DeviceType_ipad2,
            DeviceType_ipad_mini1,
            DeviceType_ipad3,
            DeviceType_ipad4,
            DeviceType_ipad_air,
            DeviceType_ipad_mini2,
            DeviceType_ipad_mini3,
            DeviceType_ipad_air2,
            DeviceType_ipad_new,
            
        } DeviceType;
        
    public:
        /*---------------------------
            String
         ---------------------------*/
        static String               ConvertToString( NSString* str, Encoding encoding = Encoding_UTF8 );
        static NSString*            ConvertToNSString( const char* pStr, Encoding encoding = Encoding_UTF8 );
        static NSString*            ConvertToNSString( const wchar_t* pStr, Encoding encoding = Encoding_UTF8 );
        
        static NSString*            GetLocalizedString( NSString* key );
        static String               GetLocalizedString( const String& key ) { return ConvertToString(GetLocalizedString(ConvertToNSString(key.c_str()))); }
        
        /*---------------------------
            Application Directory
         ---------------------------*/
        static const String&        GetDirVideo();          //! video directory
        static const String&        GetDirPicture();        //! camera picture directory
        static const String&        GetDirMusic();          //! music directory
        static const String&        GetDirEBook();          //! ebook directory
        static const String&        GetDirRingtones();      //! ringtones directory
        static const String&        GetDirRecorder();       //! Recorder directory
        static const String&        GetDirWallpaper();      //! Wallpaper directory
        static const String&        GetDirSysAudioUISounds();    //! System Sounds directory
        static const String&        GetDirSysAddressBook(); //! AddressBook directory
        static const String&        GetDirSysSMS();         //! SMS directory
        static const String&        GetDirSysFontCache();   //! System font directory
        static const String&        GetDirSysDocument();    //! System Document directory
        
        static const String&        GetDirBundle();         //! application sandbox directory
        static const String&        GetDirBundleApp();      //! application app directory (Don't modify this directory)
        static const String&        GetDirBundleDocuments();//! application documents
        static const String&        GetDirBundleLibrary();  //! application library; (The contents of this directory exception of the Caches subdirectory will be backed up by iTunes.)
        static const String&        GetDirBundleTmp();      //! application temporary files
        
        /*---------------------------
            Application Function
         ---------------------------*/
        static UIApplication*       AppSingleton();                      //! current application
        static void                 AppIgnoreTouch( bool ignore );       //! Ignore touch event
        
        /**
         * @brief 是否显示topBar, topBar 高度为20.
         *          ios7 需要添加配置 key:UIViewControllerBasedStatusBarAppearance, value:NO
         */
        static void                 AppFullScreen( bool bIsFull );
        static bool                 AppIsFullScreen();
        
        static void                 AppIconBadgeNumber( u32 number );   //! Icon badge
        static UIApplicationState   AppState();
        static void                 AppIdleTimer( bool enable );        //! 如果enable,则程序关闭自动锁屏.
        static void                 AppOpenUrl( const String& url );
        
        /*---------------------------
            View Function
         ---------------------------*/
        static void                 ViewIgnoreTouch( UIView* pView, bool ignore );      //! Ignore touch event
        static void                 ViewEnableMultiTouch( UIView* pView, bool enable ); //! Multiple touch event
        static void                 ViewExclusiveTouch( UIView* pView, bool exclusive );//! Exclusive touch event, if exclusive is true, it won't send event to next view 
        
        static void                 ViewAlert( NSString* title, NSString* message, NSString* button = @"OK" );    // Show Alert Dialog.
        
        /*---------------------------
            Coordinates Function
         ---------------------------*/
        
        /**
         * @brief Convert coordinates to new coordinate in new view
         * @param pFrom   the point in pFrom
         * @param ptTo     the point in pTo
         * @return         reference to ptTo
         */
        static CGPoint&             CoordinatesView2View( UIView* pFrom, UIView* pTo, CGPoint& ptFrom, OUT CGPoint& ptTo );
        
        /*---------------------------
            Device
         ---------------------------*/
        static String               GetDeviceInfo( const DeviceInfo& info );
        
        /**
         * @brief 判断是否为iPhone.
         */
        inline static bool          DeviceIsIPhone()  { return (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone); }
        /**
         * @brief 判断是否为iPad.
         */
        inline static bool          DeviceIsIPad()  { return (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad); }
        /**
         * @brief 判断是否为ipod.
         */
        inline static bool          DeviceIsIPod()  { return ([[[UIDevice currentDevice] model] isEqualToString:@"iPod touch"]); }
    
        /**
         * @brief 获得设备的类型.
         */
        static DeviceType           DeviceVerType();
        
        /**
         * @brief 状态栏高度.
         */
        inline static f32           HeightOfStatusBar()  { return 20; }
        
        /**
         * @brief navigationBar高度.
         */
        inline static f32           HeightOfNavigationBar( UIViewController* v ) { return v ? (v.navigationController ? v.navigationController.navigationBar.bounds.size.height : 0) : 0; }
    };

} // end namespace hyc

#endif // end of define __BP_IOS_H_20110222181210__
