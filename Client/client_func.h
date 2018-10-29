/*************************************************************************
	> File Name: client_func.h
	> Author: zhushuo
	> Mail: 
	> Created Time: 2018年10月04日 星期二 21时09分10秒
 ************************************************************************/

#ifndef _CLIENT_FUNC_H
#define _CLIENT_FUNC_H
#include "client.h"
#include "common.h"
/*char file_log[6][20];
char script_name[6][50];
int sleep_time[6];*/
char log_name[80];

void *func(void *argv);
void *heart(void *argv);
void send_file(int client_long, int client_listen);
void warning_detection(int num, char *buffer);

#endif
