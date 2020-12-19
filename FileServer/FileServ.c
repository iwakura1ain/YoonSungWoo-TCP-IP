#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

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
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t serv_addr_size, clnt_addr_size;

    FILE *file;
    char buffer[BUF_SIZE];
    int read_len, result;

    Help(argc, 2, "FileServ [port] [file]");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(serv_sock, "socket() error");

    serv_addr_size = sizeof(serv_addr);
    memset(&serv_addr, 0, serv_addr_size);
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    result = bind(serv_sock, (struct sockaddr *)&serv_addr, serv_addr_size);
    Error(result, "bind() error");

    listen(serv_sock, 1);

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    file = fopen(argv[2], "rb");
    while(1){
        read_len = fread((void *)buffer, 1, BUF_SIZE, file);
        if(read_len < BUF_SIZE){
            write(clnt_sock, buffer, read_len);
            break;
        }
        else{
            write(clnt_sock, buffer, BUF_SIZE);
        }
      
    }
    shutdown(clnt_sock, SHUT_WR);
    read_len = read(clnt_sock, buffer, BUF_SIZE);
    printf("Message from Serv: %s\n", buffer);

    fclose(file);
    close(clnt_sock);
    close(serv_sock);
    return 0;
    
}




























