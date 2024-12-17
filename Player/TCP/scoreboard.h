
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#define SCOREBOARD_LEN 3

#define SCOREBOARD_CMD "SSB"

#include "../player.h"

#include "tcp_player.h"


int process_scoreboard(TCP_response** response);

#endif  // SCOREBOARD_H