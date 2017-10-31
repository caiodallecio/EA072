#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <string.h>
#include "y.tab.h"
#include "header.h"
#define PORT 5000
#define MAX_PROCESS 4

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern http_request_t * list;
int current_process = 0;

void sigchld_handler(int sig){
    if(sig == SIGCHLD){
        printf("One of the children has finished its job\n");
        current_process--;
    }
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1, f = 0;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    memset(buffer,0,sizeof(buffer));
    
    if(signal(SIGCHLD,sigchld_handler) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }
    if(signal(SIGALARM,sigchld_handler) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    char * answer;
    while(1){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        else if(current_process < MAX_PROCESS){
            printf("New Request\n");
            f = fork();
            if (f>0){
                printf("New Process #%d\n",current_process);
                valread = read( new_socket , buffer, 1024);
                printf("<<<Request>>>\n");
                printf("%s\n",buffer);
                YY_BUFFER_STATE internal_buffer = yy_scan_string(buffer);
                yyparse();
                yy_delete_buffer(internal_buffer);
                if(list != NULL){
                    answer = (char*)http_response(list);
                    printf("<<<Response>>>\n");
                    printf("%s\n",answer);
                    send(new_socket, answer, strlen(answer), 0);
                    free(answer);
                }
                return 0;
            } else if (f == 0)
                current_process++;
            else
                printf("Error on fork\n");
        } else {
            printf("We have hitted max threads\n");
            char * over = (char *)server_overload();
        
            send(new_socket, over, strlen(over), 0);
            free(over);
        }
    } 
    return 0;
}