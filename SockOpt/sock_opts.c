#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
    int sock1, sock2;
    int sock1_type, sock2_type;
    int sock1_rcvbuf, sock1_sndbuf;
    int sock2_rcvbuf, sock2_sndbuf;
    socklen_t sock1_len, sock2_len;
    int check;

    sock1 = socket(PF_INET, SOCK_STREAM, 0);
    sock1_len = sizeof(sock1);
    sock2 = socket(PF_INET, SOCK_DGRAM, 0);
    sock2_len = sizeof(sock2);

    sock1_len = sizeof(sock1);
    check = getsockopt(sock1, SOL_SOCKET, SO_TYPE , (void *)&sock1_type , &sock1_len);
    Error(check, "sock1 getsockopt() error");
    check = getsockopt(sock1, SOL_SOCKET, SO_RCVBUF, (void *)&sock1_rcvbuf, &sock1_len);
    Error(check, "getsockopt() error");
    check = getsockopt(sock1, SOL_SOCKET, SO_SNDBUF, (void *)&sock1_sndbuf, &sock1_len);

    Error(check, "getsockopt() error");
    printf("SOCK1:\n   Type: %d\n   RCVBUF: %d\n   SNDBUF: %d\n", sock1_type, sock1_rcvbuf, sock1_sndbuf);

    sock1_rcvbuf = 1024*3;
    check = setsockopt(sock1, SOL_SOCKET, SO_RCVBUF, (void*)&sock1_rcvbuf, sizeof(sock1_rcvbuf));
    Error(check,  "setsockopt() error");
    sock1_sndbuf = 1024*3;
    check = setsockopt(sock1, SOL_SOCKET, SO_SNDBUF, (void*)&sock1_sndbuf, sizeof(sock1_rcvbuf));
    Error(check,  "setsockopt() error");
    printf("SOCK1:\n   Type: %d\n   RCVBUF: %d\n   SNDBUF: %d\n", sock1_type, sock1_rcvbuf, sock1_sndbuf);

    
    sock2_len = sizeof(sock2);
    check = getsockopt(sock2, SOL_SOCKET, SO_TYPE, (void *)&sock2_type , &sock2_len);
    Error(check, "sock2 getsockopt() ");
    check = getsockopt(sock1, SOL_SOCKET, SO_RCVBUF, (void *)&sock2_rcvbuf, &sock2_len);
    Error(check, "getsockopt() error");
    check = getsockopt(sock1, SOL_SOCKET, SO_SNDBUF, (void *)&sock2_sndbuf, &sock2_len);
    Error(check, "getsockopt() error");
    printf("SOCK2:\n   Type: %d\n   RCVBUF: %d\n   SNDBUF: %d", sock2_type, sock2_rcvbuf, sock2_sndbuf);

    close(sock1);
    close(sock2);
    return 0;
    
}










