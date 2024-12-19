#ifndef QUIT_H
#define QUIT_H

#define QUIT_RESPONSE        "QUT"


#define QUT_ERR_LEN             7
#define QUT_OK_LEN              10
#define QUT_NOK_LEN             7
#define QUT_DUP_LEN             7
#define QUT_INV_LEN             7


void process_player_quit(char *input, char **response, time_t current_seconds);


#endif // QUIT_H