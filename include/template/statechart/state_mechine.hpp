//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_STATE_MECHINE_HPP_20130729143605__
#define __BP_STATE_MECHINE_HPP_20130729143605__

#include "../../hycPlatform.h"
#include "../../container/asyncContainer.h"
#include "state_event.hpp"
#include "state.hpp"

#include <typeinfo>

namespace hyc
{
namespace sc
{
    /**
     * 有限状态机.支持多线程.
     * 1. addState 将所有的状态添加至状态机.
     * 2. initiate 使用初始状态初始化.
     * 3. process_event 处理消息.
     */
    class StateMechine
    {
    public:
        StateMechine()          { terminate(); }
        virtual ~StateMechine() {}

        /**
         * add the state to mechine.
         */
        template<class StateT>
        void addState();

        /**
         * initiate the mechine use the initial state.
         */
        template<class StateInitT>
        void initiate();

        /**
         * deal the event.
         * 在由process_event引起的操作(如State.onEnter)中调用process_event方法可能引起失败.
         * 当调用process_event之前, 将发生状态迁移的话, 则忽略process_event的执行.
         */
        template<class EventT>
        void process_event( const EventT& event );

        /**
         * terminate the mechine.
         * 任何时刻可以调用.
         */
        virtual void terminate();
        virtual bool isTerminated() const { return NULL == _pCurState; }

        /**
         * update the current state.
         */
        virtual void update()   { lock(); if (_pCurState) _pCurState->onUpdate(); unlock(); }

    protected:
        void lock()     { _allStates.lock(); }
        void unlock()   { _allStates.unlock(); }

    private:
        AsyncMap<cString, StatePtr>      _allStates;
        StatePtr                         _pCurState;
        bool                             _willTransition;
    };

    //-----------------------------------------------
    template<class StateT>
    inline void StateMechine::addState()
    {
        const char* pKey = typeid(StateT).name();
        lock();
        if (isTerminated()) {
            AsyncMap<cString, StatePtr>::iterator itr = _allStates.find(pKey);
            if (itr == _allStates.end())
            {
                StateT* pS = new StateT();
                pS->_pMechine = this;
                _allStates.insert(itr, pKey, pS);
            }
        }
        unlock();
    }

    template<class StateInitT>
    inline void StateMechine::initiate()
    {
        const char* pKey = typeid(StateInitT).name();

        lock();
        terminate();
        AsyncMap<cString, StatePtr>::iterator itr = _allStates.find(pKey);
        if (itr != _allStates.end())
        {
            _pCurState = itr->second;

            _willTransition = true;
            for (itr = _allStates.begin(); itr != _allStates.end(); itr++)
                itr->second->onInitial();
            _willTransition = false;

            _pCurState->onEnter(NULL);
        }
        unlock();
    }

    template<class EventT>
    inline void StateMechine::process_event( const EventT& event )
    {
        lock();
        if (!_willTransition && _pCurState) // 无跳转才能执行.
        {
            const char* nextStateName = _pCurState->getTransition( typeid(event).name() );
            AsyncMap<cString, StatePtr>::iterator itr = _allStates.find(nextStateName);
            if (itr != _allStates.end())
                _willTransition = true;

            // deal event.
            _pCurState->onProcess(&event, _willTransition);
            
            if (_pCurState && _willTransition)
            {
                assert(itr != _allStates.end());
                _pCurState->onLeave(&event);
                if (_pCurState) {
                    _pCurState = itr->second;
                    _willTransition = false;
                    _pCurState->onEnter(&event);
                }
            }
        }
        _willTransition = false;
        unlock();
    }

    inline void StateMechine::terminate()
    {
        lock();
        if (!isTerminated())
        {
            _pCurState = NULL;
            _willTransition = true;
            for (AsyncMap<cString, StatePtr>::iterator itr = _allStates.begin(); itr != _allStates.end(); itr++)
                itr->second->onTerminate();
            _willTransition = false;
        }
        unlock();
    }

} // end namespace sc
} // end namespace hyc

#endif // end of define __BP_STATE_MECHINE_HPP_20130729143605__