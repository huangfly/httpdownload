/*
 * =============================================================================
 *
 *       文件名:  		ThreadBase.h
 * 
 *    	 描述:    		线程池的基础类
 *
 *       版本:    		V1.0
 *       创建时间:  	2019-04-21 18：44
 *       编译器:  		gcc
 *       作者:  		huangpengfei, 798677116@qq.com
 *       修改日志:  	1. Create file.
 *
 * =============================================================================
 */



#ifndef __THREADBASE_H
#define __THREADBASE_H

#include "../tool/kdeque.hpp"
#include <pthread.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <iostream>
using namespace std;
#define DEFAULTNUM 4
#define MAXNUM 8



/*
*
*类说明：单个任务的抽象类
*/
class TaskBase
{
public:
	/*
	*函数功能：抽象类的构造函数
	*
	*输入参数：void
	*
	*返回值：void
	*/
	TaskBase(){}

	virtual ~TaskBase() {}

	/*
	*函数功能：设置参数
	*
	*输入参数：void*arg
	*
	*返回值：void
	*/
	virtual void SetArg(void *arg);



	/*
	*函数功能：虚执行函数
	*
	*输入参数：void
	*
	*返回值：void
	*/
	virtual void Run() { cout << "run run run" << endl; };



public:
	void* m_arg;
	string m_taskName;
};

class ThreadPool
{
public:
	/*
	*函数功能：线程池的构造函数
	*
	*输入参数：线程池创建的线程数量
	*
	*返回值：void
	*/
	ThreadPool(int defaultNum = DEFAULTNUM);



	/*
	*函数功能：禁用线程池的拷贝构造函数
	*/
	ThreadPool(const ThreadPool&) = delete;	



	/*
	*函数功能：禁用线程池的拷贝赋值函数
	*/
	ThreadPool& operator=(const ThreadPool) = delete;



	/*
	*函数功能：线程池的析构函数
	*
	*输入参数：void
	*
	*返回值：void
	*/
	~ThreadPool();



	/*
	*函数功能：线程池启动
	*
	*输入参数：void
	*
	*返回值：void
	*/
	void Start();



	/*
	*函数功能：线程池停止
	*
	*输入参数：void
	*
	*返回值：void
	*/
	void Stop();



	/*
	*函数功能：获取线程池的当前线程数量
	*
	*输入参数：void
	*
	*返回值：线程数量
	*/
	int GetThreadNum();



	/*
	*函数功能：获取线程池的当前的任务数量
	*
	*输入参数：void
	*
	*返回值：任务量
	*/
	int GetTaskNum();



	/*
	*函数功能：向任务队列添加任务
	*
	*输入参数：void
	*
	*返回值：void
	*/
	void AddTask(TaskBase *task);


	void SetThreadNum(const int num);
private:
	/*
	*函数功能：向任务队列取任务
	*
	*输入参数：void
	*
	*返回值：任务指针
	*/
	TaskBase* Take();



	/*
	*函数功能：线程函数
	*
	*输入参数：void
	*
	*返回值：void
	*/
	static void* ThreadFunc(void * arg);

	pthread_cond_t m_con;

	pthread_mutex_t m_mutex;
private:
	bool m_running;
	KDeque<TaskBase*> m_deque;
	int m_defaultNum;
	int m_maxNum;
	pthread_t* m_threadBuf;
	
};


#endif