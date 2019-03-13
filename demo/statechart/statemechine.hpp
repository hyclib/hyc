//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_STATEMECHINE_HPP_20130801221536__
#define __BP_STATEMECHINE_HPP_20130801221536__

#include "hyc/statechart/state_mechine.hpp"

/**
* 以下代码设计如下的状态机.
*
*      StateA
*      |     |
*      v     v
*  StateB -> StateC
*/

// states.
class StateA;
class StateB;
class StateC;

// events.
class Event1 : public StateEvent    {};
class Event2 : public StateEvent    {};
class Event3 : public StateEvent    {};

// state a
class StateA : public State
{
public:
    StateA()    {
        addTransition<Event1, StateB>();
        addTransition<Event2, StateC>();
    }
public:
    virtual void    onInitial() { print_debug("init a"); }
    virtual void    onEnter( const StateEvent* pEvent ) { print_debug("enter a"); }
    virtual void    onLeave( const StateEvent* pEvent ) { print_debug("leave a"); }
    virtual void    onUpdate() {}
};

// state b
class StateB : public State
{
public:
    StateB()    {
        addTransition<Event3, StateC>();
    }
public:
    virtual void    onInitial() { print_debug("init b"); }
    virtual void    onEnter( const StateEvent* pEvent ) { print_debug("enter b"); }
    virtual void    onLeave( const StateEvent* pEvent ) { print_debug("leave b"); }
    virtual void    onUpdate() {}
};

// state c
class StateC : public State
{
public:
    StateC()    {
    }
public:
    virtual void    onInitial() { print_debug("init c"); }
    virtual void    onEnter( const StateEvent* pEvent ) { print_debug("enter c"); }
    virtual void    onLeave( const StateEvent* pEvent ) { print_debug("leave c"); }
    virtual void    onUpdate() {}
};


void statemechine()
{
    StateMechine sm;

    // add all state.
    sm.addState<StateA>();
    sm.addState<StateB>();
    sm.addState<StateC>();

    // initial.
    sm.initiate<StateA>();

    // process event.
    sm.process_event(Event1());     // stateA->stateB.
    sm.process_event(Event2());     // don't do anything.
    sm.process_event(Event3());     // stateB->stateC
}

#endif // end of define __BP_STATEMECHINE_HPP_20130801221536__