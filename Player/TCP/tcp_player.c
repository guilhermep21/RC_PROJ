#include "tcp_player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../player.h"




int fd, tcp_errcode;
ssize_t tcp_n;
socklen_t addrlen;
struct addrinfo tcp_hints, *tcp_res;
struct sockaddr_in addr;
//char buffer[TRANSMISSION_RATE];
char* buffer;

const char *TCP_commands[] = {
    "show_trials",
    "scoreboard",
    "st",
    "sb"
};

int TCP_cmd(char* cmd){
    int i;
    for (i = 0; i < 4; i++) {
        if (strcmp(cmd, TCP_commands[i]) == 0)
            return 1;
    }
    return 0;
}

int read_word(int fd, char *buffer, int size) {
    int i = 0;
    while (i < size)
    {
        if (read(fd, buffer + i, sizeof(char)) == -1)
        {
            printf("Error while reading from socket.\n");
            buffer = NULL;
            return -1;
        }
        if (buffer[i] == ' ' || buffer[i] == '\n')
        {
            memset(buffer + i, '\0', 1);
            //printf("read_word: %s\n", buffer);
            break;
        }
        i++;
    }

    return 0;
}

int null_file_message(int fd, char* msg){

    printf("msg: %s, with len: %ld\n", msg, strlen(msg));

    if (send(fd, msg , strlen(msg), 0) == -1) {
        /*error*/
        fprintf(stderr, "Error sending message to server\n");
        return -1;
    }

    char buffer[MAX_TCP_NULL_FILERCV] ; // Buffer to store received data

    if ((tcp_n = read(fd, buffer, MAX_TCP_NULL_FILERCV)) == -1) {
        /*error*/
        fprintf(stderr, "Error receiving message to server\n"); 
        return -1;
    } 

    printf("received %ld bytes\n", tcp_n);

    printf("received message from server: %s\n", buffer);

    return 1;
}

int open_auction_message(int fd, char* filename, char* msg){

    struct stat st;
    size_t filesize;

    if (stat(filename, &st) == 0) {
        filesize = st.st_size;
    } else {
        fprintf(stderr, "Error getting file size\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    char filesize_str[10];
    sprintf(filesize_str, "%ld", filesize);

    strcat(msg, filesize_str);
    strcat(msg, " ");
    msg[strlen(msg)] = '\0';

    printf("msg: %s|\n", msg);

    // Send the file data

    if ((write(fd, msg, strlen(msg))) == -1) {
        fprintf(stderr, "Error sending data\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    ssize_t bytes_sent;
    size_t total_bytes_sent = 0;

    printf("sending file...\n");

    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    while ((tcp_n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        printf("reading more data...\n");
        if ((bytes_sent = write(fd, buffer, tcp_n)) == -1) {
            fprintf(stderr, "Error sending data\n");
            fclose(file);
            close(fd);
            return -1;
        }
        printf("wrote %ld bytes\n", bytes_sent);
        total_bytes_sent += bytes_sent;
    }

    printf("total_bytes_sent: %ld\n", total_bytes_sent);

    ssize_t bytes_read;
    if ((bytes_read = read(fd, buffer, filesize)) == -1) {
        /*error*/
        fprintf(stderr, "Error receiving message from server\n");
        return -1;
    } 

    printf("received %ld bytes\n", bytes_read);
    buffer[bytes_read] = '\0';
    printf("received message from server: %s\n", buffer);

    printf("closing file\n");
    // Close the file
    fclose(file);
    return 1;
}

int receive_asset_message(int fd, char* msg){
    char cmd[3];
    
    if ((send(fd, msg , strlen(msg), 0)) == -1) {
        /* error */
        fprintf(stderr, "Error sending message to server\n");
        return -1;
    }
     //printf("msg: %s, with len: %ld\n", msg, strlen(msg));

    // check if file exists by checking the status of the received message

    char response[4];
    read_word(fd, response, 4);

    //printf("response: %s\n", response);

    char status[4];
    read_word(fd, status, 4);

    //printf("status: %s\n", status);

    if (strcmp(status, "NOK") == 0) {
        fprintf(stderr, "Error: server could not complete request\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    //printf("reading filename and filesize...\n");

    char fname[MAX_FILENAME_LEN + 1];
    read_word(fd, fname, MAX_FILENAME_LEN);
    //printf("fname: %s\n", fname);

    char filesize_str[10];
    read_word(fd, filesize_str, 10);
    //printf("filesize_str: %s\n", filesize_str);
    int filesize = atoi(filesize_str);

    buffer = (char*)malloc((sizeof(char) * filesize)+1);
    
    strcpy(cmd, strtok(msg, " "));

    if (cmd[strlen(cmd) - 1] == '\n')
        cmd[strlen(cmd) - 1] = '\0';

    printf("cmd: %s\n", cmd);
    if(strcmp(cmd, "SSB") == 0){
        printf("response save as %s (%s bytes) \n", fname, filesize_str);
    }    
    else if(strcmp(cmd, "STR") == 0){
        printf("received trials file: %s (%s bytes) \n", fname, filesize_str); 
    }    

    FILE *file = fopen(fname, "wb");

    if (!file) {
        fprintf(stderr, "Error opening file\n");
        freeaddrinfo(tcp_res);
        close(fd);
        return -1;
    }

    int total_bytes_received = 0;
    int bytes_received;


    while ( (size_t) total_bytes_received < atoi(filesize_str)) {
        
        bytes_received = read(fd, buffer, TRANSMISSION_RATE);
        
        if (bytes_received < 0){
            perror("Error reading from TCP socket");
            fprintf(stderr, "Error reading from TCP socket\n");
            fclose(file);
            close(fd);
            return -1;
        }
        printf("%s\n", buffer);
        //printf("\n");
        //printf("received %d bytes\n", bytes_received);
        //printf("\n");

        total_bytes_received += bytes_received;

        //printf("total_bytes_received: %d\n", total_bytes_received);

        if (fwrite(buffer, 1 , bytes_received, file) != (size_t) bytes_received){
            fprintf(stderr, "Error writing file\n");
            fclose(file);
            close(fd);
            return -1;
        
        } // Write the received bytes to the file
        
    }

    printf("\n");
    //printf("received %d bytes\n", total_bytes_received);
    

    fclose(file);

    //fprintf(stdout, "File %s received\n", fname);
    return 1;
}  

int send_TCP(TCP_response* response, char* ip, char* port) {

    /* Cria um socket TCP (SOCK_STREAM) para IPv4 (AF_INET).
    É devolvido um descritor de ficheiro (fd) para onde se deve comunicar. */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        fprintf(stderr, "Error creating socket\n");
        exit(1);
    }

    memset(&tcp_hints, 0, sizeof tcp_hints);
    tcp_hints.ai_family = AF_INET;
    tcp_hints.ai_socktype = SOCK_STREAM; // TCP socket

    tcp_errcode = getaddrinfo(ip, port, &tcp_hints, &tcp_res);
    if (tcp_errcode != 0) {
        fprintf(stderr, "error sending message to server\n");
        return -1;
    }

    /* Em TCP é necessário estabelecer uma ligação com o servidor primeiro (Handshake).
       Então primeiro cria a conexão para o endereço obtido através de `getaddrinfo()`. */
    tcp_n = connect(fd, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (tcp_n == -1) {
        fprintf(stderr, "Error connecting to server\n");
        return -1;
    }

    if (response->file == NULL) {
        if (null_file_message(fd, response->msg) == -1){
            fprintf(stderr, "[ERROR]: null file message\n");
            return -1;
        }
    }

    else if (response->file->code == FILE_TO_BE_SENT) {
        /*LER E ENVIAR FICHEIRO PARA O SERVIDOR*/
        if (open_auction_message(fd, response->file->filename, response->msg) == -1){
            fprintf(stderr, "[ERROR]: open auction message\n");
            return -1;
        }
    }
    else if(response->file->code == FILE_TO_BE_RECEIVED){
        //RECEBER FICHEIRO DO SERVIDOR E ESCREVER PARA O DISCO
        if(receive_asset_message(fd, response->msg) == -1){
            fprintf(stderr, "[ERROR]: receive message.\n");
        }
    }
    free(buffer);
    freeaddrinfo(tcp_res);
    close(fd);

    return 1;
}



