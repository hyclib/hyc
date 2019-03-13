//
//  identifierUtils.hpp
//  hyc
//
//  Created by brainpoint on 14-3-22.
//  Copyright (c) 2014年 华耀信息科技有限公司. All rights reserved.
//

#ifndef HEADER_IDENTIFIERUTiLS_HPP_df234234235
#define HEADER_IDENTIFIERUTiLS_HPP_df234234235

namespace hyc
{
    //-----------------------------------------------
    String IdentifierUtils::generateUUID()
    {
        UUID uuid;
        UuidCreate(&uuid);
        
#if defined(_UNICODE) || defined(UNICODE)
        unsigned short* str;
#else
        unsigned char* str;
#endif
        UuidToString(&uuid, &str);
        
        String result = reinterpret_cast<tchar*>(str);
        
        RpcStringFree(&str);
        return result;
    }
}

#endif // HEADER_IDENTIFIERUTiLS_HPP_df234234235