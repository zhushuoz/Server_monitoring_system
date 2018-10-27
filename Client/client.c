/*************************************************************************
> File Name: send.c
> Author: 
> Mail: 
> Created Time: 2018年09月23日 星期日 16时23分09秒
************************************************************************/

#include "main_client.h"

int main() {
    pthread_t t[PTHREAD + 1];
    for (int i = 0; i < INS; i++) {
        para[i] = i;
        if(pthread_create(&t[i], NULL, func, (void *)&para[i]) == -1){
            printf("pthread_create is error\n");
            exit(1);
        }
        sleep(1);
    }
    if (pthread_create(&t[INS], NULL, heart, NULL) == -1) { 
        printf("pthread_create is error\n");
        exit(1);
    }
    int n = atoi(get_conf_value(log_name, "client_listen_port"));
    int client_listen = creat_listen(n);
    struct sockaddr_in server_addr;
    int client_temp; 
    int i = 0;
    while(1) {
        socklen_t len = sizeof(server_addr);
        if((client_temp = accept(client_listen, (struct sockaddr *)&server_addr, &len)) < 0) {
            close(client_temp);
        }else {
            printf("connect success!\n"); 
            send_file(client_temp);
            close(client_temp);
        } 
    }
    close(client_listen);
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    pthread_join(t[2], NULL);
    pthread_join(t[3], NULL);
    for (int i = 0; i < INS; i++) {
        pthread_mutex_destroy(&mutex[i]);
    }
    return 0;
}

