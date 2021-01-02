#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#define BUF_SIZE 100
#define TRUE 1
#define FALSE 0

void Error(int error, char *message){
    if(error == -1){
        fprintf(stderr, "stderr: %s", message);
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

void Client(int clnt_sock, int *ctos_pipe){
    fd_set read_fds, temp;
    struct timeval timeout;
    int result, msg_len;
    char buf[BUF_SIZE];
    
    FD_ZERO(&read_fds);
    FD_SET(ctos_pipe[0], &read_fds);
    FD_SET(clnt_sock, &read_fds);

    while(1){
        temp = read_fds;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100;
        
        result = select(2, &temp, 0, 0, &timeout);
        if(result > 0){
            if(FD_ISSET(clnt_sock, &temp)){ //network communication with remote client
                msg_len = read(clnt_sock, buf, BUF_SIZE);
                if(msg_len == 0){ //client disconnects
                    write(ctos_pipe[1], buf, msg_len); // send disconnect to server process
                    close(clnt_sock);
                    close(ctos_pipe[0]);
                    close(ctos_pipe[1]);
                                        
                    fputs("\nClient disconnected", stdout);
                    return;
                }
                else{ //client sends message
                    buf[msg_len] = 0;
                    fputs("\nMessage from Client: ", stdout);
                    fputs(buf, stdout);
                
                    write(ctos_pipe[1], buf, msg_len);
                }
            }
            else if(FD_ISSET(ctos_pipe[0], &temp)){ //client proc recieves propagation
                msg_len = read(ctos_pipe[0], buf, BUF_SIZE);
                fputs("\nRecieved Propogation.", stdout);

                write(clnt_sock, buf, msg_len);
            }
        }
        
    }
    
}

/*void Propogate(char *message , int clnt_pipe[5][2], int except_fds, int max_fds){
    int i;
    
    for(i=0; i<max_fds;i++){
        if(i == except_fds)
            continue;
        write(clnt_pipe[i][0], message, sizeof(message));
    }
    }*/

void CloseAtIndex(int fds[5][2], int index, int *max_fds){
    int i;
    
    close(fds[index][0]);
    close(fds[index][1]);
    for(i=index; i<*max_fds-1; i++){
        fds[i][0] = fds[i+1][0];
        fds[i][1] = fds[i+1][1];
    }
}

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_size;

    fd_set listen_fds, temp_fds;
    struct timeval timeout;

    char message[BUF_SIZE], recv_buf[BUF_SIZE];
    int msg_len, recv_len, result;

    
    fputs("Client started...\n", stdout);
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    Error(sock, "socket() error");

    serv_addr_size = sizeof(serv_addr);    
    memset(&serv_addr, 0, serv_addr_size);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_family = AF_INET;
    
    result = connect(sock, (struct sockaddr*)&serv_addr, serv_addr_size);
    Error(result, "connect() error");

    fprintf(stderr, "stderr: starting select() loop\n");
    while(1){
        //fprintf(stderr, ".");
        timeout.tv_sec = 1;
        timeout.tv_usec = 1000;

        FD_ZERO(&listen_fds);
        FD_SET(STDIN_FILENO, &listen_fds); //stdin
        FD_SET(sock, &listen_fds);  //server
        temp_fds = listen_fds;
        
        result = select(sock+1, &temp_fds, 0, 0, &timeout);
        if(result > 0){
            if(FD_ISSET(sock, &temp_fds)){ //server sent back propagation
                fprintf(stderr, "stderr: server sent back propagation\n");
                /*
                recv_len = 0;
                while(1){
                    recv_len += read(sock, message, BUF_SIZE);
                    if(!strcmp(&message[recv_len-1], "\0")){
                        break;
                    }
                    else{
                        
                    }
                }                
                */
                //ioctl(sock, FIONREAD, &msg_len);            
                recv_len = read(sock, recv_buf, BUF_SIZE);
                if(recv_len == 0){
                    exit(1);
                }
                else{
                    recv_buf[recv_len] = 0;
                    fprintf(stdout, "%s\n", recv_buf);
                }
            }
            else if(FD_ISSET(STDIN_FILENO, &temp_fds)){ //message inputted
                fgets(message, BUF_SIZE, stdin);
                msg_len = strlen(message)-1;
                if(!strcmp(message, "q"))
                    break;
                else
                    fprintf(stderr, "stderr: sending message\n");
                    write(sock, message, msg_len);
            }
        }
        else if(result == 0){
            continue;
        }
        else{
            fputs("select() error, quitting", stdout);
            exit(1);
        }   

        
    }
    
        
    return 0;
}




















