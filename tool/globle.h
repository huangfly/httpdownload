#ifndef __GLOBLE_H
#define __GLOBLE_H

#include "err.h"
#include <stdio.h>
#include <unistd.h>
#include <string>
using namespace std;
#pragma pack(1)

#define MSG_PAGE 1024
#define MAX_MSG_PAGE (BASE_PAGE * BASE_PAGE)


#define CHECK_LOGCONF(path) \
do{ \
	char ToolDir[64] = { 0 }; \
	char ConfDir[64] = { 0 }; \
	char cmd[128] = { 0 }; \
	char buf[1024]; \
	sprintf(ToolDir, ZLOG_CONF_PATH); \
	sprintf(ConfDir, "%s", #path); \
	sprintf(cmd, "%s %s", ToolDir, ConfDir); \
	FILE*	file; \
	k_assert((file = popen(cmd, "r")) != NULL); \
	fgets(buf, sizeof(buf), file); \
	if (NULL != strstr(buf, "ERROR")) \
	{ \
		LOG_ERROR("Log Config error"); \
		abort(); \
	} \
	pclose(file);\
 } while (0);





#endif
