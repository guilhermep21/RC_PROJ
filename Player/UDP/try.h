#ifndef TRY_H
#define TRY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../player.h"

#define TRY_CMD "TRY"
#define TRY_LEN 11

int process_try(char* input, player **user, char** response);

#endif // TRY_H