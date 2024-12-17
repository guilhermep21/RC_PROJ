#ifndef QUIT_H
#define QUIT_H

#define QUIT_RESPONSE        "QUT"
#define ERR_STATUS          "ERR"
#define OK_STATUS           "OK"
#define NOK_STATUS          "NOK"
#define DUP_STATUS          "DUP"
#define INV_STATUS          "INV"
#define GAME_FILE_PREFIX    "GAME_"
#define GAME_DIR            "../GAMES/"
#define GAME_FILE_LEN       11
#define GAME_DIR_LEN        9

#define ERR_LEN             7
#define OK_LEN              10
#define NOK_LEN             7
#define DUP_LEN             7
#define INV_LEN             7


void process_player_quit(char *input, char **response, time_t current_seconds);


#endif // QUIT_H