//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// author:       lipengxiang
// description:  
//

#ifndef __BP_GLOBALVAR_HPP_20140813172438__
#define __BP_GLOBALVAR_HPP_20140813172438__

#ifdef DEBUG
#include <map>
#endif

#include "../include/os/thread.h"

namespace hyc
{
    extern Log*                         g_pTraceLog;
    extern SyncObject*                  g_pTraceSync;
    extern hyc::integer                 g_mainThreadId;

#ifdef DEBUG
    struct DebugInfo
    {
        DebugInfo( const char* fn, int l, int size ) :filename(fn),nLine(l),size(size)   {}
        cString filename;
        int    nLine;
        int    size;
    };
    extern std::map<void*, DebugInfo>*  g_pDebugMap;


#endif

} // end namespace hyc

#endif // end of define __BP_GLOBALVAR_HPP_20140813172438__