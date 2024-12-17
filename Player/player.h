#ifndef PLAYER_H
#define PLAYER_H

typedef struct player {
    int uid;
    int player_time;
    int nT;
} player;

#define LEN_PLID 6
#define TRUE 1
#define FALSE 0
#define MAX_COMMAND_LEN 64
#define TRANSMISSION_RATE 800

#define MAX_TCP_NULL_FILE 30

#define MAX_TCP_NULL_FILERCV 8

#define MAX_FILENAME_LEN 24

#endif // PLAYER_H




