/*
 * threadPool.hpp
 *
 *  Created on: Mar 6, 2014
 *      Author: root
 */

#ifndef _BP_THREADPOOL_HPP_201403061711_
#define _BP_THREADPOOL_HPP_201403061711_

#include "../../include/hycPlatform.h"
#include "../../include/os/threadPool.h"
#include "threadPoolImp.h"

namespace hyc
{

	void __ThreadPoolWorkItem(void* param)
	{
		if (param)
		{
			hyc::ThreadPoolWorkItem* pWorkItem = (hyc::ThreadPoolWorkItem*)param;
      
      pWorkItem->onStart();

      while (true)
      {
          if (pWorkItem->isExit() || pWorkItem->onInvoke())
              break;
      }
      
      pWorkItem->onFinish();
      
      pWorkItem->getCVJoin().broadcast();
      
      pWorkItem->release();
		}
	}

	//----------------------------------------------------------------------------------------------------
    //*	ThreadPool::PrivateClass
    //----------------------------------------------------------------------------------------------------
	class ThreadPool::PrivateClass
	{
	public:
		ThreadPoolImp _poolImp;
	};

	//----------------------------------------------------------------------------------------------------
    //*	ThreadPool
    //----------------------------------------------------------------------------------------------------

	//--------------------------------
	ThreadPool::ThreadPool()
	{
		PRIVATE_CLASS_CREATE;
		PRIVATE_CLASS_GET(p);
		p->_poolImp.InitializeThreads();
	}

	//--------------------------------
	ThreadPool::~ThreadPool()
	{
		PRIVATE_CLASS_GET(p);
		p->_poolImp.ThreadDestroy();
		PRIVATE_CLASS_DESTROY;
	}

	//--------------------------------
	void ThreadPool::QueueWorkItem(ThreadPoolWorkItem* pWorkItem, ThreadPoolMode mode)
	{
		if (pWorkItem)
		{
			pWorkItem->addRef();
			//
			PRIVATE_CLASS_GET(p);
			p->_poolImp.QueueWorkItem(__ThreadPoolWorkItem, pWorkItem);
		}
	}


}
#endif

