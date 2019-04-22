/*
* ==================================================================================================================================================
*
*       �ļ���:  		downloadfact.h
*
*    	 ����:    		��������Ŀǰֻ�ṩ��http���صķ�ʽ�����ڿ����޸ĳ����һ���๤������ͨ����ͬ��Э�����������ڣ�ftp��
*
*       �汾:    		V1.0
*       ����ʱ��:  	2019-04-21 17:10
*       ������:  		gcc
*       ����:  		huangpengfei, 798677116@qq.com
*       �޸���־:  	1. Create file.
*
* ==================================================================================================================================================
*/

#ifndef __DOWNLOADFACT_H
#define __DOWNLOADFACT_H
#include "./threadpool/ThreadBase.h"
#include "task.h"
#include <vector>
using namespace std;
class DownloadFactory
{
public:
	/*
	*�������ܣ��������Ĺ��캯��
	*
	*������������ص�url����
	*
	*����ֵ��void
	*/
	DownloadFactory(const string url);

	~DownloadFactory();

	/*
	*�������ܣ���ȡ�ļ���С
	*
	*�������: ���ص�url����
	*
	*����ֵ���ļ��Ĵ�С
	*/
	static long GetDownloadFileLength (const char *url);

	/*
	*�������ܣ���������������
	*
	*�������: ���ص��ļ��浽���ص��ļ�����
	*
	*����ֵ��void
	*/
	void MakeTask(string filename);

	/*
	*�������ܣ��������������н��г�ʼ��
	*
	*�������: void
	*
	*����ֵ��void
	*/
	void init(int defaultNum = 4);





	/*
	*�������ܣ��������������
	*
	*�������: void
	*
	*����ֵ��void
	*/
	void StopParser();

	/*
	*�������ܣ���ȡ��ǰ���ص�url����
	*
	*�������: void
	*
	*����ֵ������url����
	*/
	inline string GetUrl();
private:
	/*
	*�������ܣ��������������
	*
	*�������: void
	*
	*����ֵ��void
	*/
	void StartParser();

		/*
	*�������ܣ���������ӵ��������
	*
	*�������: void
	*
	*����ֵ��void
	*/
	void PushData(CurlHttpTask* task);
private:
	vector<CurlHttpTask*> m_taskVec;
	string m_url;
	ThreadPool m_pool;
	FILE* m_fp;
};

inline string DownloadFactory::GetUrl(){
	return this->m_url;
}
#endif