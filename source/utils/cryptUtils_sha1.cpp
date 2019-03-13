//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_CRYPT_SHA1


#include "../../include/utils/cryptUtils.h"

#include "../../include/utils/stringUtils.h"
#include "../../include/utils/fileUtils.h"
#include "../../include/io/fileStream.h"
#include "../../include/container/fixArray.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   pragma warning(disable: 4018)  // signed/unsigned mismatch
#   pragma warning(disable: 4996)  //  This function or variable may be unsafe
#endif

//#include "SHA1.h"
#include "../../ext/CSHA1/SHA1.hpp"

namespace hyc
{
    class SHA1Handler::PrivateClass {
    public:
        CSHA1* _p;
    };
    
    SHA1Handler::SHA1Handler() {
        PRIVATE_CLASS_CREATE();
        PRIVATE_CLASS_GET(p);
        p->_p = NULL;
    }
    
    SHA1Handler::~SHA1Handler() {
        PRIVATE_CLASS_GET(p);
        SAFE_DELETE(p->_p);
        PRIVATE_CLASS_DESTROY;
    }
    
    //----------------------------------------------------------------------------------------------------
    //*	sha1
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    bool CryptUtils::SHA1( const Byte* pData, u32 length, OUT Digest_160& digest )
    {
        ZeroMemory(digest.digest, sizeof(digest.digest));
        if ( NULL == pData || 0 >= length)
            return false;

        CSHA1 sha1;
        sha1.Update(pData, length);
        sha1.Final();
        // or
        return sha1.GetHash(digest.digest);
    }
    
    //-----------------------------------------------
    SHA1HandlerPtr   CryptUtils::SHA1_begin()
    {
        SHA1Handler* pHandler = new SHA1Handler();
        PRIVATE_CLASS_GET_ANY(pHandler, p, SHA1Handler);
        p->_p = new CSHA1;
        return pHandler;
    }
    
    //-----------------------------------------------
    void             CryptUtils::SHA1_update( SHA1HandlerPtr pHandler, const Byte* pData, u32 length )
    {
        assert(pHandler);
        assert(pData && length);
        if (length > 0) {
            PRIVATE_CLASS_GET_ANY(pHandler, p, SHA1Handler);
            p->_p->Update(pData, length);
        }
    }
    
    //-----------------------------------------------
    bool             CryptUtils::SHA1_finish( SHA1HandlerPtr pHandler, OUT Digest_160& digest )
    {
        assert(pHandler);
        PRIVATE_CLASS_GET_ANY(pHandler, p, SHA1Handler);
        p->_p->Final();
        return p->_p->GetHash(digest.digest);
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_CRYPT_SHA1
