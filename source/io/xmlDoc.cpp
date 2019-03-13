//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_XML

#include "../../include/common/cnObjectPtr.h"
#include "../../include/utils/stringUtils.h"
#include "../../include/utils/fileUtils.h"
#include "../../include/utils/encodingUtils.h"

#include "../../include/io/xmlNode.h"
#include "../../include/io/xmlDoc.h"

#include "../../include/io/fileStream.h"

#include "../../ext/rapidxml/rapidxml.hpp"
#include "../../ext/rapidxml/rapidxml_print.hpp"
#include "xmlPrivateClass.hpp"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma warning(disable: 4996)  // func_s
#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS

namespace hyc
{
    
    //----------------------------------------------------------------------------------------------------
    //*	XmlDoc
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    XmlDoc::XmlDoc()
    {
        PRIVATE_CLASS_CREATE;
        PRIVATE_CLASS_GET(p);
        tchar str[] = {_T("")};
        p->m_document.parse<rapidxml::parse_default>(str);
    }
    
    //-----------------------------------------------
    XmlDoc::~XmlDoc()
    {
        clear();
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    bool XmlDoc::parseFile( const tchar* pszFile )
    {
        PRIVATE_CLASS_GET(p);

        if ( StringUtils::IsEmpty(pszFile) )  {
            trace(_T("xml filepath empty"));
            return false;
        }

        // 读取流.
        FileStream fs;
        if ( !fs.open( pszFile ) )  {
            trace(_T("xml file can't open"));
            return false;
        }

        // buf长度不够, 重新申请.
        integer len = fs.getByteSize();
        if ( 0 >= len )  { p->m_strPath = pszFile; return true; }
        if ( (uinteger)len > p->m_bufNumOfByte )
        {
            SAFE_DELETE( p->m_pFileBuf );
            p->m_pFileBuf       = new tchar[len+1];
            p->m_bufNumOfByte   = len;
        }
        p->m_pFileBuf[len] = 0;
        //tcsset( p->m_pFileBuf, len+1, 0 );

        // 读取文件.
        utf8* buf = new utf8[len];
        integer readLen = fs.readByte(buf, len);
        if ( readLen < 0 ) {
            SAFE_DELETE_ARRAY(buf);
            trace(_T("xml file can't read"));
            return false;
        }

        if (EncodingUtils::IsLegalUTF8File(buf, (u32)readLen))
        {
            u32 i = EncodingUtils::UTF8ToStr(buf+3, (u32)readLen-3, p->m_pFileBuf, (u32)len);
            p->m_pFileBuf[i] = 0;
        }
        else
        {
            memcpy(p->m_pFileBuf, buf, readLen);
            p->m_pFileBuf[readLen] = 0;
        }

        // 解析.
        try {
            p->m_document.parse<rapidxml::parse_default>( p->m_pFileBuf );
        } catch (rapidxml::parse_error&) {
            trace(_T("xml file can't analysis"));
            return false;
        }

        p->m_strPath = pszFile;
        return true;
    }

    //-----------------------------------------------
    bool XmlDoc::parseStream( const tchar* buf )
    {
        PRIVATE_CLASS_GET(p);

        p->m_strPath.clear();
        if ( NULL == buf )  return false;

        // buf长度不够, 重新申请
        uinteger len = StringUtils::GetLength( buf );
        if ( len > p->m_bufNumOfByte )
        {
            SAFE_DELETE( p->m_pFileBuf );
            p->m_pFileBuf       = new tchar[len+1];
            p->m_bufNumOfByte   = len;
        }
        p->m_pFileBuf[len] = 0;
        tcsset( p->m_pFileBuf, len, 0 );
        tcscpy( p->m_pFileBuf, buf );

        // 解析
        try {
            p->m_document.parse<rapidxml::parse_default>( p->m_pFileBuf );
        } catch (rapidxml::parse_error&) {
            return false;
        }

        return true;
    }

    //-----------------------------------------------
    bool XmlDoc::saveToFile( const tchar* pszFile /*= NULL */, SaveFormat sf/* = SF_Default*/ )
    {
        PRIVATE_CLASS_GET(p);

        const tchar* path = NULL;

        // 路径选择
        if ( StringUtils::IsEmpty(pszFile) )
        {
            if ( p->m_strPath.empty() ) return false;
            path = p->m_strPath.c_str();
        }
        else
        {
            path = pszFile;
        }

        FileUtils::FileAssure( path );
        FileStream fs;
        if ( !fs.open( path, FileStream::FM_ReadWrite|FileStream::FM_New ) )  return false;

        // 读取
#if defined(_UNICODE) || defined(UNICODE)
        std::wstring    buf;
#else
        std::string    buf;
#endif // defined(_UNICODE) || defined(UNICODE)

        if ( SF_Default == sf )
            rapidxml::print( std::back_inserter(buf), p->m_document, rapidxml::print_no_indenting );
        else
            rapidxml::print( std::back_inserter(buf), p->m_document );

        // 写入文件
        if ( 0 > fs.write( buf.c_str(), buf.size() ) )  return false;

        return true;
    }

    //-----------------------------------------------
    bool XmlDoc::saveToStream( OUT tchar* buf, SaveFormat sf/* = SF_Default*/ )
    {
        PRIVATE_CLASS_GET(p);

        if ( NULL == buf )  return false;

        tchar* end = NULL;
        if ( SF_Default == sf )
            end = rapidxml::print( buf, p->m_document, rapidxml::print_no_indenting );
        else
            end = rapidxml::print( buf, p->m_document );

        *end = 0;

        return true;
    }

    //-----------------------------------------------
    void XmlDoc::clear()
    {
        PRIVATE_CLASS_GET(p);

        p->m_document.clear();
        p->m_bufNumOfByte = 0;
        SAFE_DELETE_ARRAY( p->m_pFileBuf );
        p->m_pFileBuf = NULL;
    }

    //-----------------------------------------------
    XmlNode* XmlDoc::getRoot()
    {
        PRIVATE_CLASS_GET(p);
        return &p->m_root;
    }

    //-----------------------------------------------
    const XmlNode* XmlDoc::getRoot() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return &p->m_root;
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_XML
