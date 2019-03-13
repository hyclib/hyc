/*
 * conditionVariable.hpp
 *
 *  Created on: Feb 13, 2014
 *      Author: root
 */

#ifndef __BP_CONDITIONVARIABLE_HPP_20140213174017__
#define __BP_CONDITIONVARIABLE_HPP_20140213174017__

#include <pthread.h>

#if CN_PLATFORM == CN_PLATFORM_ANDROID
#include <linux/time.h>
#else
#include <sys/time.h>
#endif

#include "../../include/hycPlatform.h"

namespace hyc
{
    
    //----------------------------------------------------------------------------------------------------
	// ConditionVariable::PrivateClass
	//----------------------------------------------------------------------------------------------------
    class ConditionVariable::PrivateClass
    {
    public:
        PrivateClass()
        {
            // cond
            pthread_cond_init( &m_hCond, NULL );
        }
        ~PrivateClass()
        {
            pthread_cond_destroy( &m_hCond );
        }

        pthread_cond_t   m_hCond;
    };


	//----------------------------------------------------------------------------------------------------
    //!	ConditionVariable
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    ConditionVariable::ConditionVariable()
    {
        PRIVATE_CLASS_CREATE;
    }

    //-----------------------------------------------
    ConditionVariable::~ConditionVariable()
    {
    	broadcast();
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    void ConditionVariable::wait( SyncObject& sync )
    {
        PRIVATE_CLASS_GET(p);
        pthread_cond_wait( &p->m_hCond, &sync.m_hMutex);
    }

    //-----------------------------------------------
    bool ConditionVariable::waitTimeout( SyncObject& sync, s32 timeout )
    {
        //PRIVATE_CLASS_GET(p);
        //return p->_cv.waitTimeout(sync, timeout);
    	PRIVATE_CLASS_GET(p);

    	struct timeval tv;
    	struct timespec ts;
    	gettimeofday(&tv, NULL);

    	long long int tt = timeout;
    	long long int ns = tv.tv_usec * 1000 + tt * 1000*1000;

    	ts.tv_sec = tv.tv_sec + ns / 1000000000;
    	ts.tv_nsec = ns % 1000000000;

    	s32 ret = pthread_cond_timedwait( &p->m_hCond, &sync.m_hMutex, &ts);
    	return 0 == ret;
    }

    //-----------------------------------------------
    void ConditionVariable::signal()
    {
    	PRIVATE_CLASS_GET(p);
    	pthread_cond_signal( &p->m_hCond );
    }

    //-----------------------------------------------
    void ConditionVariable::broadcast()
    {
    	PRIVATE_CLASS_GET(p);
    	pthread_cond_broadcast( &p->m_hCond );
    }
}


#endif // __BP_CONDITIONVARIABLE_HPP_20140213174017__
