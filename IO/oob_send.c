#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>



#define BUF_SIZE 100
#define TRUE 1
#define FALSE 0

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
    struct sockaddr_in recv_addr;
    socklen_t recv_addr_size;
    int result;

    
    Help(argc, 2, "OOBSend [ip] [port]");

    sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(sock, "socket() error");

    recv_addr_size = sizeof(recv_addr);
    memset(&recv_addr, 0, recv_addr_size);
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_addr.sin_port = htons(atoi(argv[2]));

    result = connect(sock, (struct sockaddr*)&recv_addr, recv_addr_size);
    Error(result, "connect() error");

    write(sock, "  message1  \n", strlen("message1"));
    send(sock, "oob_message2  \n", strlen("message2"), MSG_OOB);
    write(sock, "  message3  \n", strlen("message3"));
    send(sock, "oob_message4\n", strlen("message4"), MSG_OOB);
    write(sock, "  message5  ", strlen("message5"));    
        
    return 0;
}




















