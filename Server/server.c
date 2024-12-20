#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <bits/getopt_core.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include "server.h"
#include "UDP/try.h"
#include "UDP/start.h"
#include "UDP/quit.h"
#include "UDP/debug.h"

int aid;

int verbose = FALSE;
char *port = "58074";

// initialize variable in case of incomplete command

// create signal handler for SIGINT that closes sockets fds

int udp_sock = -1;
//int tcp_sock = -1;

void sigint_handler(int sig) {
    (void) sig;
    printf("Closing sockets\n");
    
    if (udp_sock != -1) {
        close(udp_sock);
    }
    /*
    if (tcp_sock != -1) {
        close(tcp_sock);
    }
    */
    exit(0);
}

void validate_args(int argc, char** argv) {

    int opt;

    while ((opt = getopt(argc, argv, "v:p:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = TRUE;
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
}

void check_UDP_command(cmds command, int fd, struct sockaddr_in addr, socklen_t addrlen) {
    char plid[7];
    memcpy(plid, command.input, 7);
    plid[7] = '\0';
    
    char* response = NULL;
    time_t current_seconds = time(NULL);


    printf("Command: %s, with len: %ld\n", command.cmd, strlen(command.cmd));

    if (strcmp(command.cmd, "SNG") == 0) {
        process_player_start(command.input, &response);
    } 
    
    else if (strcmp(command.cmd, "DBG") == 0){
        process_player_debug(command.input, &response);
    } 
    
    
    else if (strcmp(command.cmd, "TRY") == 0) {
        process_player_try(command.input, &response, current_seconds);
    } 
    
    else if (strcmp(command.cmd, "QUT") == 0) {
        process_player_quit(command.input, &response, current_seconds);
    } 
    
    else {
        printf("Invalid command\n");
    }
    
    printf("UDP response: %s\n", response);

    char* response_ptr = response;
    int response_len = strlen(response);
    int bytes_sent = 0;

    while (bytes_sent < response_len) {
        int current_chunk_size = response_len - bytes_sent < READ_WRITE_RATE ? response_len - bytes_sent : READ_WRITE_RATE;
        int n = sendto(fd, response_ptr, current_chunk_size, 0, (struct sockaddr*) &addr, addrlen);
        if (n == -1) {
            perror("Error sending response");
        }
        bytes_sent += n;
        response_ptr += n;
    }

    if (response != NULL)
        free(response);
}

int create_udp_socket() {
    int fd, errcode;
    ssize_t n;
    struct addrinfo hints, *res;

    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket

    if (fd == -1) {
        printf("Socket error.\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    hints.ai_flags = AI_PASSIVE;

    errcode = getaddrinfo(NULL, port, &hints, &res);

    if (errcode != 0) {
        perror("Address error.");
        exit(1);
    }

    n = bind(fd, res->ai_addr, res->ai_addrlen);

    if (n == -1) {
        perror("Bind error.");
        exit(1);
    }

    return fd;
}

void read_udp_socket(int fd) {
    
    ssize_t n;
    char buffer[128];
    struct sockaddr_in addr;
    socklen_t addrlen;
    addrlen = sizeof(addr);
    cmds command;

    n = recvfrom(fd, buffer, 128, 0, (struct sockaddr *)&addr, &addrlen);
    buffer[n] = '\0';
    buffer[n - 1] = '\0';
    printf("buffer: %s\n", buffer);


    if (n == -1) {
        perror("Error receiving UDP data");
        exit(1);
    }

    printf("Received UDP data from %s:%d\n",
                    inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    memcpy(command.cmd, buffer, 4); 
    command.cmd[3] = '\0'; 

    size_t input_length = strlen(buffer) - 4;
    command.input = (char* ) malloc((input_length + 1) * sizeof(char)); // Allocate memory for input
    if (command.input == NULL) {
        printf("Error allocating memory for input\n");
        exit(1);
    }

    memcpy(command.input, buffer + 4, input_length); 
    command.input[input_length] = '\0';

    // process commands here
    check_UDP_command(command, fd, addr, addrlen);    

    printf("Finished processing UDP command\n");
    free(command.input);
}

/*
void check_TCP_command(char *command, int fd){

    char* response = NULL;
    
    if (strcmp(command, "STR") == 0){
        process_show__trials(fd,  &aid, &response);
    } else if (strcmp(command, "SSB") == 0){
        process_scoreboard(fd, &response);
    }

    // send response to client through TCP socket
    if (strcmp(command, "SAS") != 0){
        printf("TCP response: %s", response);
        char* response_ptr = response;
        int response_len = strlen(response);
        int bytes_sent = 0;

        while (bytes_sent < response_len) {
            int current_chunk_size = response_len - bytes_sent < READ_WRITE_RATE ? response_len - bytes_sent : READ_WRITE_RATE;
            int n = write(fd, response_ptr, current_chunk_size);
            if (n == -1){
                perror("Error sending response");
            }
            bytes_sent += n;
            response_ptr += n;
        }

        if (response != NULL)
            free(response);

    }   
}


int create_tcp_socket(){

    int errcode;
    struct addrinfo hints, *res;
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        printf("Error creating TCP socket\n");
        exit(1);
    }

    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0 ||
        setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0) {
        printf("Error setting socket options.\n");
        return -1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM;   // TCP socket
    hints.ai_flags = AI_PASSIVE;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    errcode = getaddrinfo(NULL, port, &hints, &res);
    if ((errcode) != 0){
        //error
        fprintf(stderr, "Error getting TCP address info\n");
        exit(1);
    }

    ssize_t n = bind(fd,res->ai_addr,res->ai_addrlen);

    if (n == -1){
        //error
        fprintf(stderr, "Error binding TCP socket\n");
        exit(1);
    }

    if (listen(fd, 20) == -1){
        //error
        fprintf(stderr, "Error listening TCP socket\n");
        exit(1);
    }

    return fd;
}


void read_tcp_socket(int fd){

    // read from TCP socket first 3 bytes to get command

    char command[4];
    ssize_t n = read_word(fd, command, 4);
    if (n == -1) {
        //error
        fprintf(stderr, "Error reading from TCP socket\n");
        exit(1);
    }

    printf("command: %s\n", command);

    check_TCP_command(command, fd);
}
*/

/**
  * 
  * Main function
  * 
  */
int main(int argc, char** argv){

    // validate arguments
    validate_args(argc, argv);    

    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        return 1;
    }

    udp_sock = create_udp_socket();
    printf("udp_sock: %d\n", udp_sock);

    //tcp_sock = create_tcp_socket();
    //printf("tcp_sock: %d\n", tcp_sock);

    //aid = get_global_aid_number();
    //printf("aid: %d\n", aid);

    int maxfd;
    fd_set activefds;

    while (TRUE) {

        FD_ZERO(&activefds);
        FD_SET(udp_sock, &activefds);
        //FD_SET(tcp_sock, &activefds);

        fd_set readfds = activefds;

        maxfd = udp_sock;
        //maxfd = udp_sock > tcp_sock ? udp_sock : tcp_sock;

        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(udp_sock, &readfds)) {
            read_udp_socket(udp_sock);
        }
        /*
        if (FD_ISSET(tcp_sock, &readfds)) {

            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);

            int tcp_client_socket = accept(tcp_sock, (struct sockaddr*)&client_addr, &addr_len);
            printf("Accepted connection from TCP client with fd: %d\n", tcp_client_socket);

            if (tcp_client_socket == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            read_tcp_socket(tcp_client_socket);
            
            printf("Shutting down TCP client socket\n");
            if (shutdown(tcp_client_socket, SHUT_WR) == -1) {
                perror("shutdown");
                exit(EXIT_FAILURE);
            }

            // Close the TCP client socket when done
            printf("Closing TCP client socket\n");
            close(tcp_client_socket);
            
        }*/
    }

    // Close sockets
    //close(tcp_sock);
    close(udp_sock);

    return 0;
}