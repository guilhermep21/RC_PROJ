#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <bits/getopt_core.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "../constants.h"
#include "debug.h"
#include "start.h"




void write_on_file_debug(FILE *file, char *answer, char *user_id, char* time_player) {

    if(file == NULL){
        perror("[Error]: openning file.\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *utc_time = gmtime(&now);

    fprintf(file, "%s D %s %s %04d-%02d-%02d %02d:%02d:%02d %ld\n", user_id, answer, time_player, utc_time->tm_year + 1900, utc_time->tm_mon + 1,
    utc_time->tm_mday, utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec, now);

    fclose(file);
    return;
}




int create_game_debug_file(char *game_file, char *userid, char *time_player, char *answer) {
    FILE *f_game = fopen(game_file, "w");

    if(f_game == NULL){
        fprintf(stderr, "Error creating game file.\n");
        return -1;
    }
    
    write_on_file_debug(f_game, answer, userid, time_player);
    
    return 0;
}

void process_player_debug(char *input, char **response) {
    //PLID time C1 C2 C3 C4
    char *input_cpy = malloc(strlen(input) + 1);
    strcpy(input_cpy, input);
    char *user_id = strtok(input_cpy, " ");
    char *time_player = strtok(NULL, " ");
    char *answer = malloc(5 * sizeof(char));
    strcpy(answer, strtok(NULL, " "));
    for (int i = 0; i < 3; i++){
        strcat(answer, strtok(NULL, " "));
    }


    char game_file[strlen(user_id) + strlen(GAME_FILE_PREFIX) + strlen(GAME_DIR) +1];
    sprintf(game_file, "%s%s%s",GAME_DIR, GAME_FILE_PREFIX, user_id);
    game_file[strlen(game_file)] = '\0';

    //check if the input is valid
    if(strlen(user_id) != 6 || atoi(time_player) > 600 || atoi(time_player) < 0 || strlen(time_player) != 3) {     
        *response = (char*) malloc(sizeof(char) * DEBUG_ERR_LEN);
        sprintf((*response), "%s %s\n", DEBUG_RESPONSE, ERR_STATUS);
        return;
    }


    else if(access(game_file, F_OK) == 0){
        *response = (char*) malloc(sizeof(char) * DEBUG_NOK_LEN);
        sprintf((*response), "%s %s\n", DEBUG_RESPONSE, NOK_STATUS);
        return;
    }

    else{
        if(create_game_debug_file(game_file, user_id, time_player, answer) == -1) {
            fprintf(stderr, "CGM: Error creating game file.\n");
            return;
        }
    }
    
    char user_gm_dir[strlen(user_id) + strlen(GAME_DIR)];
    sprintf(user_gm_dir, "%s%s", GAME_DIR, user_id);

    if(access(user_gm_dir, F_OK) != 0) {
        if(create_user_gm_dir(user_id) == -1) {
            fprintf(stderr, "Error creating user directory\n");
            return;
        }
    }



    *response = (char*) malloc(sizeof(char) * DEBUG_OK_LEN);
    sprintf((*response), "%s %s\n", DEBUG_RESPONSE, OK_STATUS);



      
    //writting on GAME file debug info


    free(input_cpy);
    free(answer);
    
}

