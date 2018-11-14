/*************************************************************************
	> File Name: master.c
	> Author: zhushuo
	> Mail: 
	> Created Time: 2018年11月02日 星期日 12时02分18秒
 ************************************************************************/

#include "master_func.h"
#define MAX_SIZE 1024

char file_log[6][20] = {"Mem.log", "Disk.log", "Cpu.log", 
                       "User.log", "Sys.log", "Male.log"};

//输出任务队列
void output(LinkedList l) {
    DBG("%d\n", l.num);
    Node *p = l.head.next;
    while(p) {
        DBG("ip = %s %d\n",inet_ntoa(p->addr_client.sin_addr), htons(p->addr_client.sin_port));
        p = p->next;
    }
    DBG("\n");
    return ;
}

//链表插入操作 选择头插法 有虚拟头结点插入删除时比较方便
void insert(LinkedList* l, Node *node) {
    pthread_mutex_lock(&mutex[l->num]);
    node->next = l->head.next;
    l->head.next = node;
    l->length++;
    pthread_mutex_unlock(&mutex[l->num]);
    return ;
}

//在任务队列中删除节点node
void delete(LinkedList *l, Node *node) {
    pthread_mutex_lock(&mutex[l->num]);
    Node *p = &(l->head);
    while(p->next != NULL && p->next != node) {
        p = p->next;
    }
    if(p->next == NULL) return ;
    Node *delete_node = p->next;
    p->next = delete_node->next;
    free(delete_node);
    l->length --;
    pthread_mutex_unlock(&mutex[l->num]);
    return ;
}

//接收文件 长链接server_fd和client的IP地址
int recv_file(int server_fd, char *str) {
    int a;
    int recode;
    char addr[80];
    char buffer[MAX_SIZE];
    while(1) {
        //接收标示码
        if (recv(server_fd, &recode, 4, 0) <= 0){
            perror("recv");
            break;
        }
        send(server_fd, &recode, 4, 0);
        DBG("recode = %d\n", recode);
        sprintf(addr, "/home/zhushuo/code/DMSM_project/Log/%s/%s\0", str, file_log[recode - 100]);
        FILE *fp = fopen(addr, "a+");
        if (fp == NULL){
            fclose(fp);
            continue;
        }
        //建立短连接，接收文件
        int server_temp = connect_socked(recv_port, str);
        //DBG("short socket success : %d!\n", server_temp);
        if(server_temp > 0) {
            while((a = recv(server_temp, buffer, MAX_SIZE, 0)) > 0) {
                DBG("len = %d\n%s\n", a, buffer);
                fwrite(buffer, strlen(buffer), 1, fp);
                fflush(stdout);
                memset(buffer, 0, MAX_SIZE);
            }
            //DBG("a = %d\n%d is success\n", a, recode);
            close(server_temp);
        }
        fclose(fp);
    }
    DBG("end recv!\n");
    return 1;
}

//遍历当前的任务队列
void connect_node(LinkedList *l) {
    sleep(3);
    if (l->length == 0) sleep(5);//链表为空的情况睡眠5秒　防止空转
    Node *node = &l->head;
    while(node->next) {
        sleep(3);
        node = node->next;
        //连接当前节点的client端
        int server_socket = connect_socked(client_port, inet_ntoa(node->addr_client.sin_addr));
        //连接失败删除当前节点
        if (server_socket == 0) {
            DBG("delete %s\n", inet_ntoa(node->addr_client.sin_addr));
            delete(l, node);
            continue;
        }
        DBG("%s connect success\n", inet_ntoa(node->addr_client.sin_addr));
        char str[50] = "./Log/";
        char ip[50];
        strcpy(ip, inet_ntoa(node->addr_client.sin_addr));
        strcat(str, ip);
        mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        signal(SIGPIPE, SIG_IGN);
        //接收文件
        recv_file(server_socket, ip);
        signal(SIGPIPE, SIG_DFL);
        DBG("long_socket is close!!\n");
        close(server_socket);
    }
}


void *func(void *argv) {
    struct LinkedList *l;
    l = (struct LinkedList *)argv;
    DBG("delete start %d\n", l->num);
    //循环遍历当前任务队列
    while(1) {
        connect_node(l);
    }
    return NULL;
}

//寻找任务最少的任务队列 以便平均五个线程的负载
int find(int n, LinkedList l[]) {
    int min = 0;
    for (int i = 1; i < n; i++) {
        min = (l[min].length < l[i].length ? min : i);
    }
    return min;
}

//判断节点是否存在队列中
int if_begin(struct sockaddr_in client){
    for (int i = 0; i < INS; i++) {
        Node *p = linkedlist[i].head.next;
        while(p) {
            if (p->addr_client.sin_addr.s_addr == client.sin_addr.s_addr ) return 1;
            p = p->next;
        }
    }
    return 0;
}

//队列初始化
void init() {
    int step = ((finish - start) / INS);
    for (int i = 0; i < INS; i++) {
        linkedlist[i].length = 0;
        linkedlist[i].head.next = NULL;
        linkedlist[i].num = i;
        DBG("%d\n", linkedlist[i].num);
        for (int j = 0; j < step; j++) {
            char *host1 = (char *)malloc(sizeof(char) * 30);
            sprintf(host1,"%s.%d\0", init_host, i * step + j + start);
            Node *node = (Node *)malloc(sizeof(Node));
            node->next = NULL;
            node->addr_client.sin_family = AF_INET;
            node->addr_client.sin_port = htons(8762);
            node->addr_client.sin_addr.s_addr = inet_addr(host1);
            insert(&linkedlist[i], node);
            DBG("i = %d %s\n", i, host1);
        }
    }

}

//检测是否存在警报信息
void *func1(void *argv) {
    int warning_listen = creat_listen(warning_port);
    int server_temp;
    struct sockaddr_in client_addr;
    while(1) {
        socklen_t len = sizeof(client_addr);
        if ((server_temp = (accept(warning_listen, (struct sockaddr *)&client_addr, &len))) < 0){
            perror("accept");
            close(server_temp);
        }
        char str[50] = "./Log/Warning/";
        strcat(str, inet_ntoa(client_addr.sin_addr));
        mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        int recode;
        if(recv(server_temp, &recode, 4, 0) <= 0) {
            break;
        }
        char addr[50];
        sprintf(addr, "%s/%s", str, file_log[recode]);
        FILE *fp = fopen(addr, "a+");
        if (fp == NULL){
            fclose(fp);
            continue;
        }
        char buffer[MAX_SIZE];
        recv(server_temp, buffer, MAX_SIZE, 0);
        fwrite(buffer, strlen(buffer), 1, fp);
        fclose(fp);
        close(server_temp);
    }
}



