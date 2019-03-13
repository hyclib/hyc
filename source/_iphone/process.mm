/*
 *  Process.mm
 *  common
 *
 *  Created by pengxiang li on 1/14/11.
 *  Copyright 2011 hy. All rights reserved.
 *
 */

#import "../preheader.h"

#ifdef HYC_COMPILE_MODULE_PROCESS

#import <Foundation/Foundation.h>

#import "../../include/os/process.h"

#import <errno.h>
#import <stdbool.h>
#import <sys/sysctl.h>
#import <sys/signal.h>
//#import <sys/reboot.h>

namespace hyc
{
    typedef struct kinfo_proc kinfo_proc;
    
    static int GetBSDProcessList(kinfo_proc **procList, size_t *procCount)
    // Returns a list of all BSD processes on the system.  This routine
    // allocates the list and puts it in *procList and a count of the
    // number of entries in *procCount.  You are responsible for freeing
    // this list (use "free" from System framework).
    // On success, the function returns 0.
    // On error, the function returns a BSD errno value.
    {
        int                 err;
        kinfo_proc *        result;
        bool                done;
        static const int    name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
        // Declaring name as const requires us to cast it when passing it to
        // sysctl because the prototype doesn't include the const modifier.
        size_t              length;
        
        assert( procList != NULL);
        assert(*procList == NULL);
        assert(procCount != NULL);
        
        *procCount = 0;
        
        // We start by calling sysctl with result == NULL and length == 0.
        // That will succeed, and set length to the appropriate length.
        // We then allocate a buffer of that size and call sysctl again
        // with that buffer.  If that succeeds, we're done.  If that fails
        // with ENOMEM, we have to throw away our buffer and loop.  Note
        // that the loop causes use to call sysctl with NULL again; this
        // is necessary because the ENOMEM failure case sets length to
        // the amount of data returned, not the amount of data that
        // could have been returned.
        
        result = NULL;
        done = false;
        do {
            assert(result == NULL);
            
            // Call sysctl with a NULL buffer.
            
            length = 0;
            err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                         NULL, &length,
                         NULL, 0);
            if (err == -1) {
                err = errno;
            }
            
            // Allocate an appropriately sized buffer based on the results
            // from the previous call.
            
            if (err == 0) {
                result = (kinfo_proc *)malloc(length);
                if (result == NULL) {
                    err = ENOMEM;
                }
            }
            
            // Call sysctl again with the new buffer.  If we get an ENOMEM
            // error, toss away our buffer and start again.
            
            if (err == 0) {
                err = sysctl( (int *) name, (sizeof(name) / sizeof(*name)) - 1,
                             result, &length,
                             NULL, 0);
                if (err == -1) {
                    err = errno;
                }
                if (err == 0) {
                    done = true;
                } else if (err == ENOMEM) {
                    assert(result != NULL);
                    free(result);
                    result = NULL;
                    err = 0;
                }
            }
        } while (err == 0 && ! done);
        
        // Clean up and establish post conditions.
        
        if (err != 0 && result != NULL) {
            free(result);
            result = NULL;
        }
        *procList = result;
        if (err == 0) {
            *procCount = length / sizeof(kinfo_proc);
        }

        assert( (err == 0) == (*procList != NULL) );

        return err;
    }
    
    //-----------------------------------------------
    void Process::GetCurProcess( OUT ProcessInfo& info )
    {        
        /* kinfo_proc* processInfo = NULL;
        size_t count = 0;
        
        info.m_pid = getpid();
        info.m_name.clear();
        
        s32 result = GetBSDProcessList( &processInfo, &count );
        if ( !result ) {
            for ( s32 i = 0; i < count; ++i) {
                kinfo_proc p = processInfo[i];
                
                if ( p.kp_proc.p_pid == info.m_pid ) {
                    info.m_name = p.kp_proc.p_comm;
                    return;
                }
            }
        } // if */
        
        NSString* path = [[NSBundle mainBundle] bundlePath];
        info.m_name	= [path cStringUsingEncoding:NSUnicodeStringEncoding];
        info.m_pid	= getpid();        
    }
    
    //-----------------------------------------------
    void Process::GetAllProcess( OUT Array<ProcessInfo>* pArrInfo )
    {
        pArrInfo->clear();
        ProcessInfo info;
        
        kinfo_proc* processInfo = NULL;
        size_t count = 0;
        
        s32 result = GetBSDProcessList( &processInfo, &count );
        if ( !result ) {
            for ( s32 i = 0; i < count; ++i) {
                kinfo_proc p = processInfo[i];
                
                info.m_pid  = p.kp_proc.p_pid;
                info.m_name = p.kp_proc.p_comm;
                pArrInfo->push_back( info );
            }
        } // if
    }
    
    //------------------------------------------------
    bool Process::Kill( integer pid )
    {
		if ( pid < 0 )	return false;
		return 0 == kill( (pid_t)pid, SIGKILL );
    }

    bool Process::Shutdown()
    {
        //return 0 == reboot(RB_HALT);
        return false;
    }
    
    bool Process::Reboot()
    {
        //return 0 == reboot(RB_AUTOBOOT);
        return false;
    }
    
    
    
} // end namespace hyc
#endif // HYC_COMPILE_MODULE_PROCESS

