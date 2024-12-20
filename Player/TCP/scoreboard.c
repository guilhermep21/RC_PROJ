#include "scoreboard.h"
#include "tcp_player.h"
#include "../player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_scoreboard(TCP_response** response) {
    (*response)->msg = (char *) malloc(sizeof(char) * SCOREBOARD_LEN);
    (*response)->file = (file *) malloc(sizeof(file));
    (*response)->file->code = FILE_TO_BE_RECEIVED;

    int return_code = sprintf((*response)->msg, "%s\n", SCOREBOARD_CMD);
    if(return_code < 0){
        fprintf(stderr, "error: open\n");
        return -1;
    }
    return 0;
}