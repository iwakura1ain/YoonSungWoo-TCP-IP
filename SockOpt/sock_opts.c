#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
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
    int sock1, sock2;
    socklen_t sock1_optlen, sock2_optlen;
    int check;

    int sock1_type, sock2_type;
    int sock1_rcvbuf, sock2_rcvbuf;
    int sock1_sndbuf, sock2_sndbuf;
    int sock1_nagle, sock2_nagle;
   
    sock1 = socket(PF_INET, SOCK_STREAM, 0);
    sock2 = socket(PF_INET, SOCK_DGRAM, 0);


    sock1_optlen = sizeof(sock1_type);
    check = getsockopt(sock1, SOL_SOCKET, SO_TYPE , (void *)&sock1_type , &sock1_optlen);
    Error(check, "sock1 getsockopt() error");
    sock1_optlen = sizeof(sock1_rcvbuf);
    check = getsockopt(sock1, SOL_SOCKET, SO_RCVBUF, (void *)&sock1_rcvbuf, &sock1_optlen);
    Error(check, "getsockopt() error");
    sock1_optlen = sizeof(sock1_sndbuf);
    check = getsockopt(sock1, SOL_SOCKET, SO_SNDBUF, (void *)&sock1_sndbuf, &sock1_optlen);
    Error(check, "getsockopt() error");
    sock1_optlen = sizeof(sock1_nagle);
    check = getsockopt(sock1, IPPROTO_TCP, TCP_NODELAY, (void*)&sock1_nagle, &sock1_optlen);
    printf("SOCK1:\n   Type: %d\n   RCVBUF: %d\n   SNDBUF: %d\n   NAGLE:%d\n"
           , sock1_type, sock1_rcvbuf, sock1_sndbuf, sock1_nagle);
    
    sock1_rcvbuf = 1024*3;
    check = setsockopt(sock1, SOL_SOCKET, SO_RCVBUF, (void*)&sock1_rcvbuf, sizeof(sock1_rcvbuf));
    Error(check,  "setsockopt() error");
    sock1_sndbuf = 1024*3;
    check = setsockopt(sock1, SOL_SOCKET, SO_SNDBUF, (void*)&sock1_sndbuf, sizeof(sock1_sndbuf));
    Error(check,  "setsockopt() error");
    sock1_nagle = TRUE;
    check = setsockopt(sock1, IPPROTO_TCP, TCP_NODELAY, (void*)&sock1_nagle, sizeof(sock1_nagle));
    Error(check,  "setsockopt() error");

    printf("SOCK1:\n   Type: %d\n   RCVBUF: %d\n   SNDBUF: %d\n   NAGLE:%d\n"
           ,sock1_type, sock1_rcvbuf, sock1_sndbuf, sock1_nagle);

    /*
    sock2_len = sizeof(sock2);
    check = getsockopt(sock2, SOL_SOCKET, SO_TYPE, (void *)&sock2_type , &sock2_len);
    Error(check, "sock2 getsockopt() ");
    check = getsockopt(sock1, SOL_SOCKET, SO_RCVBUF, (void *)&sock2_rcvbuf, &sock2_len);
    Error(check, "getsockopt() error");
    check = getsockopt(sock1, SOL_SOCKET, SO_SNDBUF, (void *)&sock2_sndbuf, &sock2_len);
    Error(check, "getsockopt() error");
    printf("SOCK2:\n   Type: %d\n   RCVBUF: %d\n   SNDBUF: %d", sock2_type, sock2_rcvbuf, sock2_sndbuf);
    */
    close(sock1);
    close(sock2);
    return 0;
    
}










