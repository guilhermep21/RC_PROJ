#ifndef UDP_H
#define UDP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../constants.h"


int UDP_cmd(char* cmd);

#endif // UDP_H