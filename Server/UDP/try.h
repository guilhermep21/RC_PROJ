#ifndef TRY_H
#define TRY_H



#define TRY_RESPONSE        "RTR"



#define TRY_ERR_LEN             7
#define TRY_OK_LEN              11
#define TRY_NOK_LEN             7
#define TRY_DUP_LEN             7
#define TRY_INV_LEN             7
#define TRY_ENT_LEN             15


void process_player_try(char *input, char **response, time_t current_seconds);


#endif // TRY_H
