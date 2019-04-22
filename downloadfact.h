/*
* ==================================================================================================================================================
*
*       文件名:  		downloadfact.h
*
*    	 描述:    		下载器，目前只提供了http下载的方式，后期可以修改抽象成一个类工厂可以通过不同的协议下载类似于（ftp）
*
*       版本:    		V1.0
*       创建时间:  	2019-04-21 17:10
*       编译器:  		gcc
*       作者:  		huangpengfei, 798677116@qq.com
*       修改日志:  	1. Create file.
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
	*函数功能：下载器的构造函数
	*
	*输入参数：下载的url链接
	*
	*返回值：void
	*/
	DownloadFactory(const string url);

	~DownloadFactory();

	/*
	*函数功能：获取文件大小
	*
	*输入参数: 下载的url链接
	*
	*返回值：文件的大小
	*/
	static long GetDownloadFileLength (const char *url);

	/*
	*函数功能：创建下载子任务
	*
	*输入参数: 下载的文件存到本地的文件名称
	*
	*返回值：void
	*/
	void MakeTask(string filename);

	/*
	*函数功能：子任务的任务队列进行初始化
	*
	*输入参数: void
	*
	*返回值：void
	*/
	void init(int defaultNum = 4);





	/*
	*函数功能：结束任务解析器
	*
	*输入参数: void
	*
	*返回值：void
	*/
	void StopParser();

	/*
	*函数功能：获取当前下载的url链接
	*
	*输入参数: void
	*
	*返回值：返回url链接
	*/
	inline string GetUrl();
private:
	/*
	*函数功能：开启任务解析器
	*
	*输入参数: void
	*
	*返回值：void
	*/
	void StartParser();

		/*
	*函数功能：将任务添加到任务队列
	*
	*输入参数: void
	*
	*返回值：void
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