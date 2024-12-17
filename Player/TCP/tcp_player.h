#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define FILE_TO_BE_SENT 1

#define FILE_TO_BE_RECEIVED 0

#define MAX_TCP_NULL_FILERCV 8

typedef struct file {
    int code;
    char* filename;
} file;

typedef struct TCP_response {
    file* file;
    char* msg;
} TCP_response;

int TCP_cmd(char* cmd);
int send_TCP(TCP_response* response, char* ip, char* port);

#endif // TCP_H