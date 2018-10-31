/*************************************************************************
> File Name: send.c
> Author: 
> Mail: 
> Created Time: 2018年09月23日 星期日 16时23分09秒
************************************************************************/

#include "main_client.h"

__attribute__((constructor))
void get_conf() {
    char conf_file[50] = "./con_info.log";
    client_port = atoi(get_conf_value(conf_file, "client_listen_port"));
    short_port = atoi(get_conf_value(conf_file, "server_short_listen"));
    heart_port = atoi(get_conf_value(conf_file, "server_listen_port"));
    server_host = get_conf_value(conf_file, "server_id");
    warning_port = atoi(get_conf_value(conf_file, "server_warning_port"));
    for (int i = 0; i < 6; i++) {
        char name[20];
        sprintf(name, "file_log%d\0", i);
        script[i].file_log = get_conf_value(conf_file, name); 
        sprintf(name, "script_name%d\0", i);
        script[i].script_name = get_conf_value(conf_file, name);
        sprintf(name, "sleep%d\0", i);
        script[i].sleep = atoi(get_conf_value(conf_file, name));
        script[i].num = i;
    }
}


int main() {
    //pthread_t t[PTHREAD + 1];
    for (int i = 0; i < INS; i++) {
        if(pthread_create(&t[i], NULL, func, (void *)&script[i]) == -1){
            printf("pthread_create is error\n");
            exit(1);
        }
        sleep(1);
    }
    if (pthread_create(&t[INS], NULL, heart, NULL) == -1) { 
        printf("pthread_create is error\n");
        exit(1);
    }
    int client_listen = creat_listen(client_port);
    int client_short = creat_listen(short_port);
    struct sockaddr_in server_addr;
    int client_temp; 
    while(1) {
        socklen_t len = sizeof(server_addr);
        if((client_temp = accept(client_listen, (struct sockaddr *)&server_addr, &len)) < 0) {
            close(client_temp);
        }else {
            send_file(client_temp, client_short);
            close(client_temp);
            printf("connect success!\n"); 
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

