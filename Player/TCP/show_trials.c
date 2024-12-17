#include "show_trials.h"
#include "tcp_player.h"
#include "../player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_show_trials(player* user, TCP_response** response){

    if(user->uid == 0){
        fprintf(stderr, "error: You need to start a game first\n");
        return -1;
    }

    (*response)->msg = (char *) malloc(sizeof(char) * SHOW_TRIALS_LEN);
    (*response)->file = (file *) malloc(sizeof(file));
    (*response)->file->code = FILE_TO_BE_RECEIVED;

    int return_code = sprintf((*response)->msg, "%s %d\n", SHOW_TRIALS_CMD, user->uid);

    if(return_code < 0){
        fprintf(stderr, "error: open\n");
        return -1;
    }
    return 0;
}