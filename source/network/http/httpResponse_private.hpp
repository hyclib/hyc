//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_HTTPRESPONSE_PRIVATE_HPP_20101222143409__
#define __BP_HTTPRESPONSE_PRIVATE_HPP_20101222143409__
#ifdef HYC_COMPILE_MODULE_NET

#include <map>
#include "../../../include/container/byteStream.h"

namespace hyc
{
    typedef std::map<cString, cString>  HeaderMap;

    //----------------------------------------------------------------------------------------------------
    //*	HttpResponse::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class HttpResponse::PrivateClass : public MemoryAllocObject
    {
    public:
        PrivateClass()  { }

        // header map
        mutable HeaderMap   m_mapHeaders;

        // 状态码
        HttpVersion         m_httpVersion;
        HttpStatusCode      m_statusCode;
        cString             m_statusCodeDesc;
    };


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
#endif // end of define __BP_HTTPRESPONSE_PRIVATE_HPP_20101222143409__
