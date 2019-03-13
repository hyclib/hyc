//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifdef HYC_COMPILE_MODULE_XML

#ifndef __BP_XMLPRIVATECLASS_HPP_20101212162709__
#define __BP_XMLPRIVATECLASS_HPP_20101212162709__

#include <map>

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	XmlNode::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class XmlNode::PrivateClass
    {
    public:
        PrivateClass() : m_pNode(NULL), m_pDoc(NULL)  {  }
        rapidxml::xml_node<tchar>*      m_pNode;
        XmlDoc::PrivateClass*           m_pDoc;
    };

    //----------------------------------------------------------------------------------------------------
    //*	XmlDoc::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class XmlDoc::PrivateClass
    {
    public:
        PrivateClass();
        ~PrivateClass() { SAFE_DELETE( m_pFileBuf ); }

        typedef std::map< rapidxml::xml_node<tchar>*, ObjectPtr<XmlNode> > NodeMap;
        NodeMap                         m_mapNodes;

        rapidxml::xml_document<tchar>   m_document;
        String                          m_strPath;
        tchar*                          m_pFileBuf;
        uinteger                             m_bufNumOfByte; // 字节长度
        XmlNode                         m_root;
    };


} // end namespace hyc

#endif // end of define __BP_XMLPRIVATECLASS_HPP_20101212162709__

#endif // HYC_COMPILE_MODULE_XML
