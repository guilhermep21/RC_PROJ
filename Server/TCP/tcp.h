#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../constants.h"


int TCP_cmd(char *cmd);

typedef struct file {
    int code;
    char* filename;
} file;

typedef struct TCP_response {
    file* file;
    char* msg;
} TCP_response;



#endif // TCP_H