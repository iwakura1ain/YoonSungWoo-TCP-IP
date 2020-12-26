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

int gval = 10;
int main(int argc, char *argv[]){
    pid_t pid;
    int lval = 20;
    gval++; lval++;

    pid = fork();
    if(pid == 0){
        gval += 2;
        lval += 2;
    }
    else{
        gval -= 2;
        lval -= 2;
    }

    if(pid == 0){
        printf("Child Proc: [%d %d]\n", gval, lval);
    }
    else{
        printf("Parent Proc: [%d %d]\n", gval, lval);
    }
    
    return 0;
}










