/*************************************************************************
	> File Name: master.h
	> Author: 
	> Mail: 
	> Created Time: 2018年11月17日 星期六 22时03分45秒
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

#define PTHREAD 6
#define INS 5 
#ifdef _DEBUG
#define DBG(fmt , args...)  printf(fmt, ##args) 
#else
#define DBG(fmt , args...)  
#endif

typedef struct Node{
    struct sockaddr_in addr_client;
    struct Node *next;
}Node;

typedef struct LinkedList{
    Node head;
    int length;
    int num;
}LinkedList;

LinkedList linkedlist[INS + 1];
pthread_mutex_t mutex[5];
int recv_port, client_port, warning_port, server_listen_port;
int finish, start;
char *init_host;

#define _MASTER_H
#endif


