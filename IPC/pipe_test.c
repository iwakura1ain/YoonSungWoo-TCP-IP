#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
    int fds[2];
    char child_msg[] = "child sent message";
    char parent_msg[] = "parent sent message";
    char buf[BUF_SIZE];
    int read_len;
    pid_t pid;

    pipe(fds);
    pid = fork();
    if(pid == 0){
        fputs("\nchild started\n", stdout);
        write(fds[1], child_msg, sizeof(child_msg));
        sleep(2);
        read_len = read(fds[0], buf, BUF_SIZE);
        fputs(buf, stdout);
    }else{
        fputs("\nparent started\n", stdout);
        sleep(1);
        read_len = read(fds[0], buf, BUF_SIZE);
        fputs(buf, stdout);
        write(fds[1], parent_msg, sizeof(parent_msg));
        sleep(4);
    }
    
    
    return 0;
}













