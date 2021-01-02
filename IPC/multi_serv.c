#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
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

int GetMaxFd(int *fds, int fd_count){
    int i;
    
    int max = fds[0];
    for(i=1; i<=fd_count; i++){
        if(fds[i]> max)
            max = fds[i];
    }

    return max;
}

void Client(int clnt_sock, int ctos_pipe[2], int stoc_pipe[2]){
    fd_set read_fds, temp_fds;
    struct timeval timeout;
    int fd_list[] = {clnt_sock, stoc_pipe[0]}, max_fd = GetMaxFd(fd_list, 2);

    char message[BUF_SIZE];
    int msg_len, result;


    fprintf(stderr, "clnt: child process started\n");
    fprintf(stderr, "clnt: clnt_sock:%d stoc_pipe:%d\n", clnt_sock, stoc_pipe[0]);
    
    while(1){
        timeout.tv_sec=1;
        timeout.tv_usec=1000;

        FD_ZERO(&read_fds);
        FD_SET(clnt_sock, &read_fds);
        FD_SET(stoc_pipe[0], &read_fds);
        temp_fds = read_fds;
        
        result = select(max_fd+1, &temp_fds, NULL, NULL, &timeout);
        if(result > 0){
            if(FD_ISSET(clnt_sock, &temp_fds)){ //received client message
                fprintf(stderr, "clnt: recieved message\n");
                msg_len = read(clnt_sock, message, BUF_SIZE);
                if(msg_len <= 0){
                    fputs("client disconnected", stdout);
                    //needs closes on pipes
                    return;
                }
                else{ //send propagation
                    fprintf(stderr, "clnt: sending propagation to serv process\n");
                    message[msg_len] = 0;
                    write(ctos_pipe[1], message, msg_len);
                }
            }
            else if(FD_ISSET(stoc_pipe[0], &temp_fds)){  //client proc recieved propagation
                fprintf(stderr, "\nclnt: recieved propagation\n");
                msg_len = read(stoc_pipe[0], message, BUF_SIZE);
                write(clnt_sock, message, msg_len);
            }
        }
        else if(result == 0)
            continue;
        else
            break;
    }
    return;
            
}

void CloseFds(int fds[5][2], int start, int end){
    int i;

    for(i=start; i<=end; i++){
        close(fds[i][0]);
        close(fds[i][1]);
    }
}

void Propagate(int stoc_pipe[5][2], int clnt_count, int exclude_index, char *msg){
    int i;

    for(i=0; i<clnt_count; i++){
        if(i != exclude_index){
            fprintf(stderr, "\nmain: propagating to client %d\n", i);
            write(stoc_pipe[i][1], msg, strlen(msg));
        }
    }
}

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock, clnt_count=0;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t serv_addr_size, clnt_addr_size;
    
    pid_t pid;
    fd_set listen_fds, temp_fds;
    int stoc_pipe[5][2], ctos_pipe[5][2];
    int fd_list[6], max_fd;
    struct timeval timeout;

    char message[BUF_SIZE], *temp;
    int msg_len, i, result;


    fputs("Server started...\n", stdout);
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(serv_sock, "socket() error");
    fd_list[0] = serv_sock;
    
    serv_addr_size = sizeof(serv_addr);
    memset(&serv_addr, 0, serv_addr_size);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    result = bind(serv_sock, (struct sockaddr*)&serv_addr, serv_addr_size);
    Error(result, "bind() error");
    result = listen(serv_sock, 5);
    Error(result, "listen() error");

    
    fprintf(stderr, "main: select() loop started\n");
    while(1){
        //fprintf(stderr, ".");
        timeout.tv_sec = 1;
        timeout.tv_usec = 1000;
        max_fd = GetMaxFd(fd_list, clnt_count);

        FD_ZERO(&listen_fds);
        for(i=0; i<=clnt_count; i++){
            FD_SET(fd_list[i], &listen_fds);
        }
        temp_fds = listen_fds;

        result = select(max_fd+1, &temp_fds, NULL, NULL, &timeout); 
        if(FD_ISSET(serv_sock, &temp_fds)){  //client connected
            fprintf(stderr, "main: client requested connection");
            clnt_addr_size = sizeof(clnt_addr);
            clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
            Error(clnt_sock, "accept() error");
            fprintf(stderr, "main: client connected\n");
            
            pipe(stoc_pipe[clnt_count]);
            pipe(ctos_pipe[clnt_count]);
            pid = fork();
            if(pid == 0){ //client child process started         
                close(serv_sock);
                CloseFds(stoc_pipe, 0, clnt_count);
                CloseFds(ctos_pipe, 0, clnt_count);
                Client(clnt_sock, ctos_pipe[clnt_count], stoc_pipe[clnt_count]);

                //client process returned, also fuckin messy
                fprintf(stderr, "clnt: child process ended\n");
                close(clnt_sock);
                CloseFds(stoc_pipe, clnt_count, clnt_count);
                CloseFds(ctos_pipe, clnt_count, clnt_count);             
                exit(0);
            }
            else{ //serv process, client connected
                //FD_SET(ctos_pipe[clnt_count][0], &listen_fds);
                fd_list[clnt_count+1] = ctos_pipe[clnt_count][0];
                clnt_count++;
                fprintf(stderr, "main: client(%d) count %d\n", pid, clnt_count);
            }
        }else{ //client sent message
            for(i=0; i<clnt_count; i++){ //check all client pipes
                if(FD_ISSET(ctos_pipe[i][0], &temp_fds)){
                    fprintf(stderr, "main: recieved client%d propagation \n", i);
                    msg_len = read(ctos_pipe[i][0], message, BUF_SIZE);
                    message[msg_len] = 0;
                                        
                    fprintf(stderr, "message: %s\n", message);
                    Propagate(stoc_pipe, clnt_count, i, message);
                }
            }
            
        }
        //no way to exit, must implement
    }
    
        
    return 0;
}




















