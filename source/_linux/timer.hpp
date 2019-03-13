//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  计时器的win32平台实现
//

#ifndef __BP_TIMER_HPP_20110908110146__
#define __BP_TIMER_HPP_20110908110146__

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

} // end namespace hyc

#endif // end of define __BP_TIMER_HPP_20110908110146__
