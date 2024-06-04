#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define MAX_NAME 20
#define MAX_MSG 100

int server_link = -1;
int running = 1;

void handler(int sig){
    char message[] = "STOP";

    if (server_link != -1){
        write(server_link, message, strlen(message));
    }
    
    running = 0;
}

void* thread_fun(void* arg){
    int server_fd = *(int*)arg;
    char buff[MAX_MSG];
    int len;
    int running = 1;

    while (running){
        len = read(server_fd, buff, sizeof(buff));
        buff[len] = '\0';

        if (strcmp(buff, "end") == 0){
            running = 0;
        }
        else {
            printf("%s\n", buff);
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

    char name[MAX_NAME];
    strcpy(name, argv[1]);
    char* address = malloc(strlen(argv[2]) * sizeof(char));
    strcpy(address, argv[2]);
    int port = atoi(argv[3]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1){
        printf("Socket error\n");
        free(address);
        return 0;
    }
    server_link = server_fd;

    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(address);

    if (srv.sin_addr.s_addr == INADDR_NONE){
        printf("addr error\n");
    }

    int status = connect(server_fd, (struct sockaddr*)&srv, sizeof(srv));
    if (status == -1){
        printf("Connection error\n");
        close(server_fd);
        free(address);
        return 0;
    }

    char buff[MAX_MSG];
    int len;
    len = read(server_fd, buff, sizeof(buff));
    buff[len] = '\0';

    if (strcmp(buff, "rejected") == 0){
        printf("Connection rejected\n");
        close(server_fd);
        free(address);
        return 0;
    }

    printf("Connection accepted\n");
    write(server_fd, name, strlen(name));

    pthread_t thread;
    pthread_create(&thread, NULL, &thread_fun, &server_fd);
    
    printf("Type command:\n");
    while (running){
        fgets(buff, sizeof(buff), stdin);
        if (buff[strlen(buff) - 1] == '\n'){
            buff[strlen(buff) - 1] = '\0';
        }
        write(server_fd, buff, strlen(buff));

        if (strcmp(buff, "LIST") == 0){
            printf("List of server clients:\n");
        }
        else if (strncmp(buff, "2ALL ", 5) == 0){
            printf("Message sent to all clients\n");
        }
        else if (strncmp(buff, "2ONE ", 5) == 0){
            char* token = strtok(buff, " ");
            token = strtok(NULL, " ");
            printf("Message sent to %s\n", token);
        }
        else if (strcmp(buff, "STOP") == 0){
            printf("Client stops\n");
            running = 0;
        }
        else if (strcmp(buff, "ALIVE") == 0){
            printf("Checking clients\n");
        }
        else {
            printf("Wrong command\n");
        }
    }

    pthread_join(thread, NULL);
    close(server_fd);
    free(address);
    return 0;
}
