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
#include "try.h"
#include <time.h>
#include "../constants.h"

int calculate_score(long seconds, int nr_tries) {
    int score = 100;
    score -= (nr_tries - 1) * (100/8);
    score -= seconds/100;
    return score;
}

int check_tries (FILE *file, char *trial) {
    char buffer[256]; // Buffer for a line
    int nr_lines = 0;
    int counter = 0;
    
    while (fgets(buffer, sizeof(buffer), file)) {
        nr_lines++;
        for (int i = 3; i < 7; i++) {
            if (trial[i - 3] == buffer[i]){
                counter++;
            }
        }
        if (counter == 4){
            return -1;
        }
    }
    return nr_lines;
}

int check_n_send_colors(char *trial) {
    char colors[] = { 'R', 'G', 'B', 'Y', 'O', 'P'};
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            if (trial[i] == colors[j]) {
                break;
            }
        }
        if (j == 6) {
            return -1;
        }
    }
    return 1;
}

void answer_check (char *trial, char *answer, char **ans_check) {    
    int blacks = 0;
    int whites = 0;
    int i;
    int j;
    char trial_cpy[4];
    strcpy(trial_cpy, trial);

    // Primeiro, verifique as bolas pretas (caracteres na mesma posicao)
    for (i = 0; i < 4; i++) {
        if (trial_cpy[i] == answer[i]) {
            blacks++;                  // Conta uma bola preta
            answer[i] = '_';           // Marca a posicao como ja verificada
            trial_cpy[i] = '_';            // Marca a posicao do trial tambem
        }
    }

    // Agora, verifique as bolas brancas (caracteres corretos mas em posicao errada)
    for (i = 0; i < 4; i++) {
        if (trial_cpy[i] != '_') {  // So verifica se trial[i] nao foi marcado
            for (j = 0; j < 4; j++) {
                if (trial_cpy[i] == answer[j] && answer[j] != '_') {
                    whites++;              // Conta uma bola branca
                    answer[j] = '_';       // Marca a posicao como ja verificada
                    break;                  // Nao verifique mais esse caractere
                }
            }
        }
    }
    sprintf(*ans_check, "%d %d", blacks, whites);
}

void write_on_file_try (FILE *file, char *game_file_dir ,char *trial, char **ans_check, int plid, time_t current_seconds, int nr_tries) {
    char* answer = malloc(4 * sizeof(char));
    char *mode_word;
    char MODE;
    fseek(file, 6, SEEK_SET);
    fscanf(file, "%c", &MODE);
    
    if(MODE == 'P'){
        mode_word = malloc(5 * sizeof(char));
        sprintf(mode_word, "PLAY");
    }
    else{
        mode_word = malloc(6 * sizeof(char));
        sprintf(mode_word, "DEBUG");
    }
    
    fseek(file, 8, SEEK_SET);
    fscanf(file, "%s", answer);
    fseek(file, 37, SEEK_SET);
    long init_seconds;
    fscanf(file,"%ld", &init_seconds);
    long seconds_converted = (long)current_seconds - init_seconds;



    answer_check(trial, answer, ans_check);
    //int n_blacks = atoi(strtok(ans_check, " "));
    //int n_white = atoi(strtok(NULL, " "));

    // WRITE TRY
    fprintf(file, "T: %s %s %ld\n", trial, *ans_check, seconds_converted);

    struct tm *utc_time = gmtime(&current_seconds);
    if (*ans_check[0] == '4') {
        
        // write YYYY-MM-DD HH:MM:SS s 
        fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d %ld", utc_time->tm_year + 1900, utc_time->tm_mon +1, utc_time->tm_mday,
        utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec, seconds_converted);

        // ../GAMES/XXXXXX/YYYYMMDD_HHMMSS_W 43
        char *new_dir = malloc(44 * sizeof(char));
        sprintf(new_dir, "%s%d/%04d%02d%02d_%02d%02d%02d_W", GAME_DIR, plid, utc_time->tm_year + 1900, utc_time->tm_mon +1, utc_time->tm_mday,
        utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec);
        new_dir[strlen(new_dir)] = '\0';

        if(rename(game_file_dir, new_dir) == -1){
            perror("Erro ao mover ficheiro.\n");
        }
        free(new_dir);

        int score = calculate_score(seconds_converted, nr_tries);

        char score_file_dir[SCORE_FILE_DIR_LEN];
        sprintf(score_file_dir, "%s%03d_%d_%04d%02d%02d_%02d%02d%02d", SCORE_DIR, score, plid,
        utc_time->tm_year + 1900, utc_time->tm_mon +1, utc_time->tm_mday, utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec);
        score_file_dir[strlen(score_file_dir)] = '\0';
        printf("SCOREDIR: %s\n", score_file_dir);
        FILE *score_file = fopen(score_file_dir, "a+"); //open file

        fprintf(score_file, "%03d %d %s %d %s", score, plid, trial, nr_tries, mode_word);

        free(mode_word);
        fclose(score_file);
    }

    else if (nr_tries == 8) {
        fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d %ld", utc_time->tm_year + 1900, utc_time->tm_mon +1, utc_time->tm_mday,
        utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec, seconds_converted);

        // ../GAMES/XXXXXX/YYYYMMDD_HHMMSS_W 43
        char *new_dir = malloc(44 * sizeof(char));
        sprintf(new_dir, "%s%d/%04d%02d%02d_%02d%02d%02d_F", GAME_DIR, plid, utc_time->tm_year + 1900, utc_time->tm_mon +1, utc_time->tm_mday,
        utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec);
        new_dir[strlen(new_dir)] = '\0';

        if(rename(game_file_dir, new_dir) == -1){
            perror("Erro ao mover ficheiro.\n");
        }
        free(new_dir);

    }

    
    free(answer);
    return;
}

void process_player_try(char *input, char **response, time_t current_seconds){
    //input e do tipo: TRY PLID C C C C nr_tries
    char buffer[64];
    strcpy(buffer, input);
    buffer[strlen(input)] = '\0';
    char *trial = malloc(5 * sizeof(char));
    char *ans_check = malloc(5 * sizeof(char));
    int plid = atoi(strtok(buffer, " "));
    strcpy(trial, strtok(NULL, " "));
    for (int i = 0; i < 3; i++){
        strcat(trial, strtok(NULL, " "));
    }
    
    int check = check_n_send_colors(trial);
    int nr_tries = atoi(strtok(NULL, " "));

    char game_file_dir[PLID_LEN + strlen(GAME_FILE_PREFIX) + strlen(GAME_DIR) +1];
    sprintf(game_file_dir, "%s%s%d",GAME_DIR, GAME_FILE_PREFIX, plid);
    game_file_dir[strlen(game_file_dir)] = '\0';

    //caso do NOK
    if (access(game_file_dir, F_OK) != 0) {
        *response = (char*) malloc(sizeof(char) * TRY_NOK_LEN);
        sprintf(*response, "%s %s\n", TRY_RESPONSE, NOK_STATUS);
        return;
    }

    FILE *file = fopen(game_file_dir, "a+"); //open file

    //caso do DUP
    int nr_real_tries = check_tries(file, trial);
    if (nr_real_tries == -1) {
        *response = (char*) malloc(sizeof(char) * TRY_DUP_LEN);
        sprintf(*response, "%s %s\n", TRY_RESPONSE, DUP_STATUS);
        return;
    }
    //caso do INV
    if (nr_real_tries != nr_tries) {
        *response = (char*) malloc(sizeof(char) * TRY_INV_LEN);
        sprintf(*response, "%s %s\n", TRY_RESPONSE, INV_STATUS);
        return;
    }
    //caso do ERR
    if (check == -1) {
        *response = (char*) malloc(sizeof(char) * TRY_ERR_LEN);
        sprintf(*response, "%s %s\n", TRY_RESPONSE, ERR_STATUS);
        return;
    }

    write_on_file_try(file, game_file_dir, trial, &ans_check, plid, current_seconds, nr_tries);

    //caso do ENT
    if (ans_check[0] != '4' && nr_tries == 8) {
        fseek(file, 8, SEEK_SET);

        char ans[8]; // 4 chars + 3 spaces + null terminator
        char* ans_in_file = malloc(4 * sizeof(char));
        fscanf(file, "%s", ans_in_file);
        printf("%s\n", ans_in_file);
        int i = 0;  // Index for 'result'
        for (int j = 0; j < 4; j++) { // Loop to read 4 characters
            ans[i++] = ans_in_file[j];               // Place the character in 'result'
            if (j < 3) {                    // Add a space after every character except the last one
                ans[i++] = ' ';
            }   
        }
        printf("%s\n", ans);
        *response = (char*) malloc(sizeof(char) * TRY_ENT_LEN);
        sprintf(*response, "%s %s %s\n", TRY_RESPONSE, ENT_STATUS, ans);
        printf("R: %s", *response);
        free(ans_in_file);
    }
    else {
        *response = (char*) malloc(sizeof(char) * TRY_OK_LEN);
        sprintf(*response, "%s %s %d %s\n", TRY_RESPONSE, OK_STATUS, nr_real_tries, ans_check);
        printf("R: %s", *response);
    }

    fclose(file);
    free(trial);
    free(ans_check);
}
/*
int main(){
    char *response;
    char *input = "TRY 107177 G P Y G 8\n";
    time_t current_seconds = time(NULL);
    process_player_try(file, input, &response, current_seconds);
    free(response);

}*/
