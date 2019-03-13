//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  http工具
//

#ifndef __BP_HTTPUTILS_H_20101220174124__
#define __BP_HTTPUTILS_H_20101220174124__
#ifdef HYC_COMPILE_MODULE_NET


namespace hyc
{
    class HttpResponse;

	//----------------------------------------------------------------------------------------------------
	//*	HttpUtils
	//----------------------------------------------------------------------------------------------------

    class HYC_API HttpUtils
    {
    public:

        /*---------------------------
          解析地址; 获得 协议/主机/端口/URI部分
        ---------------------------*/
        static void     AnalyzeAddr( const tchar* pAddr, OUT cString& protocol, OUT cString& host, OUT cString& port, OUT cString& URI );
    };
	 

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
#endif // end of define __BP_HTTPUTILS_H_20101220174124__