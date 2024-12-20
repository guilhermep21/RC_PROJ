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
#include "scoreboard.h"
#include <dirent.h>

int FindTopScores(SCORELIST *list)
{
    struct dirent **filelist;
    int n_entries, i_file;
    char fname[300];
    FILE *fp;
    char mode[8];
    char buffer[24];

    n_entries = scandir("SCORES/", &filelist, 0, alphasort);

    if (n_entries <= 0)
        return (0);
    else
    {
        i_file = 0;
        while (n_entries--)
        {
            if (filelist[n_entries]->d_name[0] != '.' && i_file < 10)
            {
                sprintf(fname, "SCORES/%s", filelist[n_entries]->d_name);
                fp = fopen(fname, "r");
                if (fp != NULL)
                {
                    fgets(buffer, 24, fp);
                    list->score[i_file] = atoi(strtok(buffer, " "));
                    strcpy(list->PLID[i_file], strtok(NULL, " "));
                    strcpy(list->col_code[i_file], strtok(NULL, " "));
                    list->no_tries[i_file] = atoi(strtok(NULL, " "));
                    strcpy(mode, strtok(NULL, " "));
                    if (!strcmp(mode, "PLAY"))
                        list->mode[i_file] = MODE_PLAY;
                    if (!strcmp(mode, "DEBUG"))
                        list->mode[i_file] = MODE_DEBUG;

                    fclose(fp);
                    ++i_file;
                }
            }
            free(filelist[n_entries]);
        }
        free(filelist);
    }

    list->n_scores = i_file;
    return (i_file);
}

void process_player_scoreboard(char **response){
    SCORELIST *list = malloc(sizeof(SCORELIST));
    int n = FindTopScores(list);
    if (n == 0) {
        *response = (char*) malloc(sizeof(char) * RSS_EMPTY_LEN);
        sprintf((*response), "%s %s\n", SCOREBOARD_RESPONSE, EMPTY_STATUS);
        return;
    } else {
        int i = 0;
        int fsize = n * 14;
        *response = (char*) malloc(sizeof(char) * (RSS_OK_LEN + fsize));

        char fname[10] = "scoreboard";
        char fsize_str[4];
        sprintf(fsize_str, "%d", fsize);
        sprintf((*response), "%s %s %s %d ", SCOREBOARD_RESPONSE, OK_STATUS, fname, fsize);
        
        while(TRUE) {
            if (i == 10 || list->score[i] == 0)
                break;

            char score_str[2];
            sprintf(score_str, "%d", list->score[i]);
            strcat(*response, score_str);
            strcat(*response, " ");
            strcat(*response, list->PLID[i]);
            strcat(*response, " ");
            strcat(*response, list->col_code[i]);
            strcat(*response, " ");

            char no_tries_str[2];
            sprintf(no_tries_str, "%d", list->no_tries[i]);
            strcat(*response, no_tries_str);
            strcat(*response, "\n");
            i++;
        }
        strcat(*response, "\0");  
    }   
    free(list);
}
/*
int main() {
    char *response;
    process_player_scoreboard(&response);
    printf("%s\n", response);
    free(response);
}*/