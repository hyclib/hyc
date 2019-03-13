//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_XML

#include "../../include/common/cnObjectPtr.h"
#include "../../include/container/memoryAlloc.h"
#include "../../include/utils/stringUtils.h"

#include "../../include/io/xmlNode.h"
#include "../../include/io/xmlDoc.h"

#include "../../ext/rapidxml/rapidxml.hpp"
#include "xmlPrivateClass.hpp"

namespace hyc
{    
    //----------------------------------------------------------------------------------------------------
    //*	XmlNode
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    XmlNode::XmlNode()
    {
        PRIVATE_CLASS_CREATE;
//         PRIVATE_CLASS_GET(p);
//         if ( pDoc )
//         {
//             p->m_pDoc  = &(pDoc->m_pPrivateClass->m_document);
//             p->m_pNode = p->m_pDoc->allocate_node( rapidxml::node_element );
//         }
    }

    //-----------------------------------------------
    XmlNode::XmlNode( const XmlNode& node )
    {
        PRIVATE_CLASS_CREATE;
        (*this) = node;
    }

    //-----------------------------------------------
    XmlNode::~XmlNode()
    {
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    XmlNode& XmlNode::operator=( const XmlNode& node )
    {
        PRIVATE_CLASS_GET(p);
        PRIVATE_CLASS_GET_OTHER_CONST(&node, p2);
        p->m_pNode = p2->m_pNode;
        p->m_pDoc  = p2->m_pDoc;
        return *this;
    }

    //-----------------------------------------------
    const tchar* XmlNode::getName() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_pNode->name();
    }

    //-----------------------------------------------
    void XmlNode::setName( const tchar* pszName )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->name( p->m_pDoc->m_document.allocate_string(pszName) );
    }

    //-----------------------------------------------
    XmlNode* XmlNode::addChild( const tchar* name )
    {
        PRIVATE_CLASS_GET(p);
        if (!p->m_pDoc) return NULL;

        ObjectPtr<XmlNode>  pNode = new XmlNode();
        PRIVATE_CLASS_GET_OTHER(pNode.get(), p2);
        p2->m_pDoc = p->m_pDoc;
        p2->m_pNode = p->m_pDoc->m_document.allocate_node( rapidxml::node_element );
        pNode->setName(name);
        p->m_pNode->append_node(p2->m_pNode);

        p->m_pDoc->m_mapNodes.insert( std::make_pair(p2->m_pNode, pNode) );
        return pNode.get();
    }

    //-----------------------------------------------
    void XmlNode::removeChild( XmlNode* pNode )
    {
        if ( NULL == pNode )    return;

        PRIVATE_CLASS_GET(p);
        PRIVATE_CLASS_GET_OTHER(pNode, p2);

        rapidxml::xml_node<tchar>* pNode1 = p2->m_pNode;
        p->m_pNode->remove_node( pNode1 );
        XmlDoc::PrivateClass::NodeMap::iterator itr = p->m_pDoc->m_mapNodes.find(pNode1);
        if (p->m_pDoc->m_mapNodes.end() != itr)
        {
            p->m_pDoc->m_mapNodes.erase(itr);
        }
    }

    //----------------------------------------
    void XmlNode::removeAllChild()
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();
    }

    //-----------------------------------------------
    XmlNode* XmlNode::getParentNode()
    {
        PRIVATE_CLASS_GET(p);

        // 不存在返回
        rapidxml::xml_node<tchar>* pParentNode = p->m_pNode->parent();
        if (!pParentNode)   return NULL;

        // 查找是否存在
        XmlDoc::PrivateClass::NodeMap::iterator itr = p->m_pDoc->m_mapNodes.find( pParentNode );
        if (p->m_pDoc->m_mapNodes.end() != itr)
        {
            return (*itr).second.get();
        }
        else
        {
            ObjectPtr<XmlNode>  pNode = new XmlNode();
            PRIVATE_CLASS_GET_OTHER(pNode, p2);
            p2->m_pNode = pParentNode;
            p2->m_pDoc = p->m_pDoc;
            p->m_pDoc->m_mapNodes.insert( std::make_pair(p2->m_pNode, pNode) );
            return pNode.get();
        }
    }

    //-----------------------------------------------
    XmlNode* XmlNode::getFirstChild( const tchar* pszNodeName/* = NULL*/ )
    {
        PRIVATE_CLASS_GET(p);

        // 不存在返回
        rapidxml::xml_node<tchar>* pFirstNode = p->m_pNode->first_node(pszNodeName);
        if (!pFirstNode)   return NULL;

        // 查找是否存在
        XmlDoc::PrivateClass::NodeMap::iterator itr = p->m_pDoc->m_mapNodes.find( pFirstNode );
        if (p->m_pDoc->m_mapNodes.end() != itr)
        {
            return (*itr).second.get();
        }
        else
        {
            ObjectPtr<XmlNode>  pNode = new XmlNode();
            PRIVATE_CLASS_GET_OTHER(pNode, p2);
            p2->m_pNode = pFirstNode;
            p2->m_pDoc = p->m_pDoc;
            p->m_pDoc->m_mapNodes.insert( std::make_pair(p2->m_pNode, pNode) );
            return pNode.get();
        }
    }

    //-----------------------------------------------
    XmlNode* XmlNode::getPrevSibling( const tchar* pszNodeName/* = NULL*/ )
    {
        PRIVATE_CLASS_GET(p);

        // 不存在返回
        rapidxml::xml_node<tchar>* pSiblingNode = p->m_pNode->previous_sibling(pszNodeName);
        if (!pSiblingNode)   return NULL;

        // 查找是否存在
        XmlDoc::PrivateClass::NodeMap::iterator itr = p->m_pDoc->m_mapNodes.find( pSiblingNode );
        if (p->m_pDoc->m_mapNodes.end() != itr)
        {
            return (*itr).second.get();
        }
        else
        {
            ObjectPtr<XmlNode>  pNode = new XmlNode();
            PRIVATE_CLASS_GET_OTHER(pNode, p2);
            p2->m_pNode = pSiblingNode;
            p2->m_pDoc = p->m_pDoc;
            p->m_pDoc->m_mapNodes.insert( std::make_pair(p2->m_pNode, pNode) );
            return pNode.get();
        }
    }

    //-----------------------------------------------
    XmlNode* XmlNode::getNextSibling( const tchar* pszNodeName/* = NULL*/ )
    {
        PRIVATE_CLASS_GET(p);

        // 不存在返回
        rapidxml::xml_node<tchar>* pSiblingNode = p->m_pNode->next_sibling(pszNodeName);
        if (!pSiblingNode)   return NULL;

        // 查找是否存在
        XmlDoc::PrivateClass::NodeMap::iterator itr = p->m_pDoc->m_mapNodes.find( pSiblingNode );
        if (p->m_pDoc->m_mapNodes.end() != itr)
        {
            return (*itr).second.get();
        }
        else
        {
            ObjectPtr<XmlNode>  pNode = new XmlNode();
            PRIVATE_CLASS_GET_OTHER(pNode, p2);
            p2->m_pNode = pSiblingNode;
            p2->m_pDoc = p->m_pDoc;
            p->m_pDoc->m_mapNodes.insert( std::make_pair(p2->m_pNode, pNode) );
            return pNode.get();
        }
    }

    //-----------------------------------------------
    void XmlNode::setValue( bool val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();
        p->m_pNode->value( val?
            p->m_pDoc->m_document.allocate_string(_T("1"))
            :p->m_pDoc->m_document.allocate_string(_T("0")) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( u8 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::U32ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( u16 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::U32ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( u32 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::U32ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( u64 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::U64ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }
    //-----------------------------------------------
    void XmlNode::setValue( s8 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::S32ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( s16 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::S32ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( s32 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::S32ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( s64 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::S64ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }
    //-----------------------------------------------
    void XmlNode::setValue( f32 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::F32ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( f64 val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        String str; StringUtils::F64ToStr(val, str);
        p->m_pNode->value( p->m_pDoc->m_document.allocate_string( str.c_str() ) );
    }

    //-----------------------------------------------
    void XmlNode::setValue( const tchar* val )
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_nodes();

        if (val)
        {
            p->m_pNode->value( p->m_pDoc->m_document.allocate_string( val ) );
        }
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT bool& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = false;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            if ( _T('1') == pVal[0] )
                val = true;
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT u8& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = (u8)StringUtils::StrToU32(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT u16& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = (u16)StringUtils::StrToU32(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT u32& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = StringUtils::StrToU32(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT u64& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = StringUtils::StrToU64(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT s8& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = (s8)StringUtils::StrToS32(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT s16& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = (s16)StringUtils::StrToS32(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT s32& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = StringUtils::StrToS32(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT s64& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = StringUtils::StrToS64(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT f32& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0.0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = StringUtils::StrToF32(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT f64& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val = 0.0;
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = StringUtils::StrToF64(pVal);
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getValue( OUT String& val ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        val.clear();
        const tchar* pVal = p->m_pNode->value();
        if ( pVal )
        {
            val = pVal;
            return true;
        }
        return false;
    }

    //-----------------------------------------------
    void XmlNode::removeAttribute( const tchar* pszAttriName )
    {
        PRIVATE_CLASS_GET(p);
        if ( !StringUtils::IsEmpty(pszAttriName) )
        {
            rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
            if ( pAttri )
                p->m_pNode->remove_attribute( pAttri );
        }
    }

    //----------------------------------------
    void XmlNode::removeAllAttribute()
    {
        PRIVATE_CLASS_GET(p);
        p->m_pNode->remove_all_attributes();
    }

    //-----------------------------------------------
    String XmlNode::getFirstAttribute()
    {
        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute();
        if ( pAttri )
            return pAttri->name();

        return _T("");
    }

    //-----------------------------------------------
    String XmlNode::getFirstAttribute() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute();
        if ( pAttri )
            return pAttri->name();

        return _T("");
    }

    //-----------------------------------------------
    String XmlNode::getNextAttribute( const tchar* pszAttribute )
    {
        PRIVATE_CLASS_GET(p);
        if ( !StringUtils::IsEmpty(pszAttribute) )
        {
            rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttribute );
            if ( pAttri )
            {
                pAttri = pAttri->next_attribute();
                if ( pAttri )
                    return pAttri->name();
            }
        }

        return _T("");
    }

    //-----------------------------------------------
    String XmlNode::getNextAttribute( const tchar* pszAttribute ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        if ( !StringUtils::IsEmpty(pszAttribute) )
        {
            rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttribute );
            if ( pAttri )
            {
                pAttri = pAttri->next_attribute();
                if ( pAttri )
                    return pAttri->name();
            }
        }

        return _T("");
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, bool val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            pAttri->value( val?
                p->m_pDoc->m_document.allocate_string(_T("1"))
                :p->m_pDoc->m_document.allocate_string(_T("0")) );
        }
        else {
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                val?
                p->m_pDoc->m_document.allocate_string(_T("1"))
                :p->m_pDoc->m_document.allocate_string(_T("0")) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, u8 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::U32ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::U32ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, u16 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::U32ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::U32ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, u32 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::U32ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::U32ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, u64 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::U64ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::U64ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, s8 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::S32ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::S32ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, s16 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::S32ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::S32ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, s32 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::S32ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::S32ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, s64 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::S64ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::S64ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, f32 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::F32ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::F32ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, f64 val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            String str; StringUtils::F64ToStr(val, str);
            pAttri->value( p->m_pDoc->m_document.allocate_string(str.c_str()) );
        }
        else {
            String str; StringUtils::F64ToStr(val, str);
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( str.c_str() ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    void XmlNode::setAttribute( const tchar* pszAttriName, const tchar* val )
    {
        if ( StringUtils::IsEmpty(pszAttriName) || StringUtils::IsEmpty(val) )   return;

        PRIVATE_CLASS_GET(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            pAttri->value( p->m_pDoc->m_document.allocate_string(val) );
        }
        else {
            pAttri = p->m_pDoc->m_document.allocate_attribute( p->m_pDoc->m_document.allocate_string( pszAttriName ), 
                p->m_pDoc->m_document.allocate_string( val ) );
            p->m_pNode->append_attribute( pAttri );
        }
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT bool& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = false;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal && _T('1') == pVal[0] ) {
                val = true;                
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT u8& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = (u8)StringUtils::StrToU32(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT u16& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = (u16)StringUtils::StrToU32(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT u32& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = StringUtils::StrToU32(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT u64& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = StringUtils::StrToU64(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT s8& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = (s8)StringUtils::StrToS32(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT s16& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = (s16)StringUtils::StrToS32(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT s32& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = StringUtils::StrToS32(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT s64& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = StringUtils::StrToS64(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT f32& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = StringUtils::StrToF32(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT f64& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val = 0;

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = StringUtils::StrToF64(pVal);
            }
            return true;
        } // if
        return false;
    }

    //-----------------------------------------------
    bool XmlNode::getAttribute( const tchar* pszAttriName, OUT String& val ) const
    {
        if ( StringUtils::IsEmpty(pszAttriName) )   return false;
        val.clear();

        PRIVATE_CLASS_GET_CONST(p);
        rapidxml::xml_attribute<tchar>* pAttri = p->m_pNode->first_attribute( pszAttriName );
        if ( pAttri ) {
            const tchar* pVal = pAttri->value();
            if ( pVal ) {
                val = pVal;
            }
            return true;
        } // if
        return false;
    }


} // end namespace hyc

#endif // HYC_COMPILE_MODULE_XML
