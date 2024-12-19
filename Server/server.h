#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <time.h>

#define USERS_DIR       "USERS"
#define AUCTIONS_DIR    "AUCTIONS"

#define OK_STATUS       "OK"
#define NOK_STATUS      "NOK"
#define ERR_STATUS      "ERR"

#define HOSTED          "HOSTED"
#define BIDDED          "BIDDED"

#define BIDS            "BIDS"
#define ASSET           "ASSET"


#define PWD_SUFFIX      "_password.txt"
#define LOGIN_SUFFIX    "_login.txt"
#define TXT_SUFFIX      ".txt"
#define END_PREFIX      "END_"
#define START_PREFIX    "START_"

#define ACTIVE           "1"
#define NOT_ACTIVE       "0"

#define AUCTION_FILE_LEN 7

#define SERVER_IP       "tejo.tecnico.ulisboa.pt"
#define DEFAULT_PORT    "58090"

#define LOCAL_SERVER_IP "localhost"

#define TRUE            1
#define FALSE           0

#define UID_LEN         6
#define PWD_LEN         8

#define MAX_NAME        10

#define MAX_START_LEN   6
#define MAX_DUR_LEN     5
#define MAX_BIDDING_LEN 6

#define AID_LEN         3

#define MAX_FNAME_LEN   24

#define READ_WRITE_RATE 1024

#define YES 1
#define NO 0


typedef struct client {
    char* uid;
    char* pwd;
} client;

typedef struct {
    char cmd[4];
    char* input;
} cmds;

#endif // CONSTANTS_H