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

void read_childproc(int sig){
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("\nRemoved PID %d\n", pid);
        printf("Returned %d\n", WEXITSTATUS(status));
    }
}

void interrupt(int sig){
    if(sig == SIGALRM)
        puts("program interrupted");
    alarm(2);
}

int main(int argc, char *argv[]){
    int i;

    pid_t pid;
    struct sigaction zombie_handler;
    zombie_handler.sa_handler = read_childproc;
    zombie_handler.sa_flags = 0;
    sigemptyset(&zombie_handler.sa_mask);
    
    sigaction(SIGCHLD, &zombie_handler, 0);
    
    pid = fork();
    if(pid == 0){
        printf("Child proc1 spawned.\n");
        sleep(5);
        return 12;
    }
    else{ 
        printf("Child proc1 PID: %d\n", pid);
        pid = fork();
        if(pid == 0){
            printf("Child proc2 spawned.\n");
            sleep(5);
            exit(13);
        }
        else{
            printf("Child proc2 PID: %d\n", pid);

            for(i=0; i<5; i++){
                puts("Waiting for signal...");
                sleep(10);
            }
        }
            
        
    }
    
    return 0;
}










