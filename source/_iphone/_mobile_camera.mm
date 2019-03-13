//
//  _mobile_camera.mm
//  hyc
//
//  Created by brainpoint on 14-2-22.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#import "../preheader.h"
using namespace hyc;
#ifdef HYC_COMPILE_MODULE_MOBILE

#import <AVFoundation/AVFoundation.h>
#import "../../include/_mobile/camera.h"

namespace hymobile {
    
    //-----------------------------------------------
    bool Camera::IsSupport()
    {
        return true;
    }
    
    //-----------------------------------------------
    bool Camera::IsSupportTorch()
    {
        AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        return device.hasTorch == TRUE;
    }
    
    //-----------------------------------------------
    void Camera::TurnOnTorch()
    {
        AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        if ([device hasTorch]) {
            [device lockForConfiguration:nil];
            [device setTorchMode: AVCaptureTorchModeOn];
            [device unlockForConfiguration];
        }
    }
    
    //-----------------------------------------------
    void Camera::TurnOffTorch()
    {
        AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        if ([device hasTorch]) {
            [device lockForConfiguration:nil];
            [device setTorchMode: AVCaptureTorchModeOff];
            [device unlockForConfiguration];
        }
    }
    
} // end namespace hymobile

#endif