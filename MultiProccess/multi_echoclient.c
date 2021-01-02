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

void KillZombie(int sig){
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("\nRemoved PID %d\n", pid);
        printf("Returned %d\n", WEXITSTATUS(status));
    }
}

void ReadRoutine(int sock, char *buf){
    int read_len;

    while(1){
        read_len = read(sock, buf, BUF_SIZE);
        if(read_len == 0)
            return;
        else{
            buf[read_len] = 0;
            fputs("\nMessage from server: ", stdout);
            fputs(buf, stdout);
        }
    }
}

void WriteRoutine(int sock, char *buf){
    while(1){
        fputs("\nInput message: ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp("q", buf)){
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));        
    }
}

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_size;
    
    pid_t pid;
    struct sigaction zombie_handler;

    int msg_len, state, result;
    char message[BUF_SIZE];

    Help(argc, 2, "MultiEchoClient [ip] [port]");

    zombie_handler.sa_handler = KillZombie;
    zombie_handler.sa_flags = 0;
    sigemptyset(&zombie_handler.sa_mask);
    state = sigaction((SIGCHLD), &zombie_handler, 0);

    serv_addr_size = sizeof(serv_addr);
    memset(&serv_addr, 0, serv_addr_size);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(sock, "socket() error");
//    result = bind(sock, (struct sockaddr *)&serv_addr, serv_addr_size);
//    Error(result, "bind() error");
    result = connect(sock, (const struct sockaddr*)&serv_addr, serv_addr_size);
    Error(result, "connect() error");

    pid = fork();
    if(pid == 0)
        WriteRoutine(sock, message);
    else
        ReadRoutine(sock, message);
    

    close(sock);
    return 0;
}













