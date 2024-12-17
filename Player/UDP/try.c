#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "try.h"

int process_try(char* input, player** user, char** response) {
    char try[7];

    strtok(input, " "); 

    *response = (char*) malloc(sizeof(char) * (TRY_LEN));

    // Initialize msg_to_server with "TRY"
    //strcpy(*response, "TRY");

    strcpy(try, strtok(NULL, " "));
    strcat(try, " ");
    for (int i = 0; i < 2; i++){
        strcat(try, strtok(NULL, " "));
        strcat(try, " ");
    }
    strcat(try, strtok(NULL, " "));
    
    int return_code = sprintf(*response, "%s %d %s %d\n", TRY_CMD, (*user)->uid, try, (*user)->nT);
    

    if (return_code < 0)
        return -1;


    return 0; // Success
}