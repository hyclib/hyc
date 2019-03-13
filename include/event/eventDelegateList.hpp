//
//  eventDelegateList.hpp
//  
//
//  Created by 李 鹏翔 on 2012-9-28.
//  Copyright (c) 2012年 华耀信息科技有限公司. All rights reserved.
//

#ifndef hy_eventDelegateList_hpp_2012928100842
#define hy_eventDelegateList_hpp_2012928100842

#include "eventFunctor.hpp"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //! 使用 Type traits
    //! Event可以存在一个消息处理函数队列.
    //----------------------------------------------------------------------------------------------------
    template<class F>
    class Event;

    //----------------------------------------------------------------------------------------------------
    //!	void()
    //----------------------------------------------------------------------------------------------------
    template<>
    class Event<void()>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void()> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)())
        {
            typedef void(T::*pFun)();
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void()>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)())
        {
            typedef void(T::*pFun)();
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()()
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])();
                if (m_preventPropagate)  break;
            }
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1)
    //----------------------------------------------------------------------------------------------------
    template<class P1>
    class Event<void(P1)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1))
        {
            typedef void(T::*pFun)(P1);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1))
        {
            typedef void(T::*pFun)(P1);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1);
                if (m_preventPropagate)  break;
            }
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2>
    class Event<void(P1, P2)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;

    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2))
        {
            typedef void(T::*pFun)(P1, P2);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2))
        {
            typedef void(T::*pFun)(P1, P2);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2);
                if (m_preventPropagate)  break;
            }
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3>
    class Event<void(P1, P2, P3)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3))
        {
            typedef void(T::*pFun)(P1,P2,P3);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3))
        {
            typedef void(T::*pFun)(P1,P2,P3);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3);
                if (m_preventPropagate)  break;
            }
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3, class P4>
    class Event<void(P1, P2, P3, P4)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3, P4)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3, P4))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3, P4)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3, P4))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3, P4 p4)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3, p4);
                if (m_preventPropagate)  break;
            }
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3, class P4, class P5>
    class Event<void(P1, P2, P3, P4, P5)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3, P4, P5)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3, P4, P5)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3, p4, p5);
                if (m_preventPropagate)  break;
            }
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3, class P4, class P5, class P6>
    class Event<void(P1, P2, P3, P4, P5, P6)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3, P4, P5, P6)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3, P4, P5, P6)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3, p4, p5, p6);
                if (m_preventPropagate)  break;
            }
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    class Event<void(P1, P2, P3, P4, P5, P6, P7)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3, P4, P5, P6, P7)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3, P4, P5, P6, P7)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3, p4, p5, p6, p7);
                if (m_preventPropagate)  break;
            }
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7,P8)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    class Event<void(P1, P2, P3, P4, P5, P6, P7, P8)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3, P4, P5, P6, P7, P8)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7, P8);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3, P4, P5, P6, P7, P8)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7, P8);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3, p4, p5, p6, p7, p8);
                if (m_preventPropagate)  break;
            }
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7,P8,P9)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
    class Event<void(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3, P4, P5, P6, P7, P8, P9)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3, P4, P5, P6, P7, P8, P9)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3, p4, p5, p6, p7, p8, p9);
                if (m_preventPropagate)  break;
            }
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)
    //----------------------------------------------------------------------------------------------------
    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
    class Event<void(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
    {
        Event(const Event&);

        typedef EventFunctor<__EventFunctorClassNoUse, void(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)> BaseDelegate;
        typedef Array<BaseDelegate*> DelegateArr;
        DelegateArr    m_arrDelegates;
        bool           m_preventPropagate;
    public:
        Event()          {}
        virtual ~Event() { removeAll(); }

        // 中断消息的继续传递.
        void preventPropagate()  { m_preventPropagate = true; }
        // 队列长度.
        u32 size() const	     { return m_arrDelegates.size(); }

        // 添加事件处理.
        template<class T>
        void add(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                    return;
            }
            BaseDelegate* pD = (BaseDelegate*)(new EventFunctor<T, void(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>(pRecever, Fun));
            m_arrDelegates.push_back(pD);
        }

        // 移除事件.
        template<class T>
        void remove(T* pRecever, void(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10))
        {
            typedef void(T::*pFun)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);
            union{
                pFun f;
                void* p;
            } u;
            u.f = Fun;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                if ((T*)m_arrDelegates[i]->_this == pRecever && m_arrDelegates[i]->_p == u.p)
                {
                    delete m_arrDelegates[i];
                    m_arrDelegates.eraseByIndex(i);
                    return;
                }
            }
        }

        // 移除事件.
        void removeAll()
        {
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                delete m_arrDelegates[i];
            }
            m_arrDelegates.clear();
        }

        // 事件响应.
        void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
        {
            m_preventPropagate = false;
            for (u32 i = 0; i < m_arrDelegates.size(); ++i)
            {
                (*m_arrDelegates[i])(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
                if (m_preventPropagate)  break;
            }
        }
    };

} // end namespace hyc

#endif // hy_eventDelegateList_hpp_2012928100842
