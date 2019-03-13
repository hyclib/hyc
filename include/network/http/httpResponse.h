//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  http响应
//

#ifndef __BP_HTTPRESPONSE_H_20101222142854__
#define __BP_HTTPRESPONSE_H_20101222142854__
#ifdef HYC_COMPILE_MODULE_NET


#include "httpType.h"
#include "httpStatusCode.h"
#include "../../os/thread.h"
#include "../../common/cnObjectPtrSafe.h"

namespace hyc
{
    class HttpRequest;

    //----------------------------------------------------------------------------------------------------
    //*	HttpResponse
    //      一个HttpResponse消息包括: 状态行/消息报头(可选)/请求正文(可选)
    //----------------------------------------------------------------------------------------------------

    class HYC_API HttpResponse : public INoncopyable, public MemoryAllocObject, public RefCountSafe
    {
        PRIVATE_CLASS_DECLARE;
    public:
        HttpResponse();
        ~HttpResponse();

    public:
        HttpVersion     getHttpVersion() const;

        /*---------------------------
          消息报头;
        ---------------------------*/
        const char*     getHeader( const char* pName ) const;
        const char*     getHeader( uinteger index ) const;
        uinteger        getHeaderNum() const;

        /*---------------------------
          状态码
        ---------------------------*/
        HttpStatusCode  getStatusCode() const;
        const char*     getStatusCodeDesc() const;  // 状态码描述

        friend class HttpRequest;
    };
    typedef ObjectPtr<HttpResponse> HttpResponsePtr;

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
#endif // end of define __BP_HTTPRESPONSE_H_20101222142854__
