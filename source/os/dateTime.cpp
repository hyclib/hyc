//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#include <time.h>
#include "../../include/os/dateTime.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#include "../_win32/datetime.hpp"

#else 
#include "../_linux/datetime.hpp"

#endif


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	DateTime::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class DateTime::PrivateClass
    {
    public:
        PrivateClass() : m_tmValue(0)
        {
            memset( &m_tmInfo, 0, sizeof(tm) );
        }

        time_t      m_tmValue;
        struct tm   m_tmInfo;
    };
    
    //----------------------------------------------------------------------------------------------------
    //*	DateTime
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    DateTime::DateTime()
    {
        PRIVATE_CLASS_CREATE;
    }

    //-----------------------------------------------
    DateTime::DateTime( u16 year, u16 month, u16 day, u16 hours, u16 min, u16 sec )
    {
        PRIVATE_CLASS_CREATE;
        PRIVATE_CLASS_GET(p);

        // set time
        p->m_tmInfo.tm_year = year-1900;
        p->m_tmInfo.tm_mon  = month-1;
        p->m_tmInfo.tm_mday = day;
        p->m_tmInfo.tm_hour = hours;
        p->m_tmInfo.tm_min  = min;
        p->m_tmInfo.tm_sec  = sec;

        // update
        updateTime();
    }

    //-----------------------------------------------
    DateTime::DateTime( const DateTime& time )
    {
        PRIVATE_CLASS_CREATE;
        *this = time;
    }

    //-----------------------------------------------
    DateTime::~DateTime()
    {
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    void DateTime::updateTime()
    {
        PRIVATE_CLASS_GET(p);
        p->m_tmValue = mktime( &p->m_tmInfo );
    }

    //-----------------------------------------------
    bool DateTime::operator==( const DateTime& time ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        PRIVATE_CLASS_GET_OTHER_CONST(&time, pp);
        return p->m_tmValue == pp->m_tmValue;
    }

    //-----------------------------------------------
    bool DateTime::operator!=( const DateTime& time ) const
    {
        return !(*this == time);
    }

    //-----------------------------------------------
    DateTime& DateTime::operator=( const DateTime& time )
    {
        PRIVATE_CLASS_GET(p);
        PRIVATE_CLASS_GET_OTHER_CONST(&time, pp);
        p->m_tmInfo = pp->m_tmInfo;
        p->m_tmValue = pp->m_tmValue;
        return *this;
    }

    //-----------------------------------------------
    f64 DateTime::operator-( const DateTime& time ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        PRIVATE_CLASS_GET_OTHER_CONST(&time, pp);
        return difftime( p->m_tmValue, pp->m_tmValue ); 
    }

    //-----------------------------------------------
    Time DateTime::getTime() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_tmValue;
    }

    //-----------------------------------------------
    u16 DateTime::getHours() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_tmInfo.tm_hour;
    }

    //-----------------------------------------------
    u16 DateTime::getMinutes() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_tmInfo.tm_min;
    }

    //-----------------------------------------------
    u16 DateTime::getSeconds() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_tmInfo.tm_sec;
    }

    //-----------------------------------------------
    u16 DateTime::getYear() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_tmInfo.tm_year + 1900;
    }

    //-----------------------------------------------
    u16 DateTime::getMonth() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_tmInfo.tm_mon+1;
    }

    //-----------------------------------------------
    u16 DateTime::getDay() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_tmInfo.tm_mday;
    }

    //-----------------------------------------------
    DateTime::Weekday DateTime::getWeekday() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return (DateTime::Weekday)p->m_tmInfo.tm_wday;
    }

    //-----------------------------------------------
    void DateTime::setTime( Time time )
    {
        PRIVATE_CLASS_GET(p);
        p->m_tmValue = (integer)time;
        
#if CN_PLATFORM == CN_PLATFORM_IOS || CN_PLATFORM == CN_PLATFORM_ANDROID || CN_PLATFORM == CN_PLATFORM_LINUX || CN_PLATFORM == CN_PLATFORM_MACOSX
        localtime_r( &p->m_tmValue, &p->m_tmInfo );
#else
   	    localtime_s( &p->m_tmInfo, &p->m_tmValue );     
#endif // CN_PLATFORM == CN_PLATFORM_IPHONE
    }

    //-----------------------------------------------
    void DateTime::setHours( u16 hours )
    {
        assert( hours >= 0 && hours < 24 );
        PRIVATE_CLASS_GET(p);
        p->m_tmInfo.tm_hour = hours;
    }

    //-----------------------------------------------
    void DateTime::setMinutes( u16 minutes )
    {
        assert( minutes >= 0 && minutes < 60 );
        PRIVATE_CLASS_GET(p);
        p->m_tmInfo.tm_min = minutes;
    }

    //-----------------------------------------------
    void DateTime::setSeconds( u16 seconds )
    {
        assert( seconds >= 0 && seconds < 60 );
        PRIVATE_CLASS_GET(p);
        p->m_tmInfo.tm_sec = seconds;
    }

    //-----------------------------------------------
    void DateTime::setYear( u16 year )
    {
        assert( year >= 1900 );
        PRIVATE_CLASS_GET(p);
        p->m_tmInfo.tm_year = year-1900;
    }

    //-----------------------------------------------
    void DateTime::setMonth( u16 month )
    {
        assert( month >= 1 && month < 13 );
        PRIVATE_CLASS_GET(p);
        p->m_tmInfo.tm_mon = month-1;
    }

    //-----------------------------------------------
    void DateTime::setDay( u16 day )
    {
        assert( day >= 1 && day <= 31 );
        PRIVATE_CLASS_GET(p);
        p->m_tmInfo.tm_mday = day;
    }

    //-----------------------------------------------
    void DateTime::setToSysTime()
    {
        PRIVATE_CLASS_GET(p);
        p->m_tmValue = time(NULL);
        setTime( p->m_tmValue );
    }

    //-----------------------------------------------
    void DateTime::addYear( s16 year )
    {
        setYear( getYear()+year );
        updateTime();
    }

    //-----------------------------------------------
    void DateTime::addMonth( s16 month )
    {
        s16 year = month / 12;
        s16 mon = month % 12;
        u16 monNow = getMonth() + mon;
        
        if ( monNow > 12 )
        {
            ++year;
            monNow -= 12;
        }

        setYear( getYear()+year );
        setMonth( monNow );
        updateTime();
    }

    //-----------------------------------------------
    void DateTime::addDay( s16 day )
    {
        s64 time = getTime();
        time += day * 24 * 60 * 60;
        setTime( time );
    }

    //-----------------------------------------------
    void DateTime::addHours( integer hours )
    {
        s64 time = getTime();
        time += hours * 60 * 60;
        setTime( time );
    }

    //-----------------------------------------------
    void DateTime::addMinutes( integer minutes )
    {
        s64 time = getTime();
        time += minutes * 60;
        setTime( time );
    }

    //-----------------------------------------------
    void DateTime::addSeconds( integer seconds )
    {
        s64 time = getTime();
        time += seconds;
        setTime( time );
    }


//-----------------------------------------------
#if CN_PLATFORM == CN_PLATFORM_WINDOWS

    //-----------------------------------------------
    bool DateTime::SetSysTime( const DateTime* t )
    {
        if (t)
        {
            SYSTEMTIME st;
            st.wDay         = t->getDay();
            st.wDayOfWeek   = -1;
            st.wHour        = t->getHours();
            st.wMilliseconds= 0;
            st.wMinute      = t->getMinutes();
            st.wMonth       = t->getMonth();
            st.wSecond      = t->getSeconds();
            st.wYear        = t->getYear();
            return TRUE == ::SetLocalTime( &st );
        }
        return false;
    }

#else
    //-----------------------------------------------
    bool DateTime::SetSysTime( const DateTime* t )
    {
        if (t)
        {
            struct timeval tv;
            tv.tv_sec = (integer)t->getTime();
            tv.tv_usec = 0;

            return settimeofday(&tv, NULL);
        }
        return false;
    }

#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS


    //-----------------------------------------------
    hyc::Time DateTime::GetSysTime()
    {
        return time(NULL);
    }


} // end namespace hyc
