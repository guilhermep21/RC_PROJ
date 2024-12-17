#ifndef QUIT_H
#define QUIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../player.h"

#define QUIT_LEN 10
#define QUIT_CMD "QUT"

int process_quit(player** user, char **response);

#endif // QUIT_H