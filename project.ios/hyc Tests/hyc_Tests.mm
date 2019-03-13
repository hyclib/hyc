//
//  hyc_Tests.m
//  hyc Tests
//
//  Created by brainpoint on 14-7-16.
//  Copyright (c) 2014年 hy. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "hyc.h"
#import "../../include/_platform/iOS.h"
#import "../../test/hycTest.h"


class NetChange:public hymobile::ConnectionDelegate
{
public:
    void onConnectionTypeChange( hymobile::ConnectionType oldType, hymobile::ConnectionType newType )
    {
        NSLog(@"0");
    }
};


@interface hyc_Tests : XCTestCase

@end

@implementation hyc_Tests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample
{
    hymobile::Device::Initial();
    
    IPv6 pp = SocketUtils::AddrLocal_v6();
    
    NSLog(@"local ipv6: %d %d %d %d %d %d %d %d",
          (int)pp.__u6_addr.__u6_addr16[0],
          (int)pp.__u6_addr.__u6_addr16[1],
          (int)pp.__u6_addr.__u6_addr16[2],
          (int)pp.__u6_addr.__u6_addr16[3],
          (int)pp.__u6_addr.__u6_addr16[4],
          (int)pp.__u6_addr.__u6_addr16[5],
          (int)pp.__u6_addr.__u6_addr16[6],
          (int)pp.__u6_addr.__u6_addr16[7]);
    
    String ss = SocketUtils::AddrToStr_v6(pp);
    
    TcpTransceiver tcp;
    ConnectResult cr = tcp.connect("mango-server1.citongs.com", 3562);
    NSLog(@"ConnectResult %d %s", (int)cr, ss.c_str());
    
    JsonDoc doc;
    bool r = doc.ParseStream("{\"scene\":[{\"name\":\"一键关灯\",\"imageIndex\":5},{\"name\":\"离家\",\"imageIndex\":3}]}");
    
    new NetChange;
    //hyc_test();
    //XCTFail(@"No implementation for \"%s\"", __PRETTY_FUNCTION__);
}

@end
