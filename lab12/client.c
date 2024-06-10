#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_NAME 20
#define MAX_MSG 100

int server_fd = -1;
int running = 1;
int flag = 0;
char name[MAX_NAME];
struct sockaddr_in srv;

void handler(int sig){
    char message[MAX_NAME + 6];
    strcpy(message, name);
    strcat(message, " STOP");
    message[strlen(name) + 5] = '\0';

    if (server_fd != -1){
        sendto(server_fd, message, strlen(message), 0,
            (struct sockaddr*)&srv, sizeof(srv));
    }
    running = 0;
    flag = 1;
}

void* thread_fun(void* arg){
    struct sockaddr_in cli;
    memset(&cli, 0, sizeof(cli));
    int addr_len = sizeof(cli);
    char buff[MAX_MSG];

    while (running){
        int bytes = recvfrom(server_fd, buff, sizeof(buff), 0,
            (struct sockaddr*)&cli, (socklen_t*)&addr_len);
        
        if (flag){
            printf("\nClient stops");
            fflush(stdout);
            continue;
        }
        if (bytes > 0){
            buff[bytes] = '\0';

            if (strcmp(buff, "STOP") == 0){
                //printf("Client\n");
            }
            else {
                printf("%s\n", buff);
            }
        }
    }
    
    return NULL;
}

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Invalid number of arguments\n");
        return 0;
    }

    signal(SIGINT, handler);
    strcpy(name, argv[1]);
    char* address = malloc(strlen(argv[2]) * sizeof(char));
    strcpy(address, argv[2]);
    int port = atoi(argv[3]);


    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1){
        printf("Socket error\n");
        free(address);
        return 0;
    }

    //struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr(address);
    srv.sin_port = htons(port);

    char message[MAX_NAME + 5];
    strcpy(message, name);
    strcat(message, " NEW");
    message[strlen(name) + 4] = '\0';
    sendto(server_fd, message, strlen(message), 0,
        (struct sockaddr*)&srv, sizeof(srv));
    
    pthread_t thread;
    pthread_create(&thread, NULL, &thread_fun, NULL);

    char buff[MAX_MSG + MAX_NAME + 1];
    strcpy(buff, name);
    buff[strlen(name)] = ' ';
    buff[strlen(name) + 1] = '\0';

    printf("Type command:\n");
    while (running){
        fgets(buff + strlen(name) + 1, sizeof(buff) - strlen(name) - 1, stdin);
        if (buff[strlen(buff) - 1] == '\n'){
            buff[strlen(buff) - 1] = '\0';
        }

        if (strcmp(buff + strlen(name) + 1, "STOP") == 0){
            printf("Client stops\n");
            sendto(server_fd, buff, strlen(buff), 0,
                (struct sockaddr*)&srv, sizeof(srv));
            running = 0;
        }

        if (running){
            sendto(server_fd, buff, strlen(buff), 0,
                (struct sockaddr*)&srv, sizeof(srv));
        }
    }

    pthread_join(thread, NULL);
    close(server_fd);
    free(address);
    return 0;
}
