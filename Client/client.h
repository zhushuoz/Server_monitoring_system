/*************************************************************************
	> File Name: master.h
	> Author: 
	> Mail: 
	> Created Time: 2018年11月07日 星期六 22时03分45秒
 ************************************************************************/

#ifndef _MASTER_H

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#define PTHREAD 7 
#define INS 6 
#define MAX_SIZE 1024

pthread_mutex_t mutex[6];
int heart_port, client_port, short_port, warning_port; 
char *server_host;
pthread_t t[PTHREAD + 1];

typedef struct Script{
    char *file_log;
    char *script_name;
    int sleep;
    int num;
}Script;
Script script[6];

#define _MASTER_H
#endif


