#ifndef START_H
#define START_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../player.h"


#define START_CMD "SNG"
#define START_LEN 14

int process_start(char* input, player** user, char** response);

#endif // START_H