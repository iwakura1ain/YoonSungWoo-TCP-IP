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
    int sock;
    char message[BUF_SIZE];
    int str_len, result;
    int i, j;
    struct sockaddr_in my_addr, recv_addr;
    socklen_t recv_addr_size;

    
    Help(argc, 1, "Servhost [port]");
    fputs("Starting server...", stdout);

    sock = socket(PF_INET,SOCK_DGRAM,0);
    Error(sock, "socket() error");

    memset(&my_addr, 0, sizeof(sock));
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1]));

    result = bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr));
    Error(result, "bind() error");

    recv_addr_size = sizeof(recv_addr);
    for(i=0;i<3;i++){
        str_len = recvfrom(sock, message, BUF_SIZE-1, 0,
                 (struct sockaddr *)&recv_addr, &recv_addr_size);
        
        printf("Message: %s\n", message);
        sleep(5);
    }
    
    close(sock);
    return 0;
    
}












