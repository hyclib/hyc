/*
 * threadPoolImp.h
 *
 *  Created on: Mar 10, 2014
 *      Author: root
 */

#ifndef __BP_THREADPOOLIMP_H_201403101044_
#define __BP_THREADPOOLIMP_H_201403101044_

#include <map>
#include <vector>
#include <pthread.h>

typedef void (*PTHREAD_STRAT_ROUTNE)(void* lpThreadParamter);
typedef PTHREAD_STRAT_ROUTNE LPTHREAD_STRAT_ROUTNE;

#define THREADPOOL_MAX_THREAD_NUM_ 40
#define THREADPOOL_ADD_FACTOR_ 20
#define THREADPOOL_THREAD_INIT_NUM_ 4
namespace hyc
{
	class ThreadPoolParamter
	{
	public:
		LPTHREAD_STRAT_ROUTNE lpFun;
		void* lpParamter;
	};

	//--------------------------------
	//! 线程池实现
	class ThreadPoolImp
	{
	public:
		ThreadPoolImp() {}
		void QueueWorkItem(LPTHREAD_STRAT_ROUTNE lpFun, void* lpParamter);

		void InitializeThreads();
		void ThreadDestroy(int iwait = 2);
	private:
		static void* Process(void* arg);
		static void AddThread();
		static void DeleteThread();

		static bool _bShudDown;
		static int _icurr_thread_num;
		static std::map<pthread_t, int> _thread_id_map;
		static std::vector<ThreadPoolParamter> _items;

		static pthread_mutex_t __thread_mutex;
		static pthread_cond_t __thread_cond;
	};

} // end of namespace hyc

#endif /* __BP_THREADPOOLIMP_H_201403101044_ */
