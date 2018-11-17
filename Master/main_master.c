/*************************************************************************
	> File Name: 1.进程.c
	> Author: 
	> Mail: 
	> Created Time: 2018年11月15日 星期四 18时37分53秒
 ************************************************************************/

#include "main_master.h"

char log_name[80] = "/etc/pihealth_zhushuo/con_info.log";

//读取配置文件中的信息
__attribute__((constructor))
void get_conf() {
    recv_port = atoi(get_conf_value(log_name, "server_short_listen"));
    client_port = atoi(get_conf_value(log_name, "client_listen_port"));
    warning_port = atoi(get_conf_value(log_name, "server_warning_port"));
    server_listen_port = atoi(get_conf_value(log_name, "server_listen_port"));   
    init_host = get_conf_value(log_name, "prename");
    finish = atoi(get_conf_value(log_name, "finish"));
    start = atoi(get_conf_value(log_name, "start"));
    return ;
}


int main() {
    //获取程序的PID
    int pid = fork();
    if(pid < 0) {
        perror("fork");
        return 0;
    }
    if(pid > 0) {
        FILE *fp = fopen("/etc/pihealth_zhushuo/pihealth_zhushuo.pid", "w");
        if (fp == NULL) perror("fopen");
        fprintf(fp, "%d", pid);
        fclose(fp);
        return 0;
    }
    pthread_t t[PTHREAD + 1];
    //mater端的监听端口 监听client的心跳连接
    int server_listen = creat_listen(server_listen_port);
    struct sockaddr_in client_addr;
    int server_temp;
    //初始化函数
    init();
    for (int i = 0; i < INS; i++) {
        pthread_mutex_init(&mutex[i], NULL);
    }
    //遍历任务队列，并发度为5
    for (int i = 0; i < INS; i++) {
        if (pthread_create(&t[i], NULL, func, (void *)&linkedlist[i]) == -1) {
            DBG("pthread_create is error\n");
            exit(1);
        }
        sleep(1);
    }
    //监听警告信息
    pthread_create(&t[INS], NULL, func1, NULL);
    //监听心跳连接
    while(1) {
        Node *node = (Node *)malloc(sizeof(Node));
        node->next = NULL;
        socklen_t len = sizeof(client_addr);
        if((server_temp = accept(server_listen, (struct sockaddr *)&node->addr_client, &len)) < 0){
            close(server_temp);
            continue;
        }
        close(server_temp);
        //监听到client端的连接，检查任务队列中是否已经存在这个IP的节点，若不存在将表示client端信息的结构体插入任务列中
        if (if_begin(node->addr_client) != 1) {
            //插入到节点最少的任务队里，以平均任务队里的负载
            int num = find(INS, linkedlist);
            DBG("num = %d ip = %s %d\n", num, inet_ntoa(node->addr_client.sin_addr), htons(node->addr_client.sin_port));
            insert(&linkedlist[num], node);
        }else {
            //如果已经存在此IP，释放当前节点
            DBG("ip = %s is exist\n", inet_ntoa(node->addr_client.sin_addr));
            free(node);
        } 
        for (int i = 0; i < INS; i++) {
            output(linkedlist[i]);
        }
    }
    for (int i = 0; i < INS; i++){
        pthread_join(t[i], NULL);
    }
    close(server_listen);
    for (int i = 0; i < INS; i++) {
        pthread_mutex_destroy(&mutex[i]);
    }
    return 0;
}
