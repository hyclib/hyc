//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_HTTPTYPE_H_20101220112627__
#define __BP_HTTPTYPE_H_20101220112627__
#ifdef HYC_COMPILE_MODULE_NET


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //*	HTTP supported action
    //----------------------------------------------------------------------------------------------------

    typedef enum {
        HttpMethod_Get,     // 请求获取 Request-URI 所标识的资源
        HttpMethod_Post,    // 在 Request-URI 所标识的资源后附加新的数据
        HttpMethod_Head,    // 请求获取由 Request-URI 所标识的资源的响应消息报头
        HttpMethod_Put,     // 请求服务器存储一个资源，并用 Request-URI 作为其标识
        HttpMethod_Delete,  // 请求服务器删除 Request-URI 所标识的资源
        HttpMethod_Trace,   // 请求服务器回送收到的请求信息，主要用于测试或诊断
        HttpMethod_Connect, // Reserved
        HttpMethod_Options, // 请求查询服务器的性能，或者查询与资源相关的选项和需求
    } HttpMethod;


    //----------------------------------------------------------------------------------------------------
    //*	HTTP version
    //----------------------------------------------------------------------------------------------------

    typedef enum {
        HttpVersion_1,      // HTTP/1.0
        HttpVersion_1_1,    // HTTP/1.1
        HttpVersion_NG,     // HTTP-NG
    } HttpVersion;

    //----------------------------------------------------------------------------------------------------
    //*	HTTP Result
    //----------------------------------------------------------------------------------------------------

    typedef enum {
        HttpResult_ok,              // 表示完整地结束.
        HttpResult_ok_haveContent,  // 表示有数据可以继续获得 (调用requestContent).

        HttpResult_error,
        HttpResult_error_header,
        HttpResult_error_contentTolong,
        
        HttpResult_timeout,
        HttpResult_timeout_connect,
        HttpResult_timeout_send,
        HttpResult_timeout_recv,
    } HttpResult;


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
#endif // end of define __BP_HTTPTYPE_H_20101220112627__