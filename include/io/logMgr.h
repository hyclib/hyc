//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  日志管理者
//

#if defined(HYC_COMPILE_MODULE_LOG) || defined(HYC_COMPLIE_SUPPORT_TRACE)

#ifndef __BP_LOGMGR_H_2010121495407__
#define __BP_LOGMGR_H_2010121495407__

namespace hyc
{
    class Log;

    //----------------------------------------------------------------------------------------------------
    //*	LogMgr
    //----------------------------------------------------------------------------------------------------
     
    class HYC_API LogMgr : public Singleton<LogMgr>
    {
        PRIVATE_CLASS_DECLARE;
    public:
        LogMgr();
        ~LogMgr();

        // 添加默认的文件日志
        void    addFileLog( const LogLevel& level, const tchar* logFilePath );

    public:
        void    addLog( Log* pLog );  // 将管理 pLog的生命期
        void    removeLog( Log* pLog );  // 将自动释放 pLog

        // 无论什么level, DEBUG下 debug log 才记录
        void    log( const LogLevel& level, const tchar* message );
    };


} // end namespace hyc

#endif // end of define __BP_LOGMGR_H_2010121495407__

#endif // HYC_COMPILE_MODULE_LOG
