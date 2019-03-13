//
//  hycTest.h
//  hyc
//
//  Created by 鏉?楣忕繑 on 12-1-15.
//  Copyright (c) 2012骞?鍗庤€€淇℃伅绉戞妧鏈夐檺鍏徃. All rights reserved.
//
 
#ifndef hyc_hycTest_h
#define hyc_hycTest_h

#include "../include/hyc.h"
using namespace hyc;

// define assert macro
//#if CN_PLATFORM == CN_PLATFORM_IOS || CN_PLATFORM == CN_PLATFORM_MAXOS
//#   define hy_assertTrue(exp)  STAssertTrue(exp, @"failure")
//#   define hy_assertFalse(exp) STAssertFalse(exp, @"failure")
//#else
#   define hy_assertTrue(exp)  assert(exp)
#   define hy_assertFalse(exp) assert(!(exp))
//#endif

#include "../demo/common/common.h"
#include "../demo/common/objectPtr.h"

#include "../demo/os/os.h"

#include "../demo/container/arrayfind.h"
//#include "../demo/container/yunstream.h"

#include "../demo/net/smtp.h"

#include "../demo/concurrent/queue.h"

////////////////////////////
void hyc_test();
void hyc_test() 
{
    common();
    common_objectPtr();

    os();
    
    container_arrayfind();
    //container_yunstream();
    
    concurrent_queue();
}


#endif

