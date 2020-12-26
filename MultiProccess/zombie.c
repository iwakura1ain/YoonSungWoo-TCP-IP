#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    pid_t pid = fork();

    if(pid == 0){
        puts("Child process spawned.");
    }
    else{
        printf("Spawned child pid: %d", pid);
        sleep(30);
    }

    if(pid == 0){
        puts("Ending Child process.");
    }
    else{
        printf("Ending Parent process");
    }
    
    return 0;
}










