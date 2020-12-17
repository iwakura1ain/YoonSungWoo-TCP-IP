#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    char *addr = argv[1];
    char converted[20];
    struct sockaddr_in addr_in;

    if(!inet_aton(addr, &addr_in.sin_addr)){
        printf("aton() error");
    }
    else{
        printf("Network ordered ASCII address: %#x", addr_in.sin_addr.s_addr);
    }

    strcpy(converted, inet_ntoa(addr_in.sin_addr));
   n printf("\nASCII ordered Network address: %s", converted);
    
    
    
}
