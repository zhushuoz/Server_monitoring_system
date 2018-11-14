/*************************************************************************
	> File Name: master_func.h
	> Author: zhushuo
	> Mail: 
	> Created Time: 2018年11月02日 星期日 12时08分23秒
 ************************************************************************/

#ifndef _MASTER_FUNC_H
#define _MASTER_FUNC_H
#include "master.h"
#include "common.h"
void output(LinkedList l);
void insert(LinkedList* l, Node *node);
void delete(LinkedList *l, Node *node);
int recv_file(int server_fd, char *str);
void connect_node(LinkedList *l);
void *func(void *argv);
int find(int n, LinkedList l[]);
int if_begin(struct sockaddr_in client);
void init();
void *func1(void *argv);


#endif
