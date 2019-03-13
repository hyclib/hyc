//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  http响应 status-code
//

#ifndef __BP_HTTPSTATUSCODE_H_20101217143757__
#define __BP_HTTPSTATUSCODE_H_20101217143757__
#ifdef HYC_COMPILE_MODULE_NET

// 100 : Continue 
// 101 : Switching Protocols 
// 200 : OK 
// 201 : Created 
// 202 : Accepted 
// 203 : Non-Authoritative Information 
// 204 : No Content 
// 205 : Reset Content 
// 206 : Partial Content 
// 300 : Multiple Choices 
// 301 : Moved Permanently 
// 302 : Found 
// 303 : See Other 
// 304 : Not Modified 
// 305 : Use Proxy 
// 307 : Temporary Redirect 
// 400 : Bad Request 
// 401 : Unauthorized 
// 402 : Payment Required 
// 403 : Forbidden 
// 404 : Not Found 
// 405 : Method Not Allowed 
// 406 : Not Acceptable 
// 407 : Proxy Authentication Required 
// 408 : Request Time-out 409 : Conflict 
// 410 : Gone 
// 411 : Length Required 
// 412 : Precondition Failed 
// 413 : Request Entity Too Large 
// 414 : Request-URI Too Large 
// 415 : Unsupported Media Type 
// 416 : Requested range not satisfiable 
// 417 : Expectation Failed 
// 500 : Internal Server Error 
// 501 : Not Implemented 
// 502 : Bad Gateway 
// 503 : Service Unavailable 
// 504 : Gateway Time-out 
// 505 : HTTP Version not supported

namespace hyc
{
    //-----------------------------------------------
    typedef enum {

        HttpStatusCode_Error                           = 0,  // 通信错误

        /*---------------------------
          1xx   信息指示
        ---------------------------*/
        HttpStatusCode_100_Continue                    = 100,
        HttpStatusCode_101_SwitchingProtocols          = 101,

        /*---------------------------
          2xx   成功
        ---------------------------*/
        HttpStatusCode_200_Ok                          = 200,
        HttpStatusCode_201_Created                     = 201,
        HttpStatusCode_202_Accepted                    = 202,
        HttpStatusCode_203_NonAuthoritativeInformation = 203,
        HttpStatusCode_204_NoContent                   = 204,
        HttpStatusCode_205_ResetContent                = 205,
        HttpStatusCode_206_PartialContent              = 206,

        /*---------------------------
          3xx   重定向
        ---------------------------*/
        HttpStatusCode_300_MultipleChoices             = 300,
        HttpStatusCode_301_MovedPermanently            = 301,
        HttpStatusCode_302_Found                       = 302,
        HttpStatusCode_303_SeeOther                    = 303,
        HttpStatusCode_304_NotModified                 = 304,
        HttpStatusCode_305_UseProxy                    = 305,
        HttpStatusCode_307_TemporaryRedirect           = 307,

        /*---------------------------
          4xx   客户端错误
        ---------------------------*/
        HttpStatusCode_400_BadRequest                  = 400,
        HttpStatusCode_401_Unauthorized                = 401,
        HttpStatusCode_402_PaymentRequired             = 402,
        HttpStatusCode_403_Forbidden                   = 403,
        HttpStatusCode_404_NotFound                    = 404,
        HttpStatusCode_405_MethodNotAllowed            = 405,
        HttpStatusCode_406_NotAcceptable               = 406,
        HttpStatusCode_407_ProxyAuthenticationRequired = 407,
        HttpStatusCode_408_RequestTimeout              = 408,
        HttpStatusCode_409_Conflict                    = 409,
        HttpStatusCode_410_Gone                        = 410,
        HttpStatusCode_411_LengthRequired              = 411,
        HttpStatusCode_412_PreconditionFailed          = 412,
        HttpStatusCode_413_RequestEntityTooLarge       = 413,
        HttpStatusCode_414_RequestURITooLarge          = 414,
        HttpStatusCode_415_UnsupportedMediaType        = 415,
        HttpStatusCode_416_RequestedRangeNotSatisfiable= 416,
        HttpStatusCode_417_ExpectationFailed           = 417,

        /*---------------------------
          5xx   服务端错误
        ---------------------------*/
        HttpStatusCode_500_InternalServerError         = 500,
        HttpStatusCode_501_NotImplemented              = 501,
        HttpStatusCode_502_BadGateway                  = 502,
        HttpStatusCode_503_ServerUnavailable           = 503,
        HttpStatusCode_504_GatewayTimeout              = 504,
        HttpStatusCode_505_HTTPVersionNotSupported     = 505,

    } HttpStatusCode;

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
#endif // end of define __BP_HTTPSTATUSCODE_H_20101217143757__