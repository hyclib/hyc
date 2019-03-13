//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  xml操作
//
#ifdef HYC_COMPILE_MODULE_XML

#ifndef __BP_XMLDOC_H_20101206215658__
#define __BP_XMLDOC_H_20101206215658__

namespace hyc
{
    class XmlNode;

    //----------------------------------------------------------------------------------------------------
    //*	XmlDoc
    //----------------------------------------------------------------------------------------------------
     
    class HYC_API XmlDoc : public INoncopyable, public RefCount
    {
        PRIVATE_CLASS_DECLARE;
    public:
        typedef enum {
            SF_Default,
            SF_Indenting,
        } SaveFormat;

    public:
        XmlDoc();
        ~XmlDoc();

    public:
        /*---------------------------
          解析;
          解析file时, 只支持utf8编码的文件.
          解析stream时, buf需以0结尾
        ---------------------------*/
        bool            parseFile( const tchar* pszFile );
        bool            parseStream( const tchar* buf );

        /*---------------------------
          存储xml文档; 
          存储至file时,NULL代表使用打开文件时的路径;
          存储至stream时,buf需足够大,将自动以0结尾
        ---------------------------*/
        bool            saveToFile( const tchar* pszFile = NULL, SaveFormat sf = SF_Default );
        bool            saveToStream( OUT tchar* buf, SaveFormat sf = SF_Default );

        void            clear();

        // (过期接口) 代表着整个文档.
        XmlNode*        getRoot();
        const XmlNode*  getRoot() const;

        // 代表着整个文档.
        XmlNode*        getNode()       { return getRoot(); }
        const XmlNode*  getNode() const { return getRoot(); }

        friend class XmlNode;
    };
    typedef ObjectPtr<XmlDoc>   XmlDocPtr;

    /**
        [example]
        XmlDoc doc;
        if ( doc.parseFile(_T("xxxx")) ) {

            // add child node
            XmlNode node;
            doc.getRoot()->addChild( &node );
            node.setName( _T("testNode") );
            node.setValue( (integer)1 );

            // add attribute
            node.setAttribute( _T("testAttri"), (integer)2 );

            // get child
            XmlNode node2(&doc);
            if ( node.getFirstChild( &node2 ) )
            {
                while ( node2.getNextSibling( &node2 ) ) {
                    // ...
                }
            } // if

            // save
            doc.saveToFile();
        }
     */

} // end namespace hyc

#endif // end of define __BP_XMLDOC_H_20101206215658__

#endif // HYC_COMPILE_MODULE_XML
