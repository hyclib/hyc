#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_JSON

#include "../../include/io/jsonDoc.h"
#include "../../include/io/jsonObject.h"
#include "../../include/io/fileStream.h"
#include "../../include/utils/encodingUtils.h"
#include "../../include/utils/stringUtils.h"
#include "jsonDocPrivat.hpp"

namespace hyc
{
    JsonDoc::JsonDoc()
    {
        m_pPrivate = new JsonDocPrivat;
    }

    JsonDoc::~JsonDoc()
    {
        delete m_pPrivate;
    }

    bool JsonDoc::ParseFile( const tchar* pszFile )
    {
        FileStream fs;
        if ( !fs.open( pszFile, FileStream::FM_Binary|FileStream::FM_Read ) )
            return false;

        integer size = fs.getByteSize();
        Byte* pBuf = new Byte[size+1];
        if ( !pBuf )    return false;
        pBuf[size] = 0;

        fs.seek( 0, FileStream::SO_Begin );
        fs.readByte( pBuf, size );

        tchar* pDestBuf;
        uinteger wsize = size;
        bool destmalloc = false;

#if defined(UNICODE) || defined(_UNICODE)
        destmalloc = true;
        pDestBuf = new tchar[size+1];
        if (EncodingUtils::IsLegalUTF8File((utf8*)pBuf, size))
        {
            wsize = EncodingUtils::UTF8ToStr((utf8*)pBuf+3, size-3, pDestBuf, size);
            pDestBuf[wsize] = 0;
        }
        else
        {
            if (!StringUtils::Narrow2wide((char*)pBuf, size, pDestBuf, wsize))
            {
                delete[] pDestBuf;
                delete[] pBuf;
            }
            pDestBuf[wsize] = _T('\0');
        }
#else
        if (EncodingUtils::IsLegalUTF8File((utf8*)pBuf, (u32)size))
        {
            if (CN_IS_UTF8_CSTRING)
            {
                pDestBuf = (tchar*)pBuf;
            }
            else
            {
                destmalloc = true;
                pDestBuf = new tchar[size+1];
                wsize = EncodingUtils::UTF8ToStr((utf8*)pBuf+3, (u32)size-3, pDestBuf, (u32)size);
                pDestBuf[wsize] = _T('\0');
            }
        }
        else
        {
            pDestBuf = (tchar*)pBuf;
        }
#endif

        bool r = ParseStream( pDestBuf, (u32)wsize );


        if (destmalloc)
            delete[] pDestBuf;

        delete[] pBuf;

        return r;
    }

    bool JsonDoc::ParseStream( const tchar* buf, u32 size )
    {
        return m_pPrivate->Parse(buf, size);
    }

    int JsonDoc::GetErrorIndex()
    {
        return m_pPrivate->m_nCurIndex;
    }

    JsonObject* JsonDoc::GetRootObject()
    {
        if (!m_pPrivate->m_pRoot)
            m_pPrivate->m_pRoot = new JsonObject;
        return m_pPrivate->m_pRoot;
    }

    const tchar* JsonDoc::SaveToStream()
    {
        return m_pPrivate->SaveToStream();
    }

    JsonObject* JsonDoc::AddObject()
    {
        return m_pPrivate->AddObject();
    }
}

#endif // HYC_COMPILE_MODULE_JSON
