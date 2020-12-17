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
    else{
        return;
    }
}

int main(int argc, char *argv[]){
    int serv_sock;
    char message[BUF_SIZE];
    int str_len, temp;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    
    Help(argc, 1, "UDPEchoServ [port]");
    fputs("Starting server...", stdout);

    serv_sock = socket(PF_INET,SOCK_DGRAM,0);
    Error(serv_sock, "socket() error");

    memset(&serv_addr, 0, sizeof(serv_sock));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    temp = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    Error(temp, "bind() error");
    
    while(1){
        clnt_addr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
                           (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("\nEchoing: %s", message);
        sendto(serv_sock, message, str_len,0,
               (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
    }
    close(serv_sock);
    return 0;
    
}












