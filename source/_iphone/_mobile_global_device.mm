//
//  _mobile_global_device.mm
//  hyc
//
//  Created by lipengxiang on 5/6/14.
//  Copyright (c) 2014 hy. All rights reserved.
//

#import "../preheader.h"
#import "_mobile_global_device.h"
#ifdef HYC_COMPILE_MODULE_MOBILE


@implementation __OrientationNotification

- (id)init
{
    if (nil == [super init])
        return nil;
    
    g_oriChangeDelay = 1000;
    
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(orientationChanged:)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];
    
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIDeviceOrientationDidChangeNotification object:nil];
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
    //[super dealloc];
}

- (void)add: (DeviceOrientationDelegate*) delegate
{
    g_arrDeviceOriSync.lock();
    g_arrDeviceOriListener.push_back(delegate);
    g_arrDeviceOriSync.unlock();
}

- (void)remove: (DeviceOrientationDelegate*) delegate
{
    g_arrDeviceOriSync.lock();
    g_arrDeviceOriListener.eraseByIndex(g_arrDeviceOriListener.find(delegate));
    g_arrDeviceOriSync.unlock();
}

- (void)orientationChanged:(NSNotification *)notification
{
    // We must add a delay here, otherwise we'll swap in the new view
	// too quickly and we'll get an animation glitch
    g_arrDeviceOriSync.lock();
    
    //[self performSelector:@selector(updateLandscapeView) withObject:nil afterDelay:hymobile::Device::GetOrientationChangeDelay()/1000];
    
    for (u32 i = 0; i < g_arrDeviceOriListener.size(); ++i) {
        g_arrDeviceOriListener[i]->onDeviceOrientationChanged(hymobile::Device::GetOrientation());
    }
    
    g_arrDeviceOriSync.unlock();
}

@end


#endif
