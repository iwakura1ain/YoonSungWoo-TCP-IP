#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void Error(int error, char *message){
    if(error <= 0){
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
    int i;
    struct hostent *host;

                                   
    struct sockaddr_in host_addr;
        
    Help(argc, 1, "DNSLookup [url]");
    host = gethostbyname(argv[1]);
    printf("Official Name: %s\n", host->h_name);
    for(i=0; host->h_aliases[i]; i++)
        printf("Alias %d: %s\n", i+1, host->h_aliases[i]);
    printf("Address Type: %d\n Address Len: %d\n", host->h_addrtype, host->h_length);
    for(i=0; host->h_addr_list[i];i++)
        printf("Address %d: %s\n", i, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) );

    memset(&host_addr, 0, sizeof(host_addr));
    host_addr.sin_addr.s_addr = inet_addr(host->h_addr_list[0]);
    host = gethostbyaddr((char*)&host_addr.sin_addr, 4, AF_INET);
    //Error((int)host, "gethostbyaddr() error");
    
    printf("Official Name: %s\n", host->h_name);
    for(i=0; host->h_aliases[i]; i++)
        printf("Alias %d: %s\n", i+1, host->h_aliases[i]);
    printf("Address Type: %d\n Address Len: %d\n", host->h_addrtype, host->h_length);
    for(i=0; host->h_addr_list[i];i++)
        printf("Address %d: %s\n", i, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) );
    
    return 0;
}












