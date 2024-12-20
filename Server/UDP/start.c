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
#include "start.h"
#include <time.h>
#include "../constants.h"

char *randomAnsGen() {
    srand(time(NULL));
    char *resultado = malloc(4 * sizeof(char));
    char colors[] = {'R', 'G', 'B', 'Y', 'O', 'P'};

    for (int i = 0; i < 4; i++) {
        int color = rand() % 6;
        resultado[i] = colors[color];
    }
    return resultado;
}

void write_on_file_start(FILE *file, char *answer, char *user_id, char* time_player) {

    if(file == NULL){
        perror("[Error]: openning file.\n");
        return;
    }

    time_t now = time(NULL);
    struct tm *utc_time = gmtime(&now);

    fprintf(file, "%s P %s %s %04d-%02d-%02d %02d:%02d:%02d %ld\n", user_id, answer, time_player, utc_time->tm_year + 1900, utc_time->tm_mon + 1,
    utc_time->tm_mday, utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec, now);

    fclose(file);
    return;
}

int create_user_gm_dir(char *userid) {
    char user_gm_dir[strlen(userid) + strlen(GAME_DIR)];
    sprintf(user_gm_dir, "%s%s", GAME_DIR, userid);

    if(mkdir(user_gm_dir, 0777) == -1){
        fprintf(stderr, "Error creating directory\n");
        return -1;
    }

    return 0;  
}


int create_game_start_file(char *game_file, char *userid, char *time_player) {
    printf("file name: %s\n", game_file);
    FILE *f_game = fopen(game_file, "w");

    if(f_game == NULL){
        fprintf(stderr, "Error creating game file.\n");
        return -1;
    }
    
    char *answer = randomAnsGen();
    write_on_file_start(f_game, answer, userid, time_player);
    
    return 0;
}

void process_player_start(char *input, char **response) {

    char *input_cpy = malloc(strlen(input) + 1);
    strcpy(input_cpy, input);
    char *user_id = strtok(input_cpy, " ");
    char *time_player = strtok(NULL, " ");

    char game_file[strlen(user_id) + strlen(GAME_FILE_PREFIX) + strlen(GAME_DIR) +1];
    sprintf(game_file, "%s%s%s",GAME_DIR, GAME_FILE_PREFIX, user_id);
    game_file[strlen(game_file)] = '\0';

    //check if the input is valid
    if(strlen(user_id) != 6 || atoi(time_player) > 600 || atoi(time_player) < 0 || strlen(time_player) != 3) {     
        *response = (char*) malloc(sizeof(char) * START_ERR_LEN);
        sprintf((*response), "%s %s\n", START_RESPONSE, ERR_STATUS);
        return;
    }


    else if(access(game_file, F_OK) == 0){
        *response = (char*) malloc(sizeof(char) * START_NOK_LEN);
        sprintf((*response), "%s %s\n", START_RESPONSE, NOK_STATUS);
        return;
    }

    else{
        if(create_game_start_file(game_file, user_id, time_player) == -1) {
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



    *response = (char*) malloc(sizeof(char) * START_OK_LEN);
    sprintf((*response), "%s %s\n", START_RESPONSE, OK_STATUS);



      
    //writting on GAME file start info


    free(input_cpy);
    
}

/*
int main() {
    FILE *file = NULL;
    char *response = NULL;
    char input[] = "107177 100";
    process_player_start(file, input, &response);
    free(response);
}*/