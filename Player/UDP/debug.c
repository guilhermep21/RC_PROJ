#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "debug.h"

int process_debug(char* input, player** user, char** response) {
    char code[7];

    strtok(input, " ");

    (*user)->uid = atoi(strtok(NULL, " "));

    (*user)->player_time = atoi(strtok(NULL, " "));
    (*user)->nT = 1;

    *response = (char*) malloc(sizeof(char) * DEBUG_LEN);

    strcpy(code, strtok(NULL, " "));
    strcat(code, " ");
    for (int i = 0; i < 2; i++){
        strcat(code, strtok(NULL, " "));
        strcat(code, " ");
    }
    strcat(code, strtok(NULL, " "));
    
    int return_code = sprintf(*response, "%s %d %d %s\n", DEBUG_CMD, (*user)->uid, (*user)->player_time, code);

    if (return_code < 0)
        return -1;   
    
    return 0;
}