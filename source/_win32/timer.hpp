//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  计时器的win32平台实现
//

#ifndef __BP_TIMER_HPP_20101108110146__
#define __BP_TIMER_HPP_20101108110146__

namespace hyc
{
	
    //----------------------------------------------------------------------------------------------------
    //*	ITimer_p
    //----------------------------------------------------------------------------------------------------

    class Timer::PrivateClass
    {
    public:
        PrivateClass() : m_startTick(0), m_timerMask(0), m_lastCount(0) {}

    public:
        DWORD_PTR       m_timerMask; // thread affinity mask
        LARGE_INTEGER   m_frequency; // counts per second
        DWORD           m_startTick;
        mutable LARGE_INTEGER   m_startCount;
        mutable LONGLONG        m_lastCount;
    };


    //----------------------------------------------------------------------------------------------------
    //*	Timer
    //----------------------------------------------------------------------------------------------------


    //-----------------------------------------------
    Timer::Timer()
    {
        PRIVATE_CLASS_CREATE;
        start();
    }

    Timer::~Timer()
    {
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    bool Timer::start()
    {
        PRIVATE_CLASS_GET(p);

        // Get the current process core mask
        DWORD_PTR procMask;
        DWORD_PTR sysMask;

        BOOL r = ::GetProcessAffinityMask( ::GetCurrentProcess(), &procMask, &sysMask );
        if (0 == r)
        {
//             UTILSLOG(_T("Timer_windows::start"), _T("fail call ::GetProcessAffinityMask in Timer_windows::start"));
            return false;
        }

        // If procMask is 0, consider there is only one core available
        // (using 0 as procMask will cause an infinite loop below)
        if (procMask == 0)
            procMask = 1;

        // Find the lowest core that this process uses
        if (0 == p->m_timerMask)
        {
            p->m_timerMask = 1;
            while (( p->m_timerMask & procMask ) == 0)
            {
                p->m_timerMask <<= 1;
            }
        } // if

        resetCount();

        return true;
    }

    //-----------------------------------------------
    bool Timer::resetCount()
    {
        PRIVATE_CLASS_GET(p);

        HANDLE thread = ::GetCurrentThread();

        // Set affinity to the first core
        DWORD_PTR oldMask = ::SetThreadAffinityMask( thread, p->m_timerMask );
        if (0 == oldMask)
        {
//             UTILSLOG(_T("Timer_windows::resetCount"), _T("fail call ::SetThreadAffinityMask in Timer_windows::resetCount"));
            return false;
        }

        // Get the constant frequency
        BOOL r = ::QueryPerformanceFrequency( &p->m_frequency );
        if (0 == r)
        {
//             UTILSLOG(_T("Timer_windows::resetCount"), _T("fail call ::QueryPerformanceFrequency in Timer_windows::resetCount"));
            return false;
        }

        // Query the timer
        r = ::QueryPerformanceCounter( &p->m_startCount );
        if (0 == r)
        {
//             UTILSLOG(_T("Timer_windows::resetCount"), _T("fail call ::QueryPerformanceCounter in Timer_windows::resetCount"));
            return false;
        }

        p->m_startTick = ::GetTickCount();

        // Reset affinity
        oldMask = ::SetThreadAffinityMask( thread, oldMask );
        if (0 == oldMask)
        {
//             UTILSLOG(_T("Timer_windows::resetCount"), _T("fail call old ::SetThreadAffinityMask in Timer_windows::resetCount"));
            return false;
        }

        p->m_lastCount = 0;

        return true;
    }

    //-----------------------------------------------
    uinteger Timer::getMilliseconds() const
    {
        PRIVATE_CLASS_GET_CONST(p);

        //--------------------------------------------------
        // get performance count

        LARGE_INTEGER curCount;

        HANDLE thread = ::GetCurrentThread();

        // Set affinity to the first core
        DWORD oldMask = ::SetThreadAffinityMask(thread, p->m_timerMask);

        // Query the timer
        ::QueryPerformanceCounter(&curCount);

        // Reset affinity
        ::SetThreadAffinityMask(thread, oldMask);

        curCount.QuadPart = curCount.QuadPart - p->m_startCount.QuadPart;

        //--------------------------------------------------
        // count time

        // scale by 1000 for milliseconds
        unsigned long newMilli = (unsigned long) (1000 * curCount.QuadPart / p->m_frequency.QuadPart);

        // Adjustment the performance counter 
        // (surprisingly hyc, see Microsoft KB: Q274323)
        unsigned long check = ::GetTickCount() - p->m_startTick;
        signed long msecOff = (signed long)(newMilli - check);
        if (msecOff < -100 || msecOff > 100)
        {
            // We must keep the timer running forward
            LONGLONG adjust = min(msecOff * p->m_frequency.QuadPart / 1000, curCount.QuadPart - p->m_lastCount);
            p->m_startCount.QuadPart += adjust;
            curCount.QuadPart -= adjust;

            // Re-calculate milliseconds
            newMilli = (unsigned long) (1000 * curCount.QuadPart / p->m_frequency.QuadPart);
        }

        // Record last time for adjust
        p->m_lastCount = curCount.QuadPart;

        return newMilli;
    }

    //-----------------------------------------------
    uinteger Timer::getMicroseconds() const
    {
        PRIVATE_CLASS_GET_CONST(p);

        //--------------------------------------------------
        // get performance count

        LARGE_INTEGER curCount;

        HANDLE thread = ::GetCurrentThread();

        // Set affinity to the first core
        DWORD oldMask = ::SetThreadAffinityMask(thread, p->m_timerMask);

        // Query the timer
        ::QueryPerformanceCounter(&curCount);

        // Reset affinity
        ::SetThreadAffinityMask(thread, oldMask);

        curCount.QuadPart = curCount.QuadPart - p->m_startCount.QuadPart;

        //--------------------------------------------------
        // count time

        // scale by 1000000 for microseconds
        unsigned long newMicro = (unsigned long) (1000000 * curCount.QuadPart / p->m_frequency.QuadPart);

        // scale by 1000 for milliseconds
        unsigned long newMilli = newMicro / 1000;

        // Adjustment the performance counter 
        // (surprisingly hyc, see Microsoft KB: Q274323)
        unsigned long check = ::GetTickCount() - p->m_startTick;
        signed long msecOff = (signed long)(newMilli - check);
        if (msecOff < -100 || msecOff > 100)
        {
            // We must keep the timer running forward
            LONGLONG adjust = min(msecOff * p->m_frequency.QuadPart / 1000, curCount.QuadPart - p->m_lastCount);
            p->m_startCount.QuadPart += adjust;
            curCount.QuadPart -= adjust;

            // Re-calculate milliseconds
            newMicro = (unsigned long) (1000000 * curCount.QuadPart / p->m_frequency.QuadPart);
        }

        // Record last time for adjust
        p->m_lastCount = curCount.QuadPart;

        return newMicro;
    }

    //-----------------------------------------------
    uinteger Timer::getThreadAffinityMask() const
    {
        PRIVATE_CLASS_GET_CONST(p);

        return p->m_timerMask;
    }

    //-----------------------------------------------
    u64 Timer::getFrequency() const
    {
        PRIVATE_CLASS_GET_CONST(p);

        return (u64)p->m_frequency.QuadPart;
    }

} // end namespace hyc

#endif // end of define __BP_TIMER_HPP_20101108110146__