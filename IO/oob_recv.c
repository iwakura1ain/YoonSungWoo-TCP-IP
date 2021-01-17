#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 100
#define TRUE 1
#define FALSE 0

void OOB_Handler(int sig);

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

int serv_sock, clnt_sock;
int main(int argc, char *argv[]){
    //int serv_sock, clnt_sock; defined as global for sighandler func to see
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t serv_addr_size, clnt_addr_size;

    char msg[BUF_SIZE];
    int msg_len, result;

    struct sigaction sig_handler;

    
    Help(argc, 1, "OOBRecv [port]");

    sig_handler.sa_handler = OOB_Handler;
    sigemptyset(&sig_handler.sa_mask);
    sig_handler.sa_flags = 0;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(serv_sock, "socker() error");

    serv_addr_size = sizeof(serv_addr);
    memset(&serv_addr, 0, serv_addr_size);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    
    result = bind(serv_sock, (struct sockaddr*)&serv_addr, serv_addr_size);
    Error(result, "bind() error");
    listen(serv_sock, 5);

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    fcntl(clnt_sock, F_SETOWN, getpid());
    result = sigaction(SIGURG, &sig_handler, 0);

    while( (msg_len = recv(clnt_sock, msg, BUF_SIZE, 0)) != 0){
        Error(msg_len, "recv() error");
        msg[msg_len] = 0;
        fprintf(stdout, "\nrecv(): %s\n", msg);
    }

    close(clnt_sock);
    close(serv_sock);
         
    return 0;
}

void OOB_Handler(int sig){
    int msg_len;
    char msg[BUF_SIZE];

    msg_len = recv(clnt_sock, msg, BUF_SIZE, 0);
    msg[msg_len] = 0;
    fprintf(stdout, "\nOOB[%d]: %s\n", sig, msg);

    return;
}




















