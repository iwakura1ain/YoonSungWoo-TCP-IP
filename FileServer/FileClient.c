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
    int sock;
    struct sockaddr_in sock_addr;
    socklen_t sock_addr_size;
    
    FILE *file;
    char buffer[BUF_SIZE];
    int read_len;
    int result, i;

    Help(argc, 3, "FileClient [ip] [port] [output]");

    sock = socket(PF_INET, SOCK_STREAM, 0);
    file = fopen(argv[3], "wb");

    sock_addr_size = sizeof(sock_addr);
    memset(&sock_addr, 0, sock_addr_size);
    sock_addr.sin_family = PF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));

    result = connect(sock, (struct sockaddr*)&sock_addr, sock_addr_size);
    Error(result, "connect() error");

    while( (read_len = read(sock, buffer, BUF_SIZE))!= 0 ){
        fwrite(buffer, 1, read_len, file);
    }

    printf("Transfer Finished");
    write(sock, "Transfer Finished.",sizeof("Transfer Finished"));
   
    fclose(file);
    close(sock);
    
    return 0;
}




























