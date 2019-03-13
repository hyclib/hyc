//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  时间
//

#ifndef __BP_DATATIME_H_20101207233146__
#define __BP_DATATIME_H_20101207233146__


namespace hyc
{
    //-----------------------------------------------
    //!  定义时间类型
    typedef s64 Time;               // unit in seconds.
    typedef s64 TimeMicrosecond;    // unit in microseconds.

	//-----------------------------------------------
    //!  能够表示的时间为: [1970年1月1日0:00:00 - 3001年1月1日0:00:00]
    //!  只能精确到秒
    class HYC_API DateTime
    {
        PRIVATE_CLASS_DECLARE;
    public:
        typedef enum {
            W_Sunday    = 0,
            W_Monday    = 1,
            W_Tuesday   = 2,
            W_Wednesday = 3,
            W_Thursday  = 4,
            W_Friday    = 5,
            W_Saturday  = 6,
        } Weekday;

    public:
        DateTime();
        DateTime( u16 year, u16 month, u16 day, u16 hours, u16 min, u16 sec );
        DateTime( const DateTime& time );
        ~DateTime();

        bool        operator==( const DateTime& time ) const;
        bool        operator!=( const DateTime& time ) const;
        DateTime&   operator=( const DateTime& time );
        f64         operator-( const DateTime& time ) const;   // 相差的秒数

        /**
         * @brief 设置系统时间.
         */
        static bool SetSysTime( const DateTime* t );

        /**
         * @brief 获得系统时间.
         */
        static Time GetSysTime();

        /**
         * @brief 获得系统的微秒时间.
         */
        static TimeMicrosecond GetSysMicrosecondTime();

    public:

        //-----------------------------------------------
        //  属性操作
        //-----------------------------------------------
        Time    getTime() const;    //! 返回的是从 1970年1月1日0:00:00 过去的秒数
        u16     getYear() const;
        u16     getMonth() const;  //! [1,12]
        u16     getDay() const;
        u16     getHours() const;
        u16     getMinutes() const;
        u16     getSeconds() const;
        Weekday getWeekday() const;

        void    setTime( Time time );
        void    setYear( u16 year );    //! [1970,+]
        void    setMonth( u16 month );  //! [1,12]
        void    setDay( u16 day );      //! [1,31]
        void    setHours( u16 hours );  //! [0,23]
        void    setMinutes( u16 minutes );  //! [0,59]
        void    setSeconds( u16 seconds );  //! [0,59]

        void    addYear( s16 year );
        void    addMonth( s16 month );
        void    addDay( s16 day );
        void    addHours( integer hours );
        void    addMinutes( integer minutes );
        void    addSeconds( integer seconds );

        void    setToSysTime();
        void    updateTime();   // 调用 setxxxx 后调用 (除setTime外)
    };

} // end namespace hyc

#endif // end of define __BP_DATATIME_H_20101207233146__