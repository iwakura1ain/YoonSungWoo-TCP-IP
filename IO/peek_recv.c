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
    int serv_sock, clnt_sock; 
    STRUCT SOCKADDR_IN SERV_ADDR, CLNT_ADDR;
    SOCKLEN_T SERV_ADDR_SIZE, CLNT_ADDR_SIZE;

    CHAR MSG[BUF_SIZE];
    INT MSG_LEN, RESULT;

        
    HELP(ARGC, 1, "OOBRECV [PORT]");

    SERV_SOCK = SOCKET(PF_INET, SOCK_STREAM, 0);
    ERROR(SERV_SOCK, "SOCKER() ERROR");

    SERV_ADDR_SIZE = SIZEOF(SERV_ADDR);
    MEMSET(&SERV_ADDR, 0, SERV_ADDR_SIZE);
    SERV_ADDR.SIN_FAMILY = AF_INET;
    SERV_ADDR.SIN_ADDR.S_ADDR = HTONL(INADDR_ANY);
    SERV_ADDR.SIN_PORT = HTONS(ATOI(ARGV[1]));

    RESULT = BIND(SERV_SOCK, (STRUCT SOCKADDR*)&SERV_ADDR, SERV_ADDR_SIZE);
    ERROR(RESULT, "BIND() ERROR");
    LISTEN(SERV_SOCK, 5);

    CLNT_ADDR_SIZE = SIZEOF(CLNT_ADDR);
    CLNT_SOCK = ACCEPT(SERV_SOCK, (STRUCT SOCKADDR*)&CLNT_ADDR, &CLNT_ADDR_SIZE);

    WHILE(1){
        MSG_LEN = RECV(CLNT_SOCK, MSG, BUF_SIZE-1, MSG_PEEK|MSG_DONTWAIT);
        IF(MSG_LEN > 0)
            BREAK;
    }
    MSG[MSG_LEN] = 0;
    Fprintf(Stdout, "First Peek: %S\N", Msg);

    Msg_Len = Recv(Clnt_Sock, Msg,Buf_Size-1, 0);
    Msg[Msg_Len] = 0;
    fprintf(stdout, "second recv: %s\n", msg);
     
    close(clnt_sock);
    close(serv_sock);
         
    return 0;
}






















