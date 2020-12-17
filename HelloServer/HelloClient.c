#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void Error(char *message, int i){
    if(i == -1){
        fputs(message, stderr);
        exit(1);
    }
}


//Usage: HelloClient <RHOST> <RPORT>
int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    Error("socket() error", sock);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    Error("connect() error",
          connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)));

    str_len=read(sock, message, sizeof(message)-1);
    Error("read() error", str_len);

    printf("Message from server:");
    printf("%s", message);
    close(sock);
    return 0;
    
    
}
