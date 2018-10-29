/*************************************************************************
	> File Name: client_func.c
	> Author: zhushuo
	> Mail: 
	> Created Time: 2018年10月04日 星期二 21时07分32秒
 ************************************************************************/

#include "client_func.h"


//运行脚本
void *func(void *argv) {
    int i = ((Script *)argv)->num, parameter = 0;
    char buffer[MAX_SIZE] = {0};
    printf("i = %d\n", i);
    while(1) {
        FILE *fp;
        if ((fp = popen(script[i].script_name, "r")) == NULL) {
            perror("Fail to popen\n");
            break; 
        }
        pthread_mutex_lock(&mutex[i]);
        FILE *fp1 = fopen(script[i].file_log, "a+");
        if (fread(buffer, 1, MAX_SIZE ,fp)) {
           if (i == 4 || i == 5) warning_detection(i, buffer);
            fwrite(buffer, 1, strlen(buffer), fp1);
            memset(buffer, 0, MAX_SIZE);
        }    
        fclose(fp);
        fclose(fp1);
        pthread_mutex_unlock(&mutex[i]);
        sleep(script[i].sleep);
    }
}

void warning_detection(int num, char *buffer) {
    if (num == 4) {
        if (strstr(buffer, "warning") == NULL) return ; 
    }else if (num == 5) {
        if (!buffer[0]) return ;
    }
    int warning_fd;
    if ((warning_fd = connect_socked(warning_port, server_host)) > 0) {
        if (send(warning_fd, &num, 4, 0)) {
            perror("send");
        }    
        printf("warning_info : %s\n", buffer);
        send(warning_fd, buffer, strlen(buffer), 0);
        close(warning_fd);
    }
    return ;
}

//心跳
void *heart(void *argv) {
    while(1) {
        int client_sock;
        struct sockaddr_in server_addr;
        printf("ip = %s port = %d\n", server_host, heart_port);
        client_sock = connect_socked(heart_port, server_host);
        if (client_sock > 0) {
            close(client_sock);
            printf("heart success\n");
        }
        sleep(30);
    }
}


void send_file(int client_long, int client_listen){
    struct sockaddr_in server_short;
    int len = sizeof(server_short);
    for (int i = 100; i < 6 + 100; i++){
        if(send(client_long, &i, 4, 0) < 0) {
            perror("send");
            break;
        }
        int recode;
        if (recv(client_long, &recode, 4, 0) <= 0) {
            perror("recv");
            break;
        }
	    printf("biaoshi ma %d\n", recode);
        int client_short;
        if ((client_short = accept(client_listen, (struct sockaddr *)&server_short, &len)) < 0) {
            close(client_short);
            break;
        }
        FILE *fp;
        char command[80];
        sprintf(command, "%s\0", script[i - 100].file_log);
        char buffer[MAX_SIZE] = {0};
        pthread_mutex_lock(&mutex[i - 100]);
        if ((fp = fopen(command, "r")) == NULL) {
            close(client_short);
            pthread_mutex_unlock(&mutex[i - 100]);
            continue;  
        }
        while(fgets(buffer, MAX_SIZE, fp) != NULL) {
            printf("%s", buffer);
            if(send(client_short, buffer, strlen(buffer), 0) < 0) {
                break;
            }
            memset(buffer, 0, sizeof(buffer));            
        }
        fclose(fp);
        command[50];
        sprintf(command, "rm -rf %s\0", script[i - 100].file_log);
        system(command);
        pthread_mutex_unlock(&mutex[i - 100]);
        close(client_short);
    }
    return ;
}

