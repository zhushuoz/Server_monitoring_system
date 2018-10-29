/*************************************************************************
> File Name: test.c
> Author: 
> Mail: 
> Created Time: 2018年09月27日 星期四 18时32分36秒
************************************************************************/

#include "common.h"

char *get_conf_value(const char *pathname, const char *key_name){
    char *line;
    size_t len = 0;
    ssize_t read;
    char *value = (char *)calloc(sizeof(char), 30);
    FILE *fp = NULL;
    fp = fopen(pathname, "r");
    if (fp == NULL) {
        perror("fopen:");
        return NULL;
    }
    while ((read = getline(&line,&len,fp)) > 0) {
        char *ptr = strstr(line,key_name);
        if (ptr == NULL) continue;
        ptr += strlen(key_name);
        if (*ptr != '=') continue;
        strncpy(value, (ptr+1), strlen(ptr+2));//strlen(per+2) 少获取一个长度，代表换行
        int tempvalue = strlen(value);
        value[tempvalue] = '\0';
    }  
    fclose(fp);
    return value;
}


int connect_socked(int port, char *host) {
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        close(fd);
        perror("Socket");
    }
    struct sockaddr_in temp; 
    temp.sin_family = AF_INET;
    temp.sin_addr.s_addr = inet_addr(host);
    temp.sin_port = htons(port);
    if(connect(fd, (struct sockaddr *)&temp, sizeof(temp)) >= 0) {
        return fd;
    } 
    //printf("warning connect shibai\n");
    //printf("connect_socked = %d\n", fd);
    close(fd);
    return 0;
}

int write_Pi_log (char *piHealthLog, const char *format,...) {
    FILE *fd = fopen(piHealthLog, "a+");
    va_list arg;
    va_start(arg, format);
    fprintf(fd, "%s %s ", __DATE__, __TIME__);
    if (vfprintf(fd, format, arg) < 0) {
        perror("write log error");
    }
    va_end(arg);
    fclose(fd);
    return 0; 
}

int send_response(int sockfd, int req) {
    send(sockfd, &req, 4, 0);
    return 1;
}

int recv_response(int sockfd) {
    int req;
    int flag = recv(sockfd, &req, 4, 0);
    if (flag < 0) return 0;
    return req;
}

int check_size(char *filename, int size, char *dir) {
    unsigned int filesize = -1;
    struct stat statbuff;
    if(stat(filename, &statbuff) < 0) {
        return 0;
    }else {
        filesize = statbuff.st_size;
    }
    
    return filesize;
}
//建立监听套接字
int creat_listen(int n) {
    int server_listen;
    struct sockaddr_in listen_addr;
    if ((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        close(server_listen);
        perror("creat socket");
        return -1;
    }
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(n);
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t len = sizeof(listen_addr);
    if(bind(server_listen, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        close(server_listen);
        perror("bing socket");
        return -1;
    }
    if (listen(server_listen, 5) < 0) {
        close(server_listen);
        perror("listen socket");
    }
    return server_listen;
}







