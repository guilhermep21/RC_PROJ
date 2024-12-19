#include "tcp.h"
#include "../constants.h"
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

const char *TCP_commands[] = {
    "STR",
    "SSB",
};

int TCP_cmd(char* cmd){
    int i;
    for (i = 0; i < 4; i++) {
        if (strcmp(cmd, TCP_commands[i]) == 0)
            return 1;
    }
    return -1;
}