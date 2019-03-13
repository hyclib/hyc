//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  xml节点
//
#ifdef HYC_COMPILE_MODULE_XML

#ifndef __BP_XMLNODE_H_20101212122932__
#define __BP_XMLNODE_H_20101212122932__

#include "../common/cnObjectPtr.h"

namespace hyc
{
    class XmlDoc;

    //----------------------------------------------------------------------------------------------------
    //*	XmlNode
    //----------------------------------------------------------------------------------------------------

    class HYC_API XmlNode : public RefCount
    {
    public:
        XmlNode( const XmlNode& node );
        XmlNode&    operator=( const XmlNode& node );

    public:
        const tchar*    getName() const;
        void            setName( const tchar* pszName );

        XmlNode*        addChild( const tchar* name );
        void            removeChild( XmlNode* pNode );
        void            removeAllChild();
        XmlNode*        getParentNode();

        XmlNode*        getFirstChild( const tchar* pszNodeName = NULL );
        XmlNode*        getPrevSibling( const tchar* pszNodeName = NULL );
        XmlNode*        getNextSibling( const tchar* pszNodeName = NULL );

        /*---------------------------
          获取/设置值
        ---------------------------*/
        void            setValue( bool val );
        void            setValue( u8 val );
        void            setValue( u16 val );
        void            setValue( u32 val );
        void            setValue( u64 val );
        void            setValue( s8 val );
        void            setValue( s16 val );
        void            setValue( s32 val );
        void            setValue( s64 val );
        void            setValue( f32 val );
        void            setValue( f64 val );
        void            setValue( const tchar* val );

        bool            getValue( OUT bool& val ) const;
        bool            getValue( OUT u8& val ) const;
        bool            getValue( OUT u16& val ) const;
        bool            getValue( OUT u32& val ) const;
        bool            getValue( OUT u64& val ) const;
        bool            getValue( OUT s8& val ) const;
        bool            getValue( OUT s16& val ) const;
        bool            getValue( OUT s32& val ) const;
        bool            getValue( OUT s64& val ) const;
        bool            getValue( OUT f32& val ) const;
        bool            getValue( OUT f64& val ) const;
        bool            getValue( OUT String& val ) const;

        /*---------------------------
          属性操作;
          设置属性值时,属性不存在时,自动添加
        ---------------------------*/
        void            removeAttribute( const tchar* pszAttriName );
        void            removeAllAttribute();
        String          getFirstAttribute();
        String          getFirstAttribute() const;
        String          getNextAttribute( const tchar* pszAttribute );
        String          getNextAttribute( const tchar* pszAttribute ) const;

        void            setAttribute( const tchar* pszAttriName, bool val );
        void            setAttribute( const tchar* pszAttriName, u8 val );
        void            setAttribute( const tchar* pszAttriName, u16 val );
        void            setAttribute( const tchar* pszAttriName, u32 val );
        void            setAttribute( const tchar* pszAttriName, u64 val );
        void            setAttribute( const tchar* pszAttriName, s8 val );
        void            setAttribute( const tchar* pszAttriName, s16 val );
        void            setAttribute( const tchar* pszAttriName, s32 val );
        void            setAttribute( const tchar* pszAttriName, s64 val );
        void            setAttribute( const tchar* pszAttriName, f32 val );
        void            setAttribute( const tchar* pszAttriName, f64 val );
        void            setAttribute( const tchar* pszAttriName, const tchar* val );

        bool            getAttribute( const tchar* pszAttriName, OUT bool& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT u8& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT u16& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT u32& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT u64& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT s8& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT s16& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT s32& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT s64& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT f32& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT f64& val ) const;
        bool            getAttribute( const tchar* pszAttriName, OUT String& val ) const;

    private:
        PRIVATE_CLASS_DECLARE;
        XmlNode();
        ~XmlNode();

        friend class XmlDoc;
    };


} // end namespace hyc

#endif // end of define __BP_XMLNODE_H_20101212122932__

#endif // HYC_COMPILE_MODULE_XML
