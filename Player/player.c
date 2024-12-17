#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <bits/getopt_core.h>
#include "player.h"
#include "UDP/start.h"
#include "UDP/try.h"
#include "UDP/udp_player.h"
#include "TCP/scoreboard.h"
#include "TCP/show_trials.h"
#include "TCP/tcp_player.h"


player *user;
char *ip = "tejo.tecnico.ulisboa.pt";
char *port = "58011";


void validate_args(int argc, char** argv) {

    int opt;

    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                ip = (char *) malloc(sizeof(char) * strlen(optarg));
                ip = optarg;
                printf("ip: %s\n", ip);
                break;
            case 'p':
                port = optarg;
                printf("port: %s\n", port);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n ip] [-p port]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argc == 1)
        printf("ip: %s\nport: %s\n", ip, port);
}

void UDP_response(char response[TRANSMISSION_RATE], player** user){
    char* token;
    char response_copy[TRANSMISSION_RATE];
    strcpy(response_copy, response);

    token = strtok(response_copy, " ");

    // START RESP
    if (strcmp(token, "RSG") == 0){
        token = strtok(NULL, " ");
        token[strlen(token) - 1] = '\0'; // remove \n from token
        if (strcmp(token, "NOK") == 0)
            printf("You are already in game\n");
        else if(strcmp(token, "OK") == 0)
            printf("New game started (max %d sec)\n", (*user)->player_time);
        else
            printf("Invalid input \n");  
    }
    // TRY RESP
    else if(strcmp(token, "RTR") == 0){
        token = strtok(NULL, " ");
        token[strlen(token)] = '\0';
        printf("%s\n", token);
        if(strcmp(token, "OK") == 0){
            (*user)->nT += 1;
            int n_trials = atoi(strtok(NULL, " "));
            int nB = atoi(strtok(NULL, " "));
            int nW = atoi(strtok(NULL, " "));
            if(nB == 4)
                printf("WELL DONE! You guessed in %d trials\n", n_trials);
            else    
                printf("nB = %d, nW = %d\n", nB, nW);
        }
        else if(strcmp(token, "DUP") == 0)
            printf("Already made this guess, try another.\n");
        else if(strcmp(token,"INV") == 0)
            printf("Invalid try.\n");
        else if(strcmp(token, "NOK") == 0)
            printf("You have to start a game first.\n");
        else if(strcmp(token, "ENT") == 0)
            printf("Game Over!\nYou have no more tries available!\n");
        else if(strcmp(token, "ETM") == 0)
            printf("Game Over!\nYour time is up!\n");
        else
            printf("Invalid try.\n");
    }
    // QUIT RESP
    else if(strcmp(token, "RQT") == 0){
        token = strtok(NULL, " ");
        token[strlen(token)] = '\0';
        if(strcmp(token, "OK") == 0){
            char code[7];
            strcpy(code, strtok(NULL, " "));
            strcat(code, " ");
            for (int i = 0; i < 2; i++){
                strcat(code, strtok(NULL, " "));
                strcat(code, " ");
            }
            strcat(code, strtok(NULL, " "));
            printf("You quit your game!\nThe secret code was %s\n", code);
        }
        else if(strcmp(token, "NOK") == 0)
            printf("No ongoing game currently\n");
        else
            printf("Unable to quit right now\n");
    }
    else if(strcmp(token, "DBG") == 0){
        token = strtok(NULL, " ");
        token[strlen(token)] = '\0';
        if(strcmp(token, "OK") == 0)
            printf("Debug session started.\n");
        else if(strcmp(token, "NOK") == 0)
            printf("You have an ongoing game.\n");
        else
            printf("[DEBUG]: Invalid debug.\n");        
    }    
    else
        printf("Invalid UDP_response\n");
}


void check_UDP_cmd(char* input, char* cmd){

    char* request = NULL;
    char* udp_buffer = NULL;

    if (strcmp(cmd, "start") == 0){
        if(process_start(input, &user, &request) == -1)
            fprintf(stderr, "[ERROR]: start\n");
    }

    else if (strcmp(cmd, "try") == 0){
        if(process_try(input,&user, &request) == -1)
            fprintf(stderr, "[ERROR]: try\n");
    }        

    else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0){
        if(process_quit(&user, &request) == -1)
            fprintf(stderr, "[ERROR]: quit\n");
    }        
        

    else if (strcmp(cmd, "debug") == 0){
        if(process_debug(input, &user, &request) == -1)
            fprintf(stderr, "[ERROR]: debug\n");
    }
    else
        printf("nao implementado\n");


    if (request != NULL){
        if (send_UDP(request, &udp_buffer, ip, port) == -1){
            printf("error: server did not respond\n");
            
            free(request);
            
            return;
        }

        UDP_response(udp_buffer, &user);    
    }  
}

void check_TCP_cmd(char* cmd){
    TCP_response* request;
    request = (TCP_response *)malloc(sizeof(TCP_response));

    if(strcmp(cmd, "show_trials") == 0 || strcmp(cmd, "st") == 0){
        if(process_show_trials(user, &request) == -1)
            printf("error: show_trials\n");
    }        

    else if(strcmp(cmd, "scoreboard") == 0 || strcmp(cmd, "sb") == 0){
        if(process_scoreboard(&request) == -1)
            printf("error: scoreboard\n");
    }            

    if(request->msg != NULL){
        send_TCP(request, ip, port);
        free(request->msg);
    }  
    free(request);          
}

void process_cmd(char* input){ 

    char *cmd;

    char input_copy[MAX_COMMAND_LEN];

    strcpy(input_copy, input);

    cmd = strtok(input, " ");

    if (cmd[strlen(cmd)] == '\n')
        cmd[strlen(cmd)] = '\0';

    if (UDP_cmd(cmd))
        check_UDP_cmd(input_copy, cmd);
    else if(TCP_cmd(cmd))
        check_TCP_cmd(cmd); 
    else
        fprintf(stderr, "[ERROR]: unkown_command\n");
}

int main(int argc, char** argv) {

    // check if command is complete

    validate_args(argc, argv);

    // initialize client, uid and pwd
    user = (player*) malloc(sizeof(player));
    
    while (TRUE) {

        char input[MAX_COMMAND_LEN];

        memset(input, '\0', MAX_COMMAND_LEN - 1);

        if (fgets(input, MAX_COMMAND_LEN, stdin) == NULL) { 
            printf("é aqui no fgets\n");
            // No valid command configuration can be longer than 128 characters so this shouldn't be an issue.
            ferror(stdin);
            exit(-1);
        }
        if (input[0] == '\n') { // If the user only pressed enter.
            continue;
        }

        int n_char = strcspn(input, "\n");

        if (input[n_char] == '\n') {
            input[n_char] = '\0';
        }
        // see which command was inputed
        process_cmd(input);

    }

    return 0;
}