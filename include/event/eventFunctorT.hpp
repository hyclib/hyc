//
//  eventFunctorT.hpp
//  带返回值.
//
//  Created by 李 鹏翔 on 2012-9-28.
//  Copyright (c) 2012年 华耀信息科技有限公司. All rights reserved.
//

#ifndef __BP_EVENTFUNCTORT_HPP_20130326163414__
#define __BP_EVENTFUNCTORT_HPP_20130326163414__

#include "eventFunctor.hpp"

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R()
    //----------------------------------------------------------------------------------------------------
    template<class T, class R>
    class EventFunctor<T, R()>
    {
    public:
        typedef R(T::*Fun)();
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R()>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()()              { return (_this->*_fun)(); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1>
    class EventFunctor<T, R(P1)>
    {
    public:
        typedef R(T::*Fun)(P1);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1)         { return (_this->*_fun)(p1); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2>
    class EventFunctor<T, R(P1, P2)>
    {
    public:
        typedef R(T::*Fun)(P1, P2);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2)  { return (_this->*_fun)(p1, p2); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3>
    class EventFunctor<T, R(P1, P2, P3)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3)    { return (_this->*_fun)(p1, p2, p3); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3,p4)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3, class P4>
    class EventFunctor<T, R(P1, P2, P3, P4)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3, P4);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3,P4)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4)    { return (_this->*_fun)(p1, p2, p3, p4); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3,p4,p5)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3, class P4, class P5>
    class EventFunctor<T, R(P1, P2, P3, P4, P5)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3, P4, P5);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3,P4,P5)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)    { return (_this->*_fun)(p1, p2, p3, p4, p5); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3,p4,p5,p6)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3, class P4, class P5, class P6>
    class EventFunctor<T, R(P1, P2, P3, P4, P5, P6)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3, P4, P5, P6);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3,P4,P5,P6)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)    { return (_this->*_fun)(p1, p2, p3, p4, p5, p6); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3,p4,p5,p6,p7)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    class EventFunctor<T, R(P1, P2, P3, P4, P5, P6, P7)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3,P4,P5,P6,P7)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)    { return (_this->*_fun)(p1, p2, p3, p4, p5, p6, p7); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };


    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3,p4,p5,p6,p7,p8)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    class EventFunctor<T, R(P1, P2, P3, P4, P5, P6, P7, P8)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3,P4,P5,P6,P7,P8)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)    { return (_this->*_fun)(p1, p2, p3, p4, p5, p6, p7, p8); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3,p4,p5,p6,p7,p8,p9)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
    class EventFunctor<T, R(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3,P4,P5,P6,P7,P8,P9)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)    { return (_this->*_fun)(p1, p2, p3, p4, p5, p6, p7, p8, p9); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };


    //----------------------------------------------------------------------------------------------------
    //!	EventFunctor R(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)
    //----------------------------------------------------------------------------------------------------
    template<class T, class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
    class EventFunctor<T, R(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
    {
    public:
        typedef R(T::*Fun)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);
    public:
        EventFunctor() : _this(NULL), _p(0) {}
        EventFunctor(const EventFunctor& e) { _this = e._this; _p = e._p; }
        EventFunctor(T* pRecver, Fun pFun)  { _this = pRecver; _fun = pFun; }
        virtual ~EventFunctor()             {}
        EventFunctor* clone() const         { return new EventFunctor<T,R(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)>(_this, _fun); }
        bool         isEmpty() const        { return !_this || !_p; }
        virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)    { return (_this->*_fun)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
    public:
        T*	_this;
        union { Fun _fun; void*	_p; };
    };

} // end namespace hyc

#endif // end of define __BP_EVENTFUNCTORT_HPP_20130326163414__