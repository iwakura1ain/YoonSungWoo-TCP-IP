#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int main(int argc, char *argv[]){
    int status, is_exit;
    pid_t pid1, pid2;

    pid1 = fork();
    if(pid1 == 0){
        sleep(14);
        return 1;
    }
    else{
        printf("Child1 (PID: %d) started...\n", pid1);

        pid2=fork();
        if(pid2 == 0){
            sleep(30);  
            return 2;
            
        }
        else{
            printf("Child2 (PID: %d) started...\n", pid2);
            
            wait(&status);
            is_exit = WIFEXITED(status);
            if(is_exit)
                printf("Child1 returned %d.\n", WEXITSTATUS(status));

            while(!waitpid(pid2, &status, WNOHANG)){
                sleep(5);
                puts("Child2 still running...");
            }
            is_exit = WIFEXITED(status);
            if(is_exit){
                printf("Child2 returned %d.\n", WEXITSTATUS(status));
            }
            /*
            wait(&status);
            is_exit = WIFEXITED(status);
            if(is_exit)
                printf("Child2 returned %d.\n", WIFEXITED(status));
            */
            
        }
    }
      
    return 0;
}










