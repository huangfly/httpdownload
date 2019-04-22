/*
* =============================================================================
*
*       文件名:  		task.h
*
*    	 描述:    		单个下载任务 继承与taskbase
*
*       版本:    		V1.0
*       创建时间:  	2019-04-21 17:10
*       编译器:  		gcc
*       作者:  		huangpengfei, 798677116@qq.com
*       修改日志:  	1. Create file.
*
* =============================================================================
*/

#ifndef __TASK_H
#define __TASK_H


#include "threadpool/ThreadBase.h"
#include <curl/curl.h>
#include <string>
#include <vector>
using namespace std;
class CurlHttpTask :public TaskBase
{
public:
	/*
	*函数功能：下载器的构造函数
	*
	*输入参数：文件指针，写文件开始的位置，结束的位置，下载的url，是否分段
	*
	*返回值：void
	*/
	CurlHttpTask(FILE* f, long startPos, long stopPos, string url,bool ranged):m_fp(f), m_startPos(startPos), m_stopPos(stopPos), m_url(url),m_ranged(ranged){}
	
	/*
	*函数功能：重写taskbase的run方法
	*
	*输入参数：void
	*
	*返回值：void
	*/
	void Run();

	
	/*
	*函数功能：设置线程的名称
	*
	*输入参数：自定义线程名称
	*
	*返回值：void
	*/
	void SetName(string name){
		this->m_taskName = name;
	}
public:
	CurlHttpTask(){}
	static pthread_mutex_t m_mutex;
	FILE*    m_fp;  //本地文件句柄
	long     m_startPos;
	long     m_stopPos;
	string   m_url;
	bool     m_ranged; //是否需要http range下载?
};

#endif