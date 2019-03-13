/*
 *  Camera.h
 *  hyc
 *
 *  Created by lipengxiang on 11-10-21.
 *  Copyright 2011 hy. All rights reserved.
 *
 */

#ifndef HYC_MOBILE_CAMERA_H
#define HYC_MOBILE_CAMERA_H
#ifdef HYC_COMPILE_MODULE_MOBILE

namespace hymobile
{
    /**
     * The camera object provides access to the device's default camera application.
     */
    class HYC_API Camera
    {
    public:
        /**
         * @brief Judgment the device if support the camera
         */
        static bool IsSupport();
        
        /**
         * @brief Judgment the device if support the camera torch.
         */
        static bool IsSupportTorch();
        
        /**
         * @brief Open/close the torch.
         */
        static void TurnOnTorch();
        static void TurnOffTorch();
    };
    
} // end namespace of hymobile

#endif
#endif // HYC_MOBILE_CAMERA_H
