#ifndef CONSTANTS_H
#define CONSTANTS_H

#define OK_STATUS           "OK"
#define NOK_STATUS          "NOK"
#define ERR_STATUS          "ERR"
#define DUP_STATUS          "DUP"
#define INV_STATUS          "INV"
#define ENT_STATUS          "ENT"


#define ONGOING             "1"
#define STOPPED             "0"

#define ANSWER_LEN          4

#define SERVER_IP           "tejo.tecnico.ulisboa.pt"
#define DEFAULT_PORT        "58000"

#define LOCAL_SERVER_IP     "localhost"

#define TRUE                1
#define FALSE               0

#define PLID_LEN            6
#define TRY_CMD_LEN         14

#define FILE_NAME_MAX_LEN   24

#define GAME_FILE_PREFIX    "GAME_"
#define GAME_DIR            "GAMES/"

#define SCORE_DIR           "SCORES/"

#define SCORE_FILE_DIR_LEN  33

#define GAME_FILE_LEN       11
#define GAME_DIR_LEN        6

#define READ_WRITE_RATE     1024



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