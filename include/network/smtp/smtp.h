//
// Copyright(c) 1986-2100, TT, All Rights Reserved
// 作者:  htz
// 日期: 2014/5/6 17:09:14_
// 描述:  smtp简单邮件传输协议,暂不支持要求ssl的邮件服务器
//

#ifndef _SMTP_201456170914_
#define _SMTP_201456170914_

#ifdef HYC_COMPILE_MODULE_NET

namespace hyc
{
	class HYC_API Smtp: public INoncopyable, public MemoryAllocObject, public RefCountSafe
	{
		PRIVATE_CLASS_DECLARE
	public:
		Smtp();
		~Smtp();

		void	setHostInfo(const char* host, u32 port);
		void	setUserInfo(const char* name, const char* passWord);
		void	setFrom(const char* name);
		void	setTo(const char* name);

		// 返回值参考StmpResult
		s32		send(const String& msg, const String& subject);
	};
}
#endif // HYC_COMPILE_MODULE_NET
#endif // _SMTP_201456170914_