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
    struct sockaddr_in send_addr;
    char message[BUF_SIZE];
    int msg_len;
        
    Help(argc, 2, "Sendhost [ip] [port]");

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    Error(sock, "socket() error");

    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_addr.s_addr = inet_addr(argv[1]);
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(atoi(argv[2]));

    while(1){
        printf("Send: ");
        scanf("%s", message);
        msg_len = sizeof(message);
        if(!strcmp(message, "q")){
            break;
        }
        
        sendto(sock, message, msg_len, 0,
               (struct sockaddr*)&send_addr, sizeof(send_addr));
    }

    close(sock);
    return 0;
}

