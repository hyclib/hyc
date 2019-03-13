//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#if defined(HYC_COMPILE_MODULE_LOG) || defined(HYC_COMPLIE_SUPPORT_TRACE)

#include "../../include/io/log.h"
#include "../../include/container/array.h"


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	Log::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class Log::PrivateClass
    {
    public:
        PrivateClass() : m_msgMaxLength(500), m_level(LogLevel_Info)   { m_logBuf.reserve(m_msgMaxLength); }

        u32         m_msgMaxLength;
        LogLevel    m_level;
        String      m_logBuf;

        typedef Array<LogListener*>     ARR_LISTENER;
        ARR_LISTENER     m_arrListeners;
    };
    
    //----------------------------------------------------------------------------------------------------
    //*	Log
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    Log::Log()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    //-----------------------------------------------
    Log::~Log()
    {
        PRIVATE_CLASS_GET(p);
        for ( u32 i = 0; i < p->m_arrListeners.size(); ++i )
        {
            delete p->m_arrListeners[i];
        }
        p->m_arrListeners.clear();

        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    void Log::addListener( LogListener* listener )
    {
        PRIVATE_CLASS_GET(p);
        
        if ( listener )
        {
            p->m_arrListeners.push_back( listener );
        }
    }

    //-----------------------------------------------
    void Log::removeListener( LogListener* listener )
    {
        PRIVATE_CLASS_GET(p);

        if ( listener )
        {
            for ( u32 i = 0; i < p->m_arrListeners.size(); ++i )
            {
                if (p->m_arrListeners[i] == listener)
                {
                    p->m_arrListeners.eraseByIndex(i);
                    return;
                }
            }
        }
    }

    //-----------------------------------------------
    u32 Log::getListenerNum() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_arrListeners.size();
    }

    //-----------------------------------------------
    LogListener* Log::getListener( u32 index )
    {
        PRIVATE_CLASS_GET(p);
        return p->m_arrListeners[index];
    }

    //-----------------------------------------------
    const LogLevel& Log::getLogLevel() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_level;
    }

    //-----------------------------------------------
    void Log::setLogLevel( const LogLevel& level )
    {
        PRIVATE_CLASS_GET(p);
        p->m_level = level;
    }

    //-----------------------------------------------
    u32 Log::getMsgMaxLength() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_msgMaxLength;
    }

    //-----------------------------------------------
    void Log::setMsgMaxLength( u32 length )
    {
        PRIVATE_CLASS_GET(p);
        p->m_msgMaxLength = length;
        p->m_logBuf.reserve(p->m_msgMaxLength);
    }

    //-----------------------------------------------
    void Log::log( const tchar* message )
    {
        PRIVATE_CLASS_GET(p);

        if ( message )
        {
            for ( u32 i = 0; i < p->m_arrListeners.size(); ++i )
            {
                p->m_arrListeners[i]->onInvoke( message, this );
            }
        }
    }

    //-----------------------------------------------
    void Log::logf( const tchar* format, ... )
    {
        if ( format )
        {
            va_list arg_ptr;
            va_start(arg_ptr, format);
            vlogf(format, arg_ptr);            
            va_end(arg_ptr);
        }
    }

    //-----------------------------------------------
    void Log::vlogf( const tchar* format, va_list arg_ptr )
    {
        PRIVATE_CLASS_GET(p);

        if ( format )
        {
            p->m_logBuf.vformat( format, arg_ptr );

            for ( u32 i = 0; i < p->m_arrListeners.size(); ++i )
            {
                p->m_arrListeners[i]->onInvoke( p->m_logBuf.c_str(), this );
            }
        }
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_LOG
