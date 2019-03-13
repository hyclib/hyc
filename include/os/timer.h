//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  计时器
//

#ifdef HYC_COMPILE_MODULE_TIMER

#ifndef __BP_TIMER_H_20101108102550__
#define __BP_TIMER_H_20101108102550__

namespace hyc
{
	//-----------------------------------------------
    //! 计时器
    class HYC_API Timer : public INoncopyable, public RefCount
    {
        PRIVATE_CLASS_DECLARE;
    public:
        Timer();
		~Timer();
        
        /**
         * @brief start the timer in current thread and set count to 0; And set the mask of timer, frequency of timer.
         *      call this method in thread which is only validate place for calling getMilliseconds/getMicroseconds
         */
        bool    start();

        /**
         * @brief resetCount the the count to zero
         */
        bool    resetCount();

        /**
         * @brief returns milliseconds between start first time and pause
         *      when stop or not start it will return 0
         */
        uinteger    getMilliseconds() const;

        /**
         * @brief returns microseconds between start first time and pause
         *      when stop or not start it will return 0
         */
        uinteger    getMicroseconds() const;

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        /**
         * @brief get thread affinity mask;
         *      use in multiprocessor computer; mark the timer in which core
         */
        uinteger    getThreadAffinityMask() const;

        /**
         * @brief get Performance Frequency; In counts per second
         */
        u64     getFrequency() const;
#endif  
    };
    typedef ObjectPtr<Timer>    TimerPtr;

} // end namespace hyc

#endif // end of define __BP_TIMER_H_20101108102550__
#endif // HYC_COMPILE_MODULE_TIMER

