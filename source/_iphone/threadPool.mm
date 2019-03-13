/*
 *  ThreadPool.mm
 *  hyc
 *
 *  Created by pengxiang li on 6/1/11.
 *  Copyright 2011 hy. All rights reserved.
 *
 */

#include "../preheader.h"
#ifdef HYC_COMPILE_MODULE_THREADPOOL

#include "../../include/os/threadPool.h"
#import <Foundation/Foundation.h>

///////////////////////////////////////
// interface
@interface _ThreadPoolWorkItem : NSOperation
{
@private
    long     _pWorkItem;
}
@property (nonatomic, assign) long _pWorkItem;

@end

///////////////////////////////////////
// implementation
@implementation _ThreadPoolWorkItem

@synthesize _pWorkItem;

-(void)dealloc {
    if (0 != _pWorkItem)
    {
        hyc::ThreadPoolWorkItem* pWorkItem = (hyc::ThreadPoolWorkItem*)_pWorkItem;
        pWorkItem->release();
        //        delete pWorkItem;
        _pWorkItem = 0;
    }
    //[super dealloc];
}

-(void)main {
    hyc::ThreadPoolWorkItem* pWorkItem = (hyc::ThreadPoolWorkItem*)_pWorkItem;
    if (pWorkItem)
    {
        pWorkItem->onStart();
        
        while (true)
        {
            if (pWorkItem->isExit() || pWorkItem->onInvoke())
                break;
        }
        
        pWorkItem->onFinish();
        
        pWorkItem->getCVJoin().broadcast();
        
        pWorkItem->release();
        pWorkItem = NULL;
        [self cancel];
    }
}

@end

///////////////////////////////////////
// namespace hyc
namespace hyc {

    
    class ThreadPool::PrivateClass
    {
    public:
        PrivateClass()
        {
            _queue = [[NSOperationQueue alloc] init];
        }
        
        ~PrivateClass()
        {
            [_queue cancelAllOperations];
        }
        
        NSOperationQueue*    _queue;
    };

    ThreadPool::ThreadPool()
    {
        PRIVATE_CLASS_CREATE;        
    }
        
    ThreadPool::~ThreadPool()
    {
        PRIVATE_CLASS_DESTROY;
    }
    
    void ThreadPool::QueueWorkItem( ThreadPoolWorkItem* pWorkItem, ThreadPoolMode mode /*= ThreadPoolMode_Default*/ )
    {
        PRIVATE_CLASS_GET(p);
        
        pWorkItem->addRef();
        _ThreadPoolWorkItem* pWork = [[_ThreadPoolWorkItem alloc] init];
        pWork._pWorkItem =
                        (long)pWorkItem;
        
        [p->_queue addOperation:pWork];
        //[p->_queue waitUntilAllOperationsAreFinished];
    }
    

} // end namespace of hyc

#endif // HYC_COMPILE_MODULE_THREADPOOL
