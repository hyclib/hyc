//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "preheader.h"

#include "../include/_platform/platform.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#   include "_win32/platform.hpp"

#else

namespace hyc
{
    
    //-----------------------------------------------
    hyc::s32 Platform::GetLastError()
    {
        return errno;
    }
    
    //-----------------------------------------------
    hyc::String Platform::GetLastErrorMsg( s32 errCode )
    {
        return strerror(errCode);
    }
    
    //-----------------------------------------------
    hyc::uinteger Platform::GetProcessorNum()
    {
        return (uinteger)static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN));
    }

    //-----------------------------------------------
    void Platform::GetMemoryStatus( OUT MemoryStatus& status )
    {
        FILE *fd;
        char buff[256];
        
        char name[20];
        unsigned int total;
        
        fd = fopen ("/proc/meminfo", "r");
        
        fgets (buff, sizeof(buff), fd);
        fgets (buff, sizeof(buff), fd);
        fgets (buff, sizeof(buff), fd);
        fgets (buff, sizeof(buff), fd);
        sscanf (buff, "%s %u", name, &total);
        status.totalPhy = total;
        status.totalVirtual = 0;
        
        fgets (buff, sizeof(buff), fd);
        sscanf (buff, "%s %u", name, &total);
        status.availPhy = total;
        status.availVirtual = 0;
        
        fclose(fd);     //关闭文件fd
    }
    
} // end namespace hyc


#endif
