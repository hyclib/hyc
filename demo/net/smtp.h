//
// Copyright(c) 1986-2100, TT, All Rights Reserved
// ����:  htz
// ����: 2014/5/6 17:43:42_
// ����:  
//

#ifndef _STMP_20145617433342_
#define _STMP_20145617433342_

void net_smtp()
{
	Smtp smtp;
	smtp.setHostInfo("smtp.163.com", 25);
	smtp.setUserInfo("hyMailAutoSender@163.com", "hy123456789");
	smtp.setFrom("hyMailAutoSender@163.com");
	smtp.setTo("276962675@qq.com");
	s32 r = smtp.send(_T("content"), _T("title"));
	
	if(StmpResult_ok != r)
	{
        print_all_line(_T("send error"));
	}
	else
	{
        print_all_line(_T("send ok"));
	}
}

#endif // _STMP_20145617433342_
