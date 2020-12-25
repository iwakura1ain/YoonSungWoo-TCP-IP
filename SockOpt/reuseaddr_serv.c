#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
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
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int msg_len, check, i;
    int sock_opt=TRUE;
    socklen_t serv_addr_size, clnt_addr_size, sock_opt_len=sizeof(sock_opt);
    struct sockaddr_in  serv_addr, clnt_addr;

    Help(argc, 1, "Usage: ReuseAddrServ [port]");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(serv_sock, "socket() error");
    
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, sock_opt_len);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    serv_addr_size = sizeof(serv_addr);
    check = bind(serv_sock, (const struct sockaddr*)&serv_addr, serv_addr_size);
    Error(check, "bind() error");
    check = listen(serv_sock, 5);
    Error(check, "listen() error");

    clnt_addr_size = sizeof(clnt_addr);

    for(i=0; i<5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        while( (msg_len=read(clnt_sock, message, BUF_SIZE)) != 0 ){
            //msg_len = read(clnt_sock, message, BUF_SIZE);        
            write(clnt_sock, message, msg_len);
        }
    }

    close(clnt_sock);
    close(serv_sock);    
    return 0;
}










