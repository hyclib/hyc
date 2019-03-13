/*
 *  Timer.mm
 *  common
 *
 *  Created by pengxiang li on 2/28/11.
 *  Copyright 2011 hy. All rights reserved.
 *
 */

#include "../preheader.h"
#ifdef HYC_COMPILE_MODULE_TIMER

#include "../../include/os/timer.h"
#include <sys/time.h>


namespace hyc
{
	
    //----------------------------------------------------------------------------------------------------
    //*	ITimer_p
    //----------------------------------------------------------------------------------------------------
    
    class Timer::PrivateClass
    {        
    public:
        mutable struct timeval  m_start;
        mutable struct timeval  m_end;
    };
    
    
    //----------------------------------------------------------------------------------------------------
    //*	Timer
    //----------------------------------------------------------------------------------------------------
    
    
    //-----------------------------------------------
    Timer::Timer()
    {
        PRIVATE_CLASS_CREATE;
        resetCount();        
    }
    
    Timer::~Timer()
    {
        PRIVATE_CLASS_DESTROY;
    }
    
    //-----------------------------------------------
    bool Timer::start()
    {
        return resetCount();
    }
    
    //-----------------------------------------------
    bool Timer::resetCount()
    {
        PRIVATE_CLASS_GET(p);
        
        //p->m_zeroClock = clock();
        return 0 == gettimeofday(&p->m_start, NULL);
    }
    
    //-----------------------------------------------
    uinteger Timer::getMilliseconds() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        
        if ( 0 != gettimeofday(&p->m_end, NULL) )
            return 0;
        
        return (p->m_end.tv_sec-p->m_start.tv_sec) * 1000 + (p->m_end.tv_usec-p->m_start.tv_usec)/1000;
    }
    
    //-----------------------------------------------
    uinteger Timer::getMicroseconds() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        
        if ( 0 != gettimeofday(&p->m_end, NULL) )
            return 0;
        
        return (p->m_end.tv_sec-p->m_start.tv_sec) * 1000000 + (p->m_end.tv_usec-p->m_start.tv_usec);
    }
    
    /*/-----------------------------------------------
    u32 Timer::getThreadAffinityMask() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        
        return p->m_timerMask;
    }
    
    //-----------------------------------------------
    u64 Timer::getFrequency() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        
        return (u64)p->m_frequency.QuadPart;
    }*/
    
} // end namespace hyc

#endif // HYC_COMPILE_MODULE_TIMER
