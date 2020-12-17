#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100

int main(void){
    int fd;
    char buf[BUF_SIZE];

    fd =open("data.txt", O_RDONLY);
    if(fd == -1){
        printf("open() error");
    }

    printf("File descriptor: %d\n", fd);

    if(read(fd, buf, BUF_SIZE) == -1){
        printf("read() error");
    }

    printf("Data: %s", buf);

    close(fd);
    return 0;
     
}
