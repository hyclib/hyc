//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/http/httpUtils.h"
#include "../../../include/network/http/httpResponse.h"
#include "../../../include/utils/fileUtils.h"
#include "../../../include/io/fileStream.h"

namespace hyc
{

    //-----------------------------------------------
    void HttpUtils::AnalyzeAddr( const tchar* pAddr, OUT cString& protocol, OUT cString& host, OUT cString& port, OUT cString& URI )
    {
        u32 eePos = 0;
        u32 eePos2 = 0;

        host.clear();
        port = "80";
        URI.clear();

        if (!pAddr) return;

        /*---------------------------
          protocol
        ---------------------------*/
        StringUtils::StrToStr(pAddr, URI);
        URI.toLower();
        u32 pos = URI.find( "://" );
        if ( cString::npos == pos ) pos = 0;
        else {
            if ( 0 != pos ) {
                protocol = URI.substr( 0, pos );
            }
            pos += 3;
        } // if..else

        /*---------------------------
          port
        ---------------------------*/
        u32 endPos = URI.find( ":", pos );
        if ( cString::npos != endPos )
        {
            ++endPos;
            eePos = URI.find( "/", endPos );
            if ( cString::npos == eePos )   eePos = URI.size()-1;
            
            port = URI.substr( endPos, eePos-endPos );
            eePos2 = endPos - 1;
        }
        else
        {
            endPos = URI.find( "/", pos );
            eePos2 = endPos;
            eePos  = endPos;
        }

        /*---------------------------
          host
        ---------------------------*/
        host = URI.substr( pos, eePos2-pos );

        if ( cString::npos != eePos )   URI = URI.substr( eePos, URI.size() );
        else
            URI = "/";
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
