//
//  _mobile_global_motion.h
//  hyc
//
//  Created by brainpoint on 14-3-23.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#ifndef hyc__mobile_global_motion_h
#define hyc__mobile_global_motion_h
#ifdef HYC_COMPILE_MODULE_MOBILE

#import <CoreMotion/CoreMotion.h>
#import "../../include/_mobile/motion.h"
#import "../../include/_platform/iOS.h"

namespace hymobile
{
    //-----------------------------------------------
    class MotionInstance {
    public:
        MotionInstance() {
            motionMgr   = nil;
            accQueue    = nil;
            accOpen     = false;
            rotQueue    = nil;
            rotOpen     = false;
        }
        ~MotionInstance() {
            //[motionMgr release];
            //[accQueue release];
            //[rotQueue release];
            
            accQueueSync.lock();
            accQueueData.clear();
            accQueueSync.unlock();
            
            rotQueueSync.lock();
            rotQueueData.clear();
            rotQueueSync.unlock();
        }
        
        void enable() {
            if (nil == motionMgr)
            {
                motionMgr   = [[CMMotionManager alloc] init];
                accQueue    = [[NSOperationQueue alloc] init];
                rotQueue    = [[NSOperationQueue alloc] init];
            }
        }
        
        bool isEnable() const { return nil != motionMgr; }
        
        //-----------------------------------------------
        void addRotDelegate( MotionRotationDelegate* pDelegate ) {
            rotQueueSync.lock();
            if (rotQueueData.find(pDelegate) < 0) rotQueueData.push_back(pDelegate);
            openRot();
            rotQueueSync.unlock();
        }
        void removeRotDelegate( MotionRotationDelegate* pDelegate ) {
            rotQueueSync.lock();
            rotQueueData.eraseByIndex(rotQueueData.find(pDelegate));
            if (rotQueueData.empty())
            {
                [motionMgr stopGyroUpdates];
                rotOpen = false;
            }
            rotQueueSync.unlock();
        }
        
        void openRot() {
            if (rotOpen)    return;
            [motionMgr
             startGyroUpdatesToQueue:rotQueue
             withHandler:^(CMGyroData *gyroData, NSError *error) {
                 if (error)
                 {
                     [motionMgr stopGyroUpdates];
                     rotOpen = false;
                     String errMsg = hyc::PlatformIOS::ConvertToString(error.domain);
                     
                     rotQueueSync.lock();
                     for (u32 i = 0; i < rotQueueData.size(); i++) {
                         rotQueueData[i]->onError(errMsg, error.code);
                     }
                     rotQueueSync.unlock();
                 }
                 else
                 {
                     rotQueueSync.lock();
                     for (u32 i = 0; i < rotQueueData.size(); i++) {
                         rotQueueData[i]->onUpdate(
                                                   gyroData.rotationRate.x,
                                                   gyroData.rotationRate.y,
                                                   gyroData.rotationRate.z,
                                                   gyroData.timestamp);
                     }
                     rotQueueSync.unlock();
                 }
             }];
        }
        
        //-----------------------------------------------
        void addAccDelegate( MotionAccelerometerDelegate* pDelegate ) {
            accQueueSync.lock();
            if (accQueueData.find(pDelegate) < 0) accQueueData.push_back(pDelegate);
            openAccelerometer();
            accQueueSync.unlock();
        }
        void removeAccDelegate( MotionAccelerometerDelegate* pDelegate ) {
            accQueueSync.lock();
            accQueueData.eraseByIndex(accQueueData.find(pDelegate));
            if (accQueueData.empty())
            {
                [motionMgr stopAccelerometerUpdates];
                accOpen = false;
            }
            accQueueSync.unlock();
        }
        
        void openAccelerometer() {
            if (accOpen)    return;
            [motionMgr
             startAccelerometerUpdatesToQueue:accQueue
             withHandler:^(CMAccelerometerData *accelerometerData, NSError *error) {
                 
                 if (error)
                 {
                     [motionMgr stopAccelerometerUpdates];
                     accOpen = false;
                     String errMsg = PlatformIOS::ConvertToString(error.domain);
                     
                     accQueueSync.lock();
                     for (u32 i = 0; i < accQueueData.size(); i++) {
                         accQueueData[i]->onError(errMsg, error.code);
                     }
                     accQueueSync.unlock();
                 }
                 else
                 {
                     accQueueSync.lock();
                     for (u32 i = 0; i < accQueueData.size(); i++) {
                         accQueueData[i]->onUpdate(
                                                   accelerometerData.acceleration.x,
                                                   accelerometerData.acceleration.y,
                                                   accelerometerData.acceleration.z,
                                                   accelerometerData.timestamp);
                     }
                     accQueueSync.unlock();
                 }
             }];
        }
        
        CMMotionManager*    motionMgr;
        
        bool                                    accOpen;
        NSOperationQueue*                       accQueue;
        ArrayFind<MotionAccelerometerDelegate*> accQueueData;
        SyncObject                              accQueueSync;
        
        bool                                    rotOpen;
        NSOperationQueue*                       rotQueue;
        ArrayFind<MotionRotationDelegate*>      rotQueueData;
        SyncObject                              rotQueueSync;
    };
}

#endif
#endif

