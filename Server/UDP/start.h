#ifndef START_H
#define START_H

#define START_RESPONSE  "RSG"
#define ERR_STATUS "ERR"
#define OK_STATUS "OK"
#define NOK_STATUS "NOK"
#define GAME_FILE_PREFIX "GAME_"
#define GAME_DIR "../GAMES/"

#define ERR_LEN         7
#define OK_LEN          6
#define NOK_LEN         7

void process_player_start(char *input, char **response);


#endif // START_H