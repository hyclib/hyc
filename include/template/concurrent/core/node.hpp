
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// create at:    2014-04-16 10:47:47
// description:  
//

#ifndef __BP_NODE_HPP_20140416104747__
#define __BP_NODE_HPP_20140416104747__


namespace hyc
{
namespace concurrent
{
    template<class T>
    struct Node
    {
        T           value;
        Node<T>*    pNext;

        Node() : pNext(NULL)    {}
        Node( const T& t, Node<T>* pNext ) : value(t), pNext(pNext) {}

        /**
         * 将当前节点链接到原链接节点的下一个节点, 并且返回原链接的节点.
         */
        Node<T>*    linkNext();

        /**
         * 将pNode节点插入到this,pNext之间; 必须保证pNode节点为单独节点.
         */
        void        insertNext( Node<T>* pNode );

        /**
         * 判断是否为结尾节点.
         */
        bool        isNextNull() const { return pNext == NULL; }
    };

    //-----------------------------------------------
    template<class T>
    inline Node<T>* hyc::concurrent::Node<T>::linkNext()
    {
        if (pNext) {
            Node<T>* p = pNext;
            pNext = p->pNext;
            return p;
        }
        return NULL;
    }

    //-----------------------------------------------
    template<class T>
    inline void hyc::concurrent::Node<T>::insertNext( Node<T>* pNode )
    {
        assert(pNode->isNextNull());
        pNode->pNext = pNext;
        pNext = pNode;
    }


} // end namespace concurrent
} // end namespace hyyun

#endif // end of define __BP_NODE_HPP_20140416104747__
