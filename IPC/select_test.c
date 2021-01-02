#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 50
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
    fd_set read_set, temp_set;
    struct timeval timeout;

    int result, buf_len, flag=1;
    char buf[BUF_SIZE];
    
    pid_t pid;

    
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    
    while(1){
        fprintf(stderr, ".");
        temp_set = read_set;
        timeout.tv_sec = 1;
        timeout.tv_usec = 1000;
        
        result = select(1, &temp_set, 0, 0, &timeout);
        if(FD_ISSET(STDIN_FILENO, &temp_set)){
            fprintf(stderr, "stderr: Input dectected.\n");
            fgets(buf, BUF_SIZE, stdin);

            buf_len = strlen(buf)-1;
            if (buf[buf_len] == '\n')
                buf[buf_len] = '\0';

            printf("\nMessage: %s\n", buf);
            flag = 1;
        }
        
    }
        
    return 0;
}




















