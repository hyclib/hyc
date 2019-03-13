//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hyc test
// author:       lipengxiang
// description:  
//

#ifndef __BP_XML_H_2014061895022__
#define __BP_XML_H_2014061895022__

void io_xml()
{
    /**
     * 代表xml文档.
     */
    XmlDoc doc;

    /**
     * 打开文档 (或使用 parseStream 直接解析流).
     */
    if (!doc.parseFile(_T("c://xxx.xml")))
    {
        return;
    }

    /**
     * 根节点.
     */
    XmlNode* pRoot = doc.getNode();

    /**
     * 子节点.
     */
    XmlNode* pChild = pRoot->addChild( _T("ChlidNode") );
    assert(pChild);

    pChild->setValue(10);                   // 设置节点值.
    pChild->setAttribute(_T("attri1"), 20); // 设置值.
    pChild->setName(_T("newName"));         // 设置名字.

    /**
     * 遍历节点.
     */
    XmlNode* pNode = pRoot->getFirstChild();    // 第一个子节点.
    pNode          = pNode->getNextSibling();   // 下一个兄弟节点.

    /**
     * 移除节点.
     */
    pRoot->removeChild(pNode);

    /**
     * 保存.
     */
    doc.saveToFile(_T("c://xxx.xml"), XmlDoc::SF_Indenting);
}

#endif // end of define __BP_XML_H_2014061895022__