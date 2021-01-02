#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

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

void killZombie(int sig){
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("\nRemoved PID %d\n", pid);
        printf("Returned %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t serv_addr_size, clnt_addr_size;
    
    pid_t pid;
    struct sigaction zombie_handler;

    int msg_len, state, result;
    char message[BUF_SIZE];

    Help(argc, 1, "MultiEchoServ [port]");

    zombie_handler.sa_handler = killZombie;
    zombie_handler.sa_flags = 0;
    sigemptyset(&zombie_handler.sa_mask);
    state = sigaction((SIGCHLD), &zombie_handler, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(serv_sock, "socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    serv_addr_size = sizeof(serv_addr);
    result = bind(serv_sock, (struct sockaddr *)&serv_addr, serv_addr_size);
    Error(result, "bind() error");
    result = listen(serv_sock, 5);
    Error(result, "listen() error");

    while(1){
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1){
            printf("accept() error");
            continue;
        }
        
        pid = fork();
        if(pid == 0){
            close(serv_sock);
            
            while( (msg_len = read(clnt_sock, message, BUF_SIZE)) != 0){
                write(clnt_sock, message, msg_len);
            }

            close(clnt_sock);
            printf("Client disconnected.");
            return 0;
        }
        else{
            printf("Client connected, spawned client process.");
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}













