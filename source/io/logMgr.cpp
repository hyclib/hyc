//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#if defined(HYC_COMPILE_MODULE_LOG) || defined(HYC_COMPLIE_SUPPORT_TRACE)

#include "../../include/io/logListener.h"
#include "../../include/io/log.h"
#include "../../include/io/logMgr.h"
#include "../../include/container/array.h"

// 实现单例.
IMPLEMENT_INTERFACE_SINGLETON( hyc::LogMgr );

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	LogMgr::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class LogMgr::PrivateClass
    {
    public:
        typedef Array<Log*> ARR_LOG;
        ARR_LOG     m_arrLogs;
    };
    
    //----------------------------------------------------------------------------------------------------
    //*	LogMgr
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    LogMgr::LogMgr()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    //-----------------------------------------------
    LogMgr::~LogMgr()
    {
        PRIVATE_CLASS_GET(p);
        for (u32 i = 0; i < p->m_arrLogs.size(); ++i)
        {
            delete p->m_arrLogs[i];
        }
        p->m_arrLogs.clear();

        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    void LogMgr::addLog( Log* pLog )
    {
        PRIVATE_CLASS_GET(p);
        
        if ( pLog )
        {
            p->m_arrLogs.push_back( pLog );
        }
    }

    //-----------------------------------------------
    void LogMgr::removeLog( Log* pLog )
    {
        PRIVATE_CLASS_GET(p);

        if ( pLog )
        {
            for ( u32 i = 0; i < p->m_arrLogs.size(); ++i )
            {
                if ( pLog == p->m_arrLogs[i] )
                {
                    delete p->m_arrLogs[i];
                    p->m_arrLogs.eraseByIndex( i );
                    return;
                }
            }
        } // if
    }

    //-----------------------------------------------
    void LogMgr::log( const LogLevel& level, const tchar* message )
    {
        PRIVATE_CLASS_GET(p);

        if ( !message ) return;

#ifdef _DEBUG
        for ( u32 i = 0; i < p->m_arrLogs.size(); ++i )
        {
            if ( LogLevel_Debug == p->m_arrLogs[i]->getLogLevel() )
            {
                p->m_arrLogs[i]->log( message );
            }
        } // for
#endif // _DEBUG

        for ( u32 i = 0; i < p->m_arrLogs.size(); ++i )
        {
            if ( level == p->m_arrLogs[i]->getLogLevel() ) 
            {
                p->m_arrLogs[i]->log( message );
            }
        } // for
    }

    //-----------------------------------------------
    void LogMgr::addFileLog( const LogLevel& level, const tchar* logFilePath )
    {
        Log* pLog = new Log;
        pLog->setLogLevel( level );
        pLog->addListener( new LogFileListener(logFilePath) );
        addLog( pLog );
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_LOG
