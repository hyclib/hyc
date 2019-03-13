//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hy UI
// author:       lipengxiang
// description:  
//

#include "Stdafx.h"
#include "hyc/include/hyc.h"
#include "ThreadPool.h"

namespace Hy
{
namespace OS
{

    //-----------------------------------------------
    bool internal::__ThreadPoolWorkItem::onInvoke()
    {
        return getListener()->onInvoke();
    }

} // end namespace OS
} // end namespace Hy

