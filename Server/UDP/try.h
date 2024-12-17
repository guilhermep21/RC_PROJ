#ifndef TRY_H
#define TRY_H



#define TRY_RESPONSE        "RTR"
#define ERR_STATUS          "ERR"
#define OK_STATUS           "OK"
#define NOK_STATUS          "NOK"
#define DUP_STATUS          "DUP"
#define INV_STATUS          "INV"
#define ENT_STATUS          "ENT"
#define GAME_FILE_PREFIX    "GAME_"
#define GAME_DIR            "../GAMES/"
#define GAME_FILE_LEN       11
#define GAME_DIR_LEN        9

#define ERR_LEN             7
#define OK_LEN              10
#define NOK_LEN             7
#define DUP_LEN             7
#define INV_LEN             7
#define ENT_LEN             15


void process_player_try(char *input, char **response, time_t current_seconds);


#endif // TRY_H
