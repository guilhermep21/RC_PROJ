#ifndef START_H
#define START_H

#define START_RESPONSE  "RSG"


#define START_ERR_LEN         7
#define START_OK_LEN          6
#define START_NOK_LEN         7

void process_player_start(char *input, char **response);

int create_user_gm_dir(char *userid);

#endif // START_H