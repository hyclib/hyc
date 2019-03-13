//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_STATE_EVENT_HPP_20130729144540__
#define __BP_STATE_EVENT_HPP_20130729144540__


namespace hyc
{
namespace sc
{

    /**
     * 状态事件. 可以使用这个类传递参数.
     */
	class StateEvent 
    {
    public:
        virtual ~StateEvent()    {}

        /**
         * 使用类型来识别事件类型, 比使用动态类型识别快.
         * 可以使用此接口来做类型识别.
         */
        virtual u32 getType() const { return 0; }
    };

} // end namespace sc
} // end namespace hyc

#endif // end of define __BP_STATE_EVENT_HPP_20130729144540__