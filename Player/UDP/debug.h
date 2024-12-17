#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../player.h"

#define DEBUG_LEN 22 
#define DEBUG_CMD "DBG"

int process_debug(char* input, player** user, char** response);

#endif // DEBUG_H