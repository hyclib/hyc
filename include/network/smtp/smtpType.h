//
// Copyright(c) 1986-2100, TT, All Rights Reserved
// 作者:  htz
// 日期: 2014/5/6 17:25:44_
// 描述:  
//

#ifndef _SMTPTYPE_201456172544_
#define _SMTPTYPE_201456172544_

#ifdef HYC_COMPILE_MODULE_NET

namespace hyc
{
	typedef enum {
		StmpResult_ok = 0,
		StmpResult_net_error,
		StmpResult_timeout_connect,
		StmpResult_server_refuse,
		StmpResult_error_hello,
		StmpResult_username_error,
		StmpResult_passwrod_error,
		StmpResult_sender_error,
		StmpResult_target_error,
		StmpResult_content_error,
	} StmpResult;
}

#endif // HYC_COMPILE_MODULE_NET

#endif // _SMTPTYPE_201456172544_