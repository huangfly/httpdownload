#include "task.h"
#include <stdio.h>

pthread_mutex_t CurlHttpTask::m_mutex = PTHREAD_MUTEX_INITIALIZER;

size_t write_data(void* ptr, size_t size, size_t nmemb, void* userdata){
	CurlHttpTask* task = (CurlHttpTask*) userdata;
	bool ranged = task->m_ranged;
	size_t written;
	if(ranged){
		//多线程写同一个文件, 需要加锁
		pthread_mutex_lock (&CurlHttpTask::m_mutex);
		if(task->m_startPos + size * nmemb <= task->m_stopPos){
			char *buf = new char[size];
			int n = atoi(task->m_taskName.c_str());
			memset(buf,n+48,size);
			fseek(task->m_fp, task->m_startPos, SEEK_SET);
			written = fwrite(ptr, size, nmemb, task->m_fp);
			task->m_startPos += size * nmemb;
			delete[]buf;
		}
		else{
			fseek(task->m_fp, task->m_startPos, SEEK_SET);
			char *buf = new char[size];
			int n = atoi(task->m_taskName.c_str());
			memset(buf,n+48,size);
			written = fwrite(ptr, 1, task->m_stopPos - task->m_startPos + 1, task->m_fp);
			task->m_startPos = task->m_stopPos;
			delete[] buf;
		}
		pthread_mutex_unlock (&CurlHttpTask::m_mutex);
	}else{
		written = fwrite(ptr, size, nmemb, task->m_fp);
	}
	return written;
}

void CurlHttpTask::Run(){
	CURL* curl;
	CURLcode res;
	char range[64] = { 0 };
	if(this->m_ranged) {
		snprintf (range, sizeof(range), "%ld-%ld", this->m_startPos, this->m_stopPos);
		printf("range: [%s]\n", range);
	}
	printf("strat run");
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, this->m_url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3L);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5L);
	
	if(this->m_ranged){
		curl_easy_setopt(curl, CURLOPT_RANGE, range);
	}
	res = curl_easy_perform(curl);
	if(CURLE_OK != res){
		//写文件时当最后读取到的一段的大小超过endpos会只写部分，所有会有写错误产生不用处理
		if(res != 23){
			printf("Error Reason : %s\n, errorCode = %d\n", curl_easy_strerror(res),res);
		}
	}
	curl_easy_cleanup(curl);
	printf("down load ok %s\n",this->m_taskName.c_str());
}




