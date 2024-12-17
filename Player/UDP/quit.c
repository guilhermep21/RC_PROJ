
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "quit.h"



int process_quit(player** user, char** response){

    *response = (char*) malloc(sizeof(char) * QUIT_LEN);

    int return_code = sprintf(*response, "%s %d\n", QUIT_CMD, (*user)->uid);
    
    if (return_code < 0)
        return -1;    
    
    return 0; 

}