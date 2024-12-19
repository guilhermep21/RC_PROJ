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
#include "quit.h"
#include "../constants.h"

void write_on_file_quit (char *game_file_dir, int plid, time_t current_seconds) {
    FILE *file = fopen(game_file_dir, "a+");

    struct tm *utc_time = gmtime(&current_seconds);
    fseek(file, 37, SEEK_SET);
    long init_seconds;
    fscanf(file,"%ld", &init_seconds);
    long seconds_converted = (long)current_seconds - init_seconds;


    // write YYYY-MM-DD HH:MM:SS s 
    fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d %ld", utc_time->tm_year + 1900, utc_time->tm_mon +1, utc_time->tm_mday,
    utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec, seconds_converted);

    // ../GAMES/XXXXXX/YYYYMMDD_HHMMSS_Q 43
    char *new_dir = malloc(44 * sizeof(char));
    sprintf(new_dir, "%s%d/%04d%02d%02d_%02d%02d%02d_Q", GAME_DIR, plid, utc_time->tm_year + 1900, utc_time->tm_mon +1, utc_time->tm_mday,
    utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec);
    new_dir[strlen(new_dir)] = '\0';

    if(rename(game_file_dir, new_dir) == -1){
        perror("Erro ao mover ficheiro.\n");
    }

    free(new_dir);
    fclose(file);

    return;
}

void process_player_quit(char *input, char **response, time_t current_seconds) {
    char buffer[64];
    strcpy(buffer, input);
    int plid = atoi(strtok(buffer, " "));
    char game_file_dir[GAME_DIR_LEN + GAME_FILE_LEN + 1];
    sprintf(game_file_dir, "%s%s%d", GAME_DIR, GAME_FILE_PREFIX, plid);

    
    if(access(game_file_dir, F_OK) != 0){
        //Player is not in game
        *response = (char*) malloc(sizeof(char) * QUT_NOK_LEN);
        sprintf(*response, "%s %s\n", QUIT_RESPONSE, NOK_STATUS);
        return;
        
    }
    *response = (char*)malloc(sizeof(char) * QUT_OK_LEN);
    sprintf(*response, "%s %s\n", QUIT_RESPONSE, OK_STATUS);

    write_on_file_quit(game_file_dir, plid, current_seconds);


}

/*
int main() {

    time_t current_seconds = time(NULL);
    //FILE *file = fopen("../GAMES/GAME_107177", "r");
    char *input = "QUT 107177";
    char *response;
    process_player_quit(input, &response, current_seconds);
    free(response);
}*/