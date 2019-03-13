//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_STATE_HPP_20130729144032__
#define __BP_STATE_HPP_20130729144032__

#include <typeinfo>
#include "state_event.hpp"

namespace hyc
{
namespace sc
{
    class StateMechine;

    /**
     * 简单状态.
     */
    class State : public RefCountSafe
    {
    public:
        State() : _pMechine(NULL)   {}
        virtual ~State()            {}

        /**
         * 获得所属状态机.
         */
        StateMechine*       mechine()       { return _pMechine; }
        const StateMechine* mechine() const { return _pMechine; }

        /**
         * 使用[事件,下一状态]来设置状态转换.
         * 状态机启动后, 不允许再添加转换.
         */
        template<class EventT, class StateT>
        void            addTransition();
        const char*     getTransition( const char* eventName );

        /**
         * mechine initial.
         */
        virtual void    onInitial()     {}

        /**
         * mechine terminate.
         */
        virtual void    onTerminate()   {}

        /**
         * enter the state. 只在状态机进入此状态时发生 (此后无状态转变,故调用StateMechine.process_event有效).
         * @param pEvent NULL则表示其为状态机启动初始状态.
         */
        virtual void    onEnter( const StateEvent* pEvent ) {}

        /**
         * leave the state. 只在状态机离开此状态时发生 (即将进入下一状态,故调用StateMechine.process_event无效).
         */
        virtual void    onLeave( const StateEvent* pEvent ) {}

        /**
         * 处理事件. 每个事件在处理时, 都会交由当前状态处理 (调用StateMechine.process_event可能无效).
         * @param willTransition 是否将要进行状态转移, 如果为true则调用StateMechine.process_event无效
         */
        virtual void    onProcess( const StateEvent* pEvent, bool willTransition ) = 0;

        /**
         * mechine update event.
         */
        virtual void    onUpdate() = 0;

    private:
        std::map<cString, cString>          _transitions;   // [event,state]
        StateMechine*                       _pMechine;

        friend class StateMechine;
    };

   typedef ObjectPtr<State>    StatePtr;



    //-----------------------------------------------
    template<class EventT, class StateT>
    inline void State::addTransition()
    {
        const char* key = typeid(EventT).name();
        const char* value = typeid(StateT).name();

        std::map<cString, cString>::iterator itr = _transitions.find(key);
        if (itr != _transitions.end())
        {
            if (itr->second != value)
            {
                error_all(_T("the state event is exist %s"), key);
                assert(false);
            }
        }
        else
        {
            _transitions.insert(itr, std::make_pair(key, value));
        }
    }
	
    //-----------------------------------------------
    inline const char* State::getTransition( const char* eventName )
    {
        std::map<cString, cString>::iterator itr = _transitions.find(eventName);
        if (itr != _transitions.end())
        {
            return itr->second.c_str();            
        }
        else
        {
            return NULL;
        }
    }
 
} // end namespace sc
} // end namespace hyc

#endif // end of define __BP_STATE_HPP_20130729144032__
