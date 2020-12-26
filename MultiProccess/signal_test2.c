#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

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

void timeout(int sig){
    if(sig == SIGALRM)
        puts("time out!");
    alarm(2);
}

void interrupt(int sig){
    if(sig == SIGALRM)
        puts("program interrupted");
    alarm(2);
}

int main(int argc, char *argv[]){
    int i;
    
    struct sigaction handler;
    handler.sa_handler = interrupt;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGALRM, &handler, 0);

    alarm(2);
    for(i=0; i<5; i++){
        puts("waiting for signal...");
        sleep(10);
            
    }
      
    return 0;
}










