#ifndef UDP_H
#define UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "quit.h"
#include "start.h"
#include "try.h"
#include "debug.h"

int UDP_cmd(char* cmd);
int send_UDP(char *msg, char** udp_buffer, char* ip, char* port);

#endif // UDP_H