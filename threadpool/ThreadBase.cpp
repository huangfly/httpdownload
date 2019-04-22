#include "../tool/globle.h"
#include "ThreadBase.h"
#include <iostream>
using namespace std;

void TaskBase::SetArg(void *arg)
{
	m_arg = arg;
}

ThreadPool::~ThreadPool()
{
	m_running = false;
	m_deque.set_stop(m_running);

	delete[] m_threadBuf;
	m_threadBuf = NULL;
	while(!m_deque.empty())
	{
		m_deque.pop_front();
	}
}

ThreadPool::ThreadPool(int defaultNum)
{
	m_defaultNum = defaultNum;  
	m_running = false; 
}

void ThreadPool::Start()
{
	if(!m_running)
	{
		pthread_cond_init(&m_con, NULL);
		pthread_mutex_init(&m_mutex, NULL);
		cout << "threadpool is start" << endl;
		auto cpuNum = static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN)); 
		std::cout<<"m_defaultNum = "<<m_defaultNum<<std::endl;
		cpu_set_t cpu_info;
		m_running = true;
		m_deque.set_start(m_running);

		m_threadBuf = new pthread_t[m_defaultNum];
		for(auto i = 0, j=0; i < m_defaultNum; i++,j++)
		{

			pthread_create(m_threadBuf + i, NULL, ThreadFunc, this);
			if(cpuNum > m_defaultNum){

				pthread_join(m_threadBuf[i], NULL);
				CPU_ZERO(&cpu_info);
				CPU_SET(j,&cpu_info);
				pthread_setaffinity_np(m_threadBuf[i],sizeof(cpu_set_t),&cpu_info);
			}

		}

	}
}
void ThreadPool::SetThreadNum(const int num){
	this->m_defaultNum = num;
}
void ThreadPool::Stop()
{
	for(int i = 0; i<m_defaultNum; i++)
	{
		pthread_join(m_threadBuf[i], NULL);
	}
	/*if(m_running)
	{	
	std::cout<<"start stop"<<std::endl;
	m_running = false;
	m_deque.set_stop(m_running);
	for(int i = 0; i<m_defaultNum; i++)
	{
	pthread_join(m_threadBuf[i], NULL);
	}
	delete[] m_threadBuf;
	m_threadBuf = NULL;
	while(!m_deque.empty())
	{
	m_deque.pop_front();
	}
	}*/
}

int ThreadPool::GetThreadNum()
{
	return m_defaultNum;
}

int ThreadPool::GetTaskNum()
{
	return m_deque.size();
}

TaskBase* ThreadPool::Take()
{
	return m_deque.pop_front();
}

void ThreadPool::AddTask(TaskBase *task)
{
	m_deque.push_back(task);
	pthread_cond_broadcast(&m_con);
}

void* ThreadPool::ThreadFunc(void * arg)
{
	printf("thread_start\n");
	auto pool = static_cast<ThreadPool *>(arg);
	while(pool->m_running)
	{
		
		auto task = pool->Take();

		if(NULL == task)
		{
			break;
		}
		try
		{
			task->Run();
		}
		catch(const std::exception& e)
		{
			std::cout<<"ThreadPool: func threw "<< typeid(e).name() << " exception: " << e.what()<<std::endl;
		}
		catch(...)
		{
			std::cout<<"ThreadPool: func threw unhandled non-exception object"<<std::endl;
		}

	}
	printf("thread_stop\n");
	return (void *)NULL;
}
