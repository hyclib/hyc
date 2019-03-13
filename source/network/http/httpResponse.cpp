//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/http/httpResponse.h"
#include "httpResponse_private.hpp"

namespace hyc
{
   
    //----------------------------------------------------------------------------------------------------
    //*	HttpResponse
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    HttpResponse::HttpResponse()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    //-----------------------------------------------
    HttpResponse::~HttpResponse()
    {
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    const char* HttpResponse::getHeader( const char* pName ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        HeaderMap::iterator itr = p->m_mapHeaders.find( pName );
        if ( itr == p->m_mapHeaders.end() )
            return NULL;
        else
            return (*itr).second.c_str();
    }

    //-----------------------------------------------
    const char* HttpResponse::getHeader( uinteger index ) const
    {
        PRIVATE_CLASS_GET_CONST(p);

        if ( index > p->m_mapHeaders.size() )
        {
            return NULL;
        }

        uinteger i = 0;
        HeaderMap::iterator itr = p->m_mapHeaders.begin();
        while ( itr != p->m_mapHeaders.end() )
        {
            ++itr;
            ++i;
        }

        return (*itr).second.c_str();
    }

    //-----------------------------------------------
    uinteger HttpResponse::getHeaderNum() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_mapHeaders.size();
    }

    //-----------------------------------------------
    hyc::HttpStatusCode HttpResponse::getStatusCode() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_statusCode;
    }

    //-----------------------------------------------
    const char* HttpResponse::getStatusCodeDesc() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_statusCodeDesc.c_str();
    }

    //-----------------------------------------------
    hyc::HttpVersion HttpResponse::getHttpVersion() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_httpVersion;
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
