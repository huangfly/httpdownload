#include "task.h"
#include "downloadfact.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("arg num error \n");
		printf("please input like this : \"./bin/Release/server url filename\" \n");
		return -1;
	}
	curl_global_init(CURL_GLOBAL_ALL); 
	DownloadFactory fact(argv[1]);
	
	fact.MakeTask(argv[2]);
	fact.init();
	fact.StopParser();
	return 1;
}