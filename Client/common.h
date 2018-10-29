/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: 2018年11月07日 星期六 22时04分51秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
#endif
#include "client.h"

char *get_conf_value(const char *pathname, const char *key_name);
int write_Pi_log(char *piHealthLog, const char *format,...);
int send_response(int sockfd, int req);
int recv_response(int sockfd);
int check_size(char *filename, int size, char *dir);
int creat_listen(int n);
int connect_socked(int port, char *host);
