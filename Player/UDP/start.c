#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "start.h"




int process_start(char* input, player** user, char** response){
    strtok(input, " ");


    (*user)->uid = atoi(strtok(NULL, " "));
    (*user)->player_time = atoi(strtok(NULL, " "));
    (*user)->nT = 1;

    *response = (char*) malloc(sizeof(char) * START_LEN);
    
    int return_code = sprintf(*response, "%s %d %d\n", START_CMD, (*user)->uid, (*user)->player_time);

    if (return_code < 0)
        return -1;    
    
    return 0;

}

/*int main() {
    player* user = (player*)malloc(sizeof(player));
    char *response = NULL;

    char input[] = "START 107177 123";

    int result = process_start(input, &user, &response);
    printf("%s\n", response);

    free(response);
    free(user);

}*/


