//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// common libaray
// author:       lipengxiang
// description:  Process的Linux平台实现
//

#ifndef __BP_PROCESS_HPP_20101207155106__
#define __BP_PROCESS_HPP_20101207155106__

#include "../../include/utils/stringUtils.h"
#include "../../include/utils/fileUtils.h"
#include "../../include/os/process.h"
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/reboot.h>
#include <sys/wait.h>
#include <linux/reboot.h>

namespace hyc
{
	//-----------------------------------------------
	void Process::GetCurProcess( OUT ProcessInfo& info )
	{
		info.m_pid = getpid();
		info.m_name.clear();

		char name[256] = {0};
		int i = 0;
		if ((i=readlink( "/proc/self/exe", name, sizeof(name) )) <= 0)
			return;

#if defined(_UNICODE) || defined(UNICODE)
        cString str;
        StringUtils::Narrow2wide( name, i, info.m_name );
#else
		info.m_name = name;
#endif // defined(_UNICODE) || defined(UNICODE)
	}

	//-----------------------------------------------
	void Process::GetAllProcess( OUT Array<ProcessInfo>* parrInfo )
	{
        Array<ProcessInfo>& arrInfo = *parrInfo;
		arrInfo.clear();

		char name[256] = {0};

		ProcessInfo info;
		Array<String> arrDirs;
		FileUtils::DirExplorer( "/proc", NULL, &arrDirs );
		for (u32 i = 0; i < arrDirs.size(); ++i)
		{
			u32 j = 0;
			for (j = 6; j < arrDirs[i].size(); ++j)
			{
				if (!::isdigit(arrDirs[i][j]))
					break;
			}
			if (j == arrDirs[i].size() && j > 6)
			{
				ZeroMemory(name, sizeof(name));
				info.m_name = arrDirs[i] + "/exe";

				if ((readlink( info.m_name.c_str(), name, sizeof(name) )) <= 0)
					continue;

				info.m_name = name;
				info.m_pid = StringUtils::StrToS32((arrDirs[i].c_str()+6));
				arrInfo.push_back(info);
			}
		}
	}

	//------------------------------------------------
	bool Process::RunBlock( const tchar* pszExePath, const tchar* pszCmdLine/* = NULL*/ )    // 阻塞, 启动的进程结束了才返回
	{
		if ( !FileUtils::FileIsExist(pszExePath) )   return false;

		int status;
		pid_t newpid = fork();
		if (0 == newpid)
		{
			if (NULL == pszCmdLine)
			{
				return -1 != execl(pszExePath, pszExePath, NULL);
			}
			else
			{
				return -1 != execl(pszExePath, pszExePath, pszCmdLine, NULL);
			}
		}
		else
		{
			return -1 != waitpid( newpid, &status, 0 );
		}
	}

	//------------------------------------------------
	bool Process::Run( const tchar* pszExePath, const tchar* pszCmdLine/* = NULL*/, const tchar* pszRunDir/* = NULL*/ ) // 非阻塞
	{
		if ( !FileUtils::FileIsExist(pszExePath) )   return false;

		pid_t newpid = fork();
		if (0 == newpid)
		{
			if (NULL != pszRunDir)	FileUtils::DirSetCurrent(pszRunDir);
			if (NULL == pszCmdLine)
			{
				return -1 != execl(pszExePath, pszExePath, NULL);
			}
			else
			{
				return -1 != execl(pszExePath, pszExePath, pszCmdLine, NULL);
			}
		}
		else
		{
			return true;
		}
	}

	//------------------------------------------------
	bool Process::Kill( integer pid )
	{
		if ( 0 > pid )   return false;
		return 0 == kill( pid, SIGKILL );
	}

    //-----------------------------------------------
    bool Process::Shutdown()
    {
     	return 0 == reboot(LINUX_REBOOT_CMD_POWER_OFF);
    }

    //-----------------------------------------------
    bool Process::Reboot()
    {
     	return 0 == reboot(LINUX_REBOOT_CMD_RESTART);
    }

} // end namespace hyc

#endif // end of define __BP_PROCESS_HPP_20101207155106__
