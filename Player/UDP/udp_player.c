#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>




const char* UDP_commands[] = {
   "start",
   "try",
   "quit",
   "exit",
   "debug"
};

int UDP_cmd(char* cmd){
    int i;
    for (i = 0; i < 5; i++) {
        if (strcmp(cmd, UDP_commands[i]) == 0)
            return 1;
    }
    return 0;
}

int send_UDP(char *msg, char** udp_buffer, char* ip, char* port) {
    
    int fd, errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    (*udp_buffer) = (char*) malloc(5003 * sizeof(char));
    
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (fd == -1) {
        perror("socket");
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    printf("trying to connect to %s:%s\n", ip, port);

    errcode = getaddrinfo(ip, port, &hints, &res);
    if (errcode != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    printf("%s \n", msg);
    n = sendto(fd, msg, strlen(msg), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        perror("Error sending message to server\n");
        exit(1);
    }

    addrlen = sizeof(addr);
    n = recvfrom(fd, *udp_buffer, 5003, 0, (struct sockaddr*)&addr, &addrlen);
    if (n == -1) {
        perror("recvfrom");
        exit(1);
    }
    
    printf("Received %ld bytes\n", n);
    
    (*udp_buffer)[n] = '\0';
    printf("%s", *udp_buffer);

    freeaddrinfo(res);
    close(fd);

    return 0;
}


