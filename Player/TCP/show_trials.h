
#ifndef SHOW_TRIALS_H
#define SHOW_TRIALS_H

#define SHOW_TRIALS_LEN 10

#define SHOW_TRIALS_CMD "STR"

#include "../player.h"

#include "tcp_player.h"


int process_show_trials(player* user, TCP_response** response);

#endif  // SHOW_TRIALS_H