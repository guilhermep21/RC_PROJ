#ifndef SHOW_TRIALS_H
#define SHOW_TRIALS_H

#define SHOW_TRIALS_RESPONSE    "RST"

#define ACT_STATUS              "ACT"
#define FIN_STATUS              "FIN"

#define RST_ACT_LEN             8 // missing the Fname, Fsize and Fdata
#define RST_FIN_LEN             7
#define RST_NOK_LEN             7


void process_player_show_trials(char *input, char **response);


#endif // SHOW_TRIALS_H