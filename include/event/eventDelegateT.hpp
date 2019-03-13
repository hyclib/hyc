//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy framework
// author:       lipengxiang
// description:  带返回值的event delegate, 委托为空时抛出异常.
//

#ifndef __BP_EVENTDELEGATET_HPP_20130326162327__
#define __BP_EVENTDELEGATET_HPP_20130326162327__

#include "eventFunctorT.hpp"
#include "eventDelegate.hpp"

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //!	T()
    //----------------------------------------------------------------------------------------------------
    template<class R>
    class EventSingle<R()>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R()> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)())
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R()>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()()
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)();
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1>
    class EventSingle<R(P1)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1);
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2>
    class EventSingle<R(P1, P2)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2);
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3>
    class EventSingle<R(P1, P2, P3)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3);
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3, class P4>
    class EventSingle<R(P1, P2, P3, P4)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3, P4)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3, P4))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3,P4)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3, P4 p4)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3, p4);
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3, class P4, class P5>
    class EventSingle<R(P1, P2, P3, P4, P5)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3, P4, P5)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3, P4, P5))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3,P4,P5)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3, p4, p5);
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6>
    class EventSingle<R(P1, P2, P3, P4, P5, P6)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3, P4, P5, P6)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3, P4, P5, P6))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3,P4,P5,P6)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3, p4, p5, p6);
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    class EventSingle<R(P1, P2, P3, P4, P5, P6, P7)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3, P4, P5, P6, P7)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3,P4,P5,P6,P7)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3, p4, p5, p6, p7);
        }
    };

    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7,P8)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    class EventSingle<R(P1, P2, P3, P4, P5, P6, P7, P8)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3, P4, P5, P6, P7, P8)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3,P4,P5,P6,P7,P8)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3, p4, p5, p6, p7, p8);
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7,P8,P9)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
    class EventSingle<R(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3, P4, P5, P6, P7, P8, P9)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3,P4,P5,P6,P7,P8,P9)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        }
    };


    //----------------------------------------------------------------------------------------------------
    //!	void(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)
    //----------------------------------------------------------------------------------------------------
    template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
    class EventSingle<R(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
    {
        typedef EventFunctor<__EventFunctorClassNoUse, R(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)> BaseDelegate;
        BaseDelegate* m_delegate;
    public:
        EventSingle() : m_delegate(0)   {}
        EventSingle(const EventSingle& e){ if (e.m_delegate) m_delegate = e.m_delegate->clone(); else m_delegate = NULL;  }
        virtual ~EventSingle()          { SAFE_DELETE(m_delegate); }

        // 事件是否为空.
        bool isEmpty() const    { return !m_delegate || m_delegate->isEmpty(); }

        // 设置事件处理.
        template<class T>
        void set(T* pRecever, R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10))
        {
            SAFE_DELETE(m_delegate);
            m_delegate = (BaseDelegate*)(new EventFunctor<T, R(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)>(pRecever, Fun));
        }

        // 事件响应.
        R operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
        {
            if (!m_delegate || m_delegate->isEmpty())
                throw ExceptionType_NullPointer;
            return (*m_delegate)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
        }
    };

} // end namespace hyc

#endif // end of define __BP_EVENTDELEGATET_HPP_20130326162327__