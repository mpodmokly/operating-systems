#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_NAME 20
#define MAX_MSG 100

void* thread_fun(void* arg){
    int server_fd = *(int*)arg;
    struct sockaddr_in cli;
    memset(&cli, 0, sizeof(cli));
    int addr_len = sizeof(cli);
    char buff[MAX_MSG];

    int bytes = recvfrom(server_fd, buff, sizeof(buff), 0,
        (struct sockadd*)&cli, (socklen_t*)&addr_len);
    
    if (bytes > 0){
        buff[bytes] = '\0';
        printf("%s\n", buff);
    }

    return NULL;
}

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Invalid number of arguments\n");
        return 0;
    }

    char name[MAX_NAME];
    strcpy(name, argv[1]);
    char* address = malloc(strlen(argv[2]) * sizeof(char));
    strcpy(address, argv[2]);
    int port = atoi(argv[3]);


    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1){
        printf("Socket error\n");
        free(address);
        return 0;
    }

    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr(address);
    srv.sin_port = htons(port);

    char message[] = "NEW ";
    strcat(message, name);
    sendto(server_fd, message, strlen(message), 0,
        (struct sockaddr*)&srv, sizeof(srv));
    
    pthread_t thread;
    pthread_create(&thread, NULL, &thread_fun, &server_fd);

    char buff[MAX_MSG];
    printf("Type command:\n");
    fgets(buff, sizeof(buff), stdin);
    if (buff[strlen(buff) - 1] == '\n'){
        buff[strlen(buff) - 1] = '\0';
    }

    sendto(server_fd, buff, strlen(buff), 0,
        (struct sockaddr*)&srv, sizeof(srv));

    sleep(2);
    close(server_fd);
    free(address);
    return 0;
}