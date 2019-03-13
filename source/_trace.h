//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// author:       lipengxiang
// description:  
//

#ifndef __BP__TRACE_H_20140805141549__
#define __BP__TRACE_H_20140805141549__

#include "../include/_trace.h"

namespace hyc
{
#if defined(HYC_COMPLIE_SUPPORT_TRACE)
    extern "C" void  Trace_release(integer threadId);
#endif // HYC_COMPLIE_SUPPORT_TRACE


} // end namespace hyc

#endif // end of define __BP__TRACE_H_20140805141549__