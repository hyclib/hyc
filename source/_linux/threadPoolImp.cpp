/*
 * threadPoolImp.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: root
 */

#include <unistd.h>
#include "threadPoolImp.h"

namespace hyc
{

	bool ThreadPoolImp::_bShudDown = false;
	int ThreadPoolImp::_icurr_thread_num = THREADPOOL_THREAD_INIT_NUM_;
	std::vector<ThreadPoolParamter> ThreadPoolImp::_items;
	std::map<pthread_t, int> ThreadPoolImp::_thread_id_map;
	pthread_mutex_t ThreadPoolImp::__thread_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t ThreadPoolImp::__thread_cond = PTHREAD_COND_INITIALIZER;

	void ThreadPoolImp::InitializeThreads()
	{
		for (int i = 0; i < THREADPOOL_THREAD_INIT_NUM_; i++)
		{
			pthread_t tThread;
			pthread_create(&tThread, NULL, ThreadPoolImp::Process, NULL);
			_thread_id_map[tThread] = 0;
		}
	}

	void* ThreadPoolImp::Process(void* arg)
	{
		while (true)
		{
			pthread_mutex_lock(&__thread_mutex);
			// to exit
			if (1 == _thread_id_map[pthread_self()])
			{
				pthread_mutex_unlock(&__thread_mutex);
				// print exit info
				pthread_exit(NULL);
			}
			//
			if (0 == _items.size() && !_bShudDown)
			{
				if (_icurr_thread_num > THREADPOOL_THREAD_INIT_NUM_)
				{
					DeleteThread();
					if (1 == _thread_id_map[pthread_self()])
					{
						pthread_mutex_unlock(&__thread_mutex);
						// print exit info
						pthread_exit(NULL);
					}
				}
				pthread_cond_wait(&__thread_cond, &__thread_mutex);
			}
			//
			if (_bShudDown)
			{
				pthread_mutex_unlock(&__thread_mutex);
				// print exit info
				pthread_exit(NULL);
			}
			//
			if (_icurr_thread_num * THREADPOOL_ADD_FACTOR_ < _items.size())
			{
				AddThread();
			}
			//
			ThreadPoolParamter parater;
			std::vector<ThreadPoolParamter>::iterator iter = _items.begin();
			parater.lpFun = iter->lpFun;
			parater.lpParamter = iter->lpParamter;
			_items.erase(iter);
			pthread_mutex_unlock(&__thread_mutex);
			//
			(parater.lpFun)(parater.lpParamter);
		}

		return NULL;
	}

	void ThreadPoolImp::QueueWorkItem(LPTHREAD_STRAT_ROUTNE lpFun, void* lpParamter)
	{
		bool bsignal = false;
		ThreadPoolParamter paramter;
		paramter.lpFun = lpFun;
		paramter.lpParamter = lpParamter;
		pthread_mutex_lock(&__thread_mutex);
		if (0 == _items.size())
			bsignal = true;
		_items.push_back(paramter);
		pthread_mutex_unlock(&__thread_mutex);
		if (bsignal)
			pthread_cond_signal(&__thread_cond);
	}

	void ThreadPoolImp::ThreadDestroy(int iwait)
	{
		while(0 != _items.size())
		{
			sleep(iwait);
		}
		_bShudDown = true;
		pthread_cond_broadcast(&__thread_cond);
		std::map<pthread_t, int>::iterator iter = _thread_id_map.begin();
		for (; iter != _thread_id_map.end(); ++iter)
			pthread_join(iter->first, NULL);
		pthread_mutex_destroy(&__thread_mutex);
		pthread_cond_destroy(&__thread_cond);
	}

	void ThreadPoolImp::AddThread()
	{
		if(((_icurr_thread_num * THREADPOOL_ADD_FACTOR_) < _items.size()) &&
				 (THREADPOOL_MAX_THREAD_NUM_ != _icurr_thread_num))
		{
			for (int i = 0; i < THREADPOOL_THREAD_INIT_NUM_; i++)
			{
				pthread_t tThread;
				pthread_create(&tThread, NULL, ThreadPoolImp::Process, NULL);
				_thread_id_map[tThread] = 0;
			}
			_icurr_thread_num += THREADPOOL_THREAD_INIT_NUM_;
		}
	}

	void ThreadPoolImp::DeleteThread()
	{
		int size = _icurr_thread_num - THREADPOOL_THREAD_INIT_NUM_;
		std::map<pthread_t, int>::iterator iter = _thread_id_map.begin();
		for (int i = 0; i < size; ++i, ++iter)
		{
			iter->second = 1;
		}
	}

} // end of namespace hyc
