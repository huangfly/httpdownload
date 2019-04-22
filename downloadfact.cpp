#include "downloadfact.h"
#define RANGESIZE (1024*1024 *10)
DownloadFactory::DownloadFactory(string url){
	this->m_url = url;
}

DownloadFactory::~DownloadFactory(){
	if(!this->m_taskVec.empty()){
		vector<CurlHttpTask*>::iterator itr = m_taskVec.begin();
		for(; itr != this->m_taskVec.end(); itr++){
			delete *itr;
		}
	}
	 fclose(this->m_fp);
     this->m_fp = NULL;
	
}

long DownloadFactory::GetDownloadFileLength(const char *url){
	double file_len = 0;
	CURL *handle = curl_easy_init ();
	curl_easy_setopt (handle, CURLOPT_URL, url);
	curl_easy_setopt (handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt (handle, CURLOPT_MAXREDIRS, 3L);
	curl_easy_setopt (handle, CURLOPT_AUTOREFERER, 1L);
	curl_easy_setopt (handle, CURLOPT_HEADER, 0L);  
	curl_easy_setopt (handle, CURLOPT_NOBODY, 1L);   
	curl_easy_setopt (handle, CURLOPT_FORBID_REUSE, 1);
	
	if (curl_easy_perform (handle) == CURLE_OK)
	{
		curl_easy_getinfo (handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_len);
	}
	else
	{
		file_len = -1;
	}
	curl_easy_cleanup(handle);
	return file_len;
}

void DownloadFactory::MakeTask(string userfilename){
	long fileLen = 0L, start, stop,seg_num;

	fileLen = DownloadFactory::GetDownloadFileLength (this->GetUrl().c_str());
	if(fileLen <= 0){
		printf("this %s file len error\n", this->GetUrl().c_str());
		exit(1);
	}
	printf("%ld:%s\n", fileLen, this->GetUrl().c_str());
	string fullPath = "./" + userfilename; 
	printf("full_path = %s\n", userfilename.c_str());
	this->m_fp = fopen(fullPath.c_str(), "wb"); //在此统一打开文件
	if(this->m_fp == NULL){
		printf("file %s open error\n", fullPath.c_str());
		return;
	}
	if(fileLen < RANGESIZE){
		start = 0;
		stop = fileLen - 1;
		CurlHttpTask *task = new CurlHttpTask(this->m_fp, start, stop, this->GetUrl(), false);
		this->PushData(task);
		this->m_taskVec.push_back(task);
	}else{
		seg_num = (long)fileLen / RANGESIZE;
		for(int i = 0; i <= seg_num; i++){
			if(i < seg_num){
				start = i * RANGESIZE;
				stop = (i + 1) * RANGESIZE - 1;
			}
			else{
				if(fileLen % RANGESIZE != 0){
					start = i * RANGESIZE;
					stop = fileLen - 1;
				}else
					break;
			}
			char taskname[100];
			sprintf(taskname, "name-%d[%d-%d]", i,start, stop);
			CurlHttpTask *task = new CurlHttpTask(this->m_fp, start, stop, this->GetUrl(), true);
			task->SetName(string(taskname));
			this->PushData(task);
			this->m_taskVec.push_back(task);
		}
	}
}

void DownloadFactory::init(int defaultNum)
{
	this->m_pool.SetThreadNum(defaultNum);
	StartParser();
}

void DownloadFactory::PushData(CurlHttpTask* task)
{
	m_pool.AddTask(task);
}

void DownloadFactory::StartParser()
{
	m_pool.Start();
}

void DownloadFactory::StopParser()
{
	m_pool.Stop();
}