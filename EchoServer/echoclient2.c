#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void Error(char *message, int i){
    if(i == -1){
        fputs(message, stderr);
        exit(1);
    }
}

void Help(int argc, int c, char message[]){
    if(argc-1 != c){
        printf("%s", message);
    }
    exit(1);
    
}

//Usage: HelloClient <RHOST> <RPORT>
int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len, recv_len, temp_recv;

    //   Help(argc, 2, "echoclient [ip] [port]");


    sock = socket(PF_INET, SOCK_STREAM, 0);
    Error("socket() error", sock);

        
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_family = AF_INET;

    //Error("connect() error", connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) );
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        printf("connect() error");
        exit(1);
    }
    printf("connected...");

    while(1){
        fputs("Input message: ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(strcmp(message, "q\n") == 0){
            break;
        }

        str_len = write(sock, message, strlen(message));
       
        recv_len = 0;
        while(recv_len < str_len){
            temp_recv = read(sock, message, BUF_SIZE);
            if(temp_recv != -1){
                recv_len += temp_recv;
            }
            else{
                printf("read() error");
                exit(1);
            }
        }
        //str_len = read(sock, message, BUF_SIZE-1);
        message[str_len+1] = 0;
        printf("Message from serv: %s", message);
    }

    close(sock);
    return 0;
    
}

