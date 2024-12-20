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
#include "show_trials.h"
#include <dirent.h>

int FindLastGame (char *PLID , char *fname){
    struct dirent **filelist;
    int n_entries, found;
    char dirname [20];

    //sprintf(dirname, "GAMES/%s/", PLID);
    sprintf(dirname, "GAMES/%s/", PLID);

    n_entries = scandir (dirname, &filelist, 0, alphasort) ;

    found = 0;

    if (n_entries <= 0)
        return (0);
    else {
    while (n_entries--) {
        if (filelist [n_entries]->d_name[0] != '.' && ! found){
            sprintf(fname, "GAMES/%s/%s" ,PLID , filelist[n_entries]->d_name);
            found =1;
        }
        free(filelist[n_entries]);
    }
    free(filelist);
    }
    return (found);
}

char *show_trials_write(FILE *file, char *plid, time_t current_time, int game_status){
    char *response = malloc(600 * sizeof(char));
    char buffer_line[128];
    char temp[128];
    char time_to_play[4];
    char read_time[20];
    long time_init;
    long time_left;
    long time_passed;

    if(game_status == 1)
        sprintf(temp, "Active game found for player %s\n", plid);
    else
        sprintf(temp, "Last game found for player %s\n", plid);

    strcpy(response, temp);
    strcat(response, "\n");

    fseek(file, 13, SEEK_SET);
    fread(time_to_play, 4, sizeof(time_to_play), file);

    fseek(file, 37, SEEK_SET);
    fgets(read_time, sizeof(read_time), file);
    time_init = atol(read_time);
    time_passed = (long)current_time - time_init;
    time_left = atol(time_to_play) - time_passed;

    while(fgets(buffer_line, sizeof(buffer_line), file) != NULL){
        if(buffer_line[0] == 'T'){
            strtok(buffer_line, " "); // remove T:
            strcat(response, "Trial: ");
            strcat(response, strtok(NULL, " ")); // junta a resposta
            strcat(response, " nB:");
            strcat(response, strtok(NULL, " "));
            strcat(response, " nW:");
            strcat(response, strtok(NULL, " "));
            strcat(response, " at  sec:");
            strcat(response, strtok(NULL, " "));
            //strcat(response, "\n");
        }
    }
    strcat(response, "\n");

    if(game_status == 1){
        sprintf(buffer_line, "-- %ld seconds remaining to be completed --", time_left);
        strcat(response, buffer_line);
    }    
    else
        strcat(response, "-----------------------------------");

    strcat(response, "\n");

    return response;

}


void process_player_show_trials(char *input, char **response){
    char buffer[1000];
    int in_game = 0;
    time_t current_time = time(NULL);
    FILE *game_file;
    char *last_game_dir = malloc(64 * sizeof(char));
    char plid[7];
    char RSG_status[9];
    char active_game_dir[strlen(GAME_DIR) + strlen(GAME_FILE_PREFIX) + PLID_LEN + 3];
    strcpy(plid, input);
    plid[strlen(input)] = '\0';
    sprintf(active_game_dir, "%s%s%s", GAME_DIR, GAME_FILE_PREFIX, plid);
    int n = FindLastGame(plid, last_game_dir);

    char *fname = malloc(10 * sizeof(char));
    sprintf(fname, "st_%s", plid);

    // caso NOK
    if((access(active_game_dir, F_OK) != 0) && (access(last_game_dir, F_OK) != 0)){
        *response = (char*) malloc(sizeof(char) * RST_NOK_LEN);
        sprintf((*response), "%s %s\n", SHOW_TRIALS_RESPONSE, NOK_STATUS);
        free(fname);
        return;
    }

    // caso ACT
    if(access(active_game_dir, F_OK) == 0){
        game_file = fopen(active_game_dir, "r");
        if (game_file == NULL)
        {
            printf("Erro ao abrir ficheiro\n");
            return;
        }
        sprintf(RSG_status,"%s %s ", SHOW_TRIALS_RESPONSE, ACT_STATUS);
        in_game = 1;
    }
    // caso FIN
    else{
        if(n == 1){
            game_file = fopen(last_game_dir, "r");
            if(game_file == NULL){
                printf("Erro ao abrir ficheiro\n");
                return;
            }
        }        
        else{
            printf("No games found\n");
            return;
        }    
        sprintf(RSG_status,"%s %s ", SHOW_TRIALS_RESPONSE, FIN_STATUS);
    }
    char *fdata = show_trials_write(game_file, plid, current_time, in_game);
    int fsize = strlen(fdata);
    char fsize_str[6];
    sprintf(fsize_str, "%d", fsize);
    strcpy(buffer, RSG_status);
    strcat(buffer, fname);
    strcat(buffer, " ");
    strcat(buffer, fsize_str);
    strcat(buffer, " ");
    strcat(buffer, fdata);
    int buffer_size = strlen(buffer);

    *response = malloc(buffer_size * sizeof(char) + 1);
    strcpy(*response, buffer);

    free(fdata);




    free(fname);
}



/*
int main(){
    char *plid = "107005";
    char *response;

    process_player_show_trials(plid, &response);

    printf("%s", response);

    free(response);
}*/


