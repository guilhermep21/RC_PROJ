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



char* randomAnsGen() {
    srand(time(NULL));
    char* resultado = malloc(4 * sizeof(char));
    if (resultado == NULL) {
        printf("Erro ao alocar memoria!\n");
        return NULL;
    }
    char colors[] = {'R', 'G', 'B', 'Y', 'O', 'P'};

    for (int i = 0; i < 4; i++) {
        int color = rand() % 6;
        resultado[i] = colors[color];
    }
    return resultado;
}

void write_on_file(char *input, char *game_file, char *answer) {
    strtok(input, " "); //removing cmd
    char *user_id = strtok(NULL, " ");
    int time_player = atoi(strtok(NULL, " "));
    FILE *file = fopen(game_file, "w");

    if(file == NULL){
        perror("[Error]: openning file.\n");
        return;
    }
    time_t now = time(NULL);
    struct tm *utc_time = gmtime(&now);

    fprintf(file, "%s P %s %d %04d-%02d-%02d %02d:%02d:%02d %ld\n", user_id, answer, time_player, utc_time->tm_year + 1900, utc_time->tm_mon + 1,
    utc_time->tm_mday, utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec, now);

    fclose(file);
    return;
}

int create_user_gm_dir(char *userid) {
    char user_gm_dir[strlen(userid) + strlen(GAME_DIR)];
    sprintf(user_gm_dir, "%s%s", GAME_DIR ,userid);

    if(mkdir(user_gm_dir, 0777) == -1){
        fprintf(stderr, "Error creating directory\n");
        return -1;
    }

    return 0;  
}


int create_game_start_file(char *userid) {
    char game_file[strlen(userid) + strlen(GAME_FILE_PREFIX) + strlen(GAME_DIR)];
    sprintf(game_file, "%s%s%s",GAME_DIR, GAME_FILE_PREFIX, userid);

    FILE* f_game = fopen(game_file, "w");

    if(f_game == NULL){
        fprintf(stderr, "Error creating game file.\n");
        return -1;
    }

}

void process_player_start(char *input, char **response) {
    char *input_cpy = malloc(strlen(input) + 1);
    strcpy(input_cpy, input);
    strtok(input, " ");
    char *user_id = strtok(NULL, " ");
    char *time = strtok(NULL, " ");
    char game_file[strlen(user_id) + strlen(GAME_FILE_PREFIX) + strlen(GAME_DIR)];
    sprintf(game_file, "%s%s%s", GAME_DIR, GAME_FILE_PREFIX, user_id);

    //check if the input is valid
    if(strlen(user_id) != 6 || atoi(time) > 600 || atoi(time) < 0 || strlen(time) != 3) {
        *response = (char*) malloc(sizeof(char) * ERR_LEN);
        sprintf((*response), "%s %s\n", START_RESPONSE, ERR_STATUS);
        return;
    }


    char user_gm_dir[strlen(user_id) + strlen(GAME_DIR)];
    sprintf(user_gm_dir, "%s%s", GAME_DIR, user_id);

    if(access(game_file, F_OK) == 0) {
        *response = (char*) malloc(sizeof(char) * NOK_LEN);
        sprintf((*response), "%s %s\n", START_RESPONSE, NOK_STATUS);
        printf("Player already in game");
        return;
    }
    else {
        if(create_game_start_file(user_id) == -1) {
            fprintf(stderr, "Error creating game file.\n");
            return;
        }
    }

    if(access(user_gm_dir, F_OK) != 0) {
        if(create_user_gm_dir(user_id) == -1) {
            fprintf(stderr, "Error creating user directory\n");
            return;
        }
    }



    *response = (char*) malloc(sizeof(char) * OK_LEN);
    sprintf((*response), "%s %s\n", START_RESPONSE, OK_STATUS);
    printf("%s\n", (*response));
    char answer[ANSWER_LEN];
    strcpy(answer, randomAnsGen());
    
    printf("aqui\n");
    printf("input: %s, game_file: %s, answer: %s\n", input, game_file, answer);
    //writting on GAME file start info
    write_on_file(input_cpy, game_file, answer);

    free(input_cpy);
    
}

int main() {
    char *response = NULL;
    char input[] = "STR 107177 100";
    
    process_player_start(input, &response);
}