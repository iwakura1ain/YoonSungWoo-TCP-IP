#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024


void Error(int error, char *message){
    if(error == -1){
        fputs(message, stdout);
        exit(1);
    }
}

void Help(int argc, int count, char *message){
    if(argc-1 != count){
        fputs(message, stdout);
        exit(1);
    }
}

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr, from_addr;
    char message[BUF_SIZE];
    int str_len, temp_len;
    socklen_t from_addr_size;
    
    Help(argc, 2, "UDPEchoClnt [ip] [port]");

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    Error(sock, "socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    while(1){
        printf("Input: ");
        scanf("%s", message);
        temp_len = sizeof(message);
        if(!strcmp(message, "q")){
            break;
        }
        sendto(sock, message, temp_len, 0,
               (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        from_addr_size = sizeof(from_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, //(struct sockaddr*)&serv_addr, sizeof(serv_addr));
                           (struct sockaddr*)&from_addr, &from_addr_size);
        message[str_len] = 0;
        printf("\nEcho from server: %s", message);
    }

    close(sock);
    return 0;
}

