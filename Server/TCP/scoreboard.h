#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#define SCOREBOARD_RESPONSE     "RSS"

#define EMPTY_STATUS            "EMPTY"

#define RSS_OK_LEN              22 // missing the Fdata
#define RSS_EMPTY_LEN           9


#define MODE_PLAY 1
#define MODE_DEBUG 2

typedef struct {
    int score[10];          
    char PLID[10][7];       
    char col_code[10][5];   
    int no_tries[10];        
    int mode[10];            
    int n_scores;            
} SCORELIST;


void process_player_scoreboard(char **response);


#endif // SCOREBOARD_H