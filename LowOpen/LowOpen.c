#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    int fd;
    char buf[] = "alrighty then!";

    fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
    if(fd == -1){
        printf("open() error");
    }
    printf("File descriptor: %d\n", fd);

    if(write(fd, buf, sizeof(buf)) == -1){
        printf("write() error");
    }

    close(fd);
    return 0;
    
}
