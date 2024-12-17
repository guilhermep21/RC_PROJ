#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GAMES_DIR       "GAMES"
#define SCORES_DIR    "SCORES"

#define OK_STATUS       "OK"
#define NOK_STATUS      "NOK"
#define ERR_STATUS      "ERR"

#define ONGOING         "1"
#define STOPPED         "0"

#define ANSWER_LEN      4

#define GAME_FILE_LEN   7

#define SERVER_IP       "tejo.tecnico.ulisboa.pt"
#define DEFAULT_PORT    "58000"

#define LOCAL_SERVER_IP "localhost"

#define TRUE            1
#define FALSE           0

#define PLID_LEN        6
#define TRY_CMD_LEN     14

#define READ_WRITE_RATE 1024



typedef struct {
    char plid[PLID_LEN + 1];
    char answer[ANSWER_LEN + 1];
    char start_time[20];
    int time_active;
    int active;
    struct end {
        char end_time[20];
        int end_sec_time;
    } end;
} game;

typedef struct {
    char cmd[4];
    char* input;
} cmds;

#endif // CONSTANTS_H