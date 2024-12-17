#include "UDP.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <bits/getopt_core.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

const char* UDP_commands[] = {
    "SNG",
    "TRY",
    "QUT",
    "DBG",
};

int UDP_cmd(char* cmd){
    int i;
    for (i = 0; i < 4; i++) {
        if (strcmp(cmd, UDP_commands[i]) == 0) {
            return 1;
        }
    }
    return -1;
}