/*
* =============================================================================
*
*       �ļ���:  		task.h
*
*    	 ����:    		������������ �̳���taskbase
*
*       �汾:    		V1.0
*       ����ʱ��:  	2019-04-21 17:10
*       ������:  		gcc
*       ����:  		huangpengfei, 798677116@qq.com
*       �޸���־:  	1. Create file.
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
	*�������ܣ��������Ĺ��캯��
	*
	*����������ļ�ָ�룬д�ļ���ʼ��λ�ã�������λ�ã����ص�url���Ƿ�ֶ�
	*
	*����ֵ��void
	*/
	CurlHttpTask(FILE* f, long startPos, long stopPos, string url,bool ranged):m_fp(f), m_startPos(startPos), m_stopPos(stopPos), m_url(url),m_ranged(ranged){}
	
	/*
	*�������ܣ���дtaskbase��run����
	*
	*���������void
	*
	*����ֵ��void
	*/
	void Run();

	
	/*
	*�������ܣ������̵߳�����
	*
	*����������Զ����߳�����
	*
	*����ֵ��void
	*/
	void SetName(string name){
		this->m_taskName = name;
	}
public:
	CurlHttpTask(){}
	static pthread_mutex_t m_mutex;
	FILE*    m_fp;  //�����ļ����
	long     m_startPos;
	long     m_stopPos;
	string   m_url;
	bool     m_ranged; //�Ƿ���Ҫhttp range����?
};

#endif