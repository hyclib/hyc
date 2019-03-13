//
//  device.h
//  hyc
//
//  Created by brainpoint on 12-1-7.
//  Copyright (c) 2012骞?hy. All rights reserved.
//

#ifndef HYC_MOBILE_DEVICE_H
#define HYC_MOBILE_DEVICE_H

using namespace hyc;

namespace hymobile {
    
    //-----------------------------------------------
    //! Device orientation
    //-----------------------------------------------
    typedef enum {
        DeviceOrientation_Unknown,
        DeviceOrientation_Portrait,            //! Device oriented vertically, home button on the bottom
        DeviceOrientation_PortraitUpsideDown,  //! Device oriented vertically, home button on the top
        DeviceOrientation_LandscapeLeft,       //! Device oriented horizontally, home button on the right
        DeviceOrientation_LandscapeRight,      //! Device oriented horizontally, home button on the left
        DeviceOrientation_FaceUp,              //! Device oriented flat, face up
        DeviceOrientation_FaceDown             //! Device oriented flat, face down
    } DeviceOrientation;
    
    //-----------------------------------------------
    //! Device Platform
    //-----------------------------------------------
    typedef enum {
        DevicePlatform_Unknown,
        DevicePlatform_iOS,
        DevicePlatform_Android,
    } DevicePlatform;

}


#ifdef HYC_COMPILE_MODULE_MOBILE
namespace hymobile {

    //----------------------------------------------------------------------------------------------------
    //!	Device Orientation Delegate
    //----------------------------------------------------------------------------------------------------
    class HYC_API DeviceOrientationDelegate
    {
    public:
        DeviceOrientationDelegate();
        virtual ~DeviceOrientationDelegate();

        virtual void onDeviceOrientationChanged( const DeviceOrientation& orientation ) = 0;
    };
    
    //----------------------------------------------------------------------------------------------------
    //!	The device object describes the device's hardware and software.
    //----------------------------------------------------------------------------------------------------
    class HYC_API Device
    {
    public:
        /**
         * @brief Initial & Finally; Called in app start and exit
         */
        static void                 Initial();
        static void                 Finally();
        
    public:
        /**
         * @brief Get Device Orientation
         */
        static DeviceOrientation    GetOrientation();
        static void                 SetOrientation( DeviceOrientation ori, bool animated = false );
        static f64                  GetOrientationChangeDelay();
        static void                 SetOrientationChangeDelay( const f64& delay );
        
        /**
         * @brief Get model
         */
        static const String&        GetModel();
        
        /**
         * @brief Get device name.
         */
        static const String&        GetName();
        
        /**
         * @brief Get Platfrom
         */
        static const DevicePlatform& GetPlatform();
        
        /**
         * @brief Get device version
         */
        static const String&        GetVersion();
    };
    
} // end namespace of hymobile


#endif
#endif // HYC_MOBILE_DEVICE_H
