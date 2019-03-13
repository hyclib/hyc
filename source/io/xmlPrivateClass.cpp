//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#ifdef HYC_COMPILE_MODULE_XML

#include "../../include/io/xmlDoc.h"
#include "../../include/io/xmlNode.h"

#include "../../ext/rapidxml/rapidxml.hpp"
#include "../../ext/rapidxml/rapidxml_print.hpp"
#include "xmlPrivateClass.hpp"

namespace hyc
{

    XmlDoc::PrivateClass::PrivateClass()
         : m_pFileBuf(NULL), m_bufNumOfByte(0)
    { 
        m_root.m_pPrivateClass->m_pNode = &m_document;
        m_root.m_pPrivateClass->m_pDoc = this;
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_XML
