//
//  _mobile_global_connection.mm
//  hyc
//
//  Created by lipengxiang on 5/6/14.
//  Copyright (c) 2014 hy. All rights reserved.
//

#import "../preheader.h"
#import "_mobile_global_connection.h"
#ifdef HYC_COMPILE_MODULE_MOBILE

@implementation __ConnectionInstance

// 启动监听.
- (id) init
{
    if (nil == [super init])
        return nil;
    
    _oldConnectionType = hymobile::Connection::GetConnectionType();
    
    // 开启网络状况的监听.
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(reachabilityChanged:)
                                                 name: kReachabilityChangedNotification
                                               object: nil];
    _hostReach = /*[*/[Reachability reachabilityForInternetConnection] /*retain]*/; // 可以以多种形式初始化.
    [_hostReach startNotifier];  //开始监听,会启动一个run loop.
    [self updateInterfaceWithReachability: _hostReach];
    
    return self;
}

- (void) dealloc
{
    [_hostReach stopNotifier];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kReachabilityChangedNotification object:nil];
    _sync.lock();
    {
        _arrDelegates.clear();
    }
    _sync.unlock();
    
    //[_hostReach release];
    //[super dealloc];
}

- (void) add:(hymobile::ConnectionDelegate *)delegate
{
    _sync.lock();
    {
        _arrDelegates.push_back(delegate);
    }
    _sync.unlock();
}

- (void) remove:(hymobile::ConnectionDelegate *)delegate
{
    _sync.lock();
    {
        _arrDelegates.eraseByIndex(_arrDelegates.find(delegate));
    }
    _sync.unlock();
}

// 连接改变.
- (void) reachabilityChanged: (NSNotification* )note
{
    Reachability* curReach = [note object];
    NSParameterAssert([curReach isKindOfClass: [Reachability class]]);
    [self updateInterfaceWithReachability: curReach];
}

//处理连接改变后的情况.
- (void) updateInterfaceWithReachability: (Reachability*) curReach
{
    //对连接改变做出响应的处理动作。
    NetworkStatus status = [curReach currentReachabilityStatus];
    if (status != 0)
        status = (NetworkStatus)(status+1);
    
    _sync.lock();
    {
        for (hyc::u32 i = 0; i < _arrDelegates.size(); i++)
        {
            _arrDelegates[i]->onConnectionTypeChange(_oldConnectionType, (hymobile::ConnectionType)status);
        }
    }
    _sync.unlock();
    
    _oldConnectionType = (hymobile::ConnectionType)status;
    
    /*if(status == kReachableViaWWAN)
     {
     printf("\n3g/2G\n");
     }
     else if(status == kReachableViaWiFi)
     {
     printf("\nwifi\n");
     }else
     {
     printf("\n无网络\n");
     }*/
}

@end

#endif