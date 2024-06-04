#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include <pthread.h>

#define BACKLOG 10
#define MAX_CLIENTS 10
#define MAX_NAME 20
#define MAX_MSG 100
#define EMPTY -1

typedef struct {
    char name[MAX_NAME];
    int new_socket;
} client_data;

typedef struct {
    int id;
    int* clients_no;
    client_data* clients;
} thread_args;

void* thread_fun(void* thread_arg){
    thread_args* args = (thread_args*)thread_arg;
    int new_socket = args->clients[args->id].new_socket;
    char buff[MAX_MSG];
    int len;
    int running = 1;

    while (running){
        len = read(new_socket, buff, sizeof(buff));
        buff[len] = '\0';
        
        if (strcmp(buff, "LIST") == 0){
            char* names = (char*)malloc(*(args->clients_no) * MAX_NAME * sizeof(char));
            int offset = 0;

            for (int i = 0; i < MAX_CLIENTS; i++){
                if (args->clients[i].new_socket != EMPTY){
                    strcpy(names + offset, args->clients[i].name);
                    offset += strlen(args->clients[i].name);
                    names[offset] = '\n';
                    offset++;
                }
            }

            names[strlen(names) - 1] = '\0';
            write(new_socket, names, strlen(names));
            free(names);
        }
        else if (strncmp(buff, "2ALL ", 5) == 0){
            char message[MAX_MSG];
            strcpy(message, buff + 5);
            message[strlen(buff) - 5] = '\0';

            for (int i = 0; i < MAX_CLIENTS; i++){
                if (args->clients[i].new_socket != EMPTY){
                    write(args->clients[i].new_socket, message, strlen(message));
                }
            }
        }
        else if (strncmp(buff, "2ONE ", 5) == 0){
            char* name = strtok(buff, " ");
            name = strtok(NULL, " ");
            char* message = strtok(NULL, " ");
            int found = 0;

            for (int i = 0; i < MAX_CLIENTS; i++){
                if (strcmp(args->clients[i].name, name) == 0 &&
                        args->clients[i].new_socket != EMPTY){
                    found = 1;
                    write(args->clients[i].new_socket, message, strlen(message));
                }
            }

            if (!found){
                char* info = (char*)malloc(16 * MAX_NAME * sizeof(char));
                strcpy(info, "No client named ");
                strcat(info, name);
                write(new_socket, info, strlen(info));
                free(info);
            }
        }
        else if (strcmp(buff, "ALIVE") == 0){
            for (int i = 0; i < MAX_CLIENTS; i++){
                if (args->clients[i].new_socket != EMPTY &&
                        args->clients[i].new_socket != new_socket){
                    write(args->clients[i].new_socket, buff, strlen(buff));
                }
            }
        }
        else if (strcmp(buff, "STOP") == 0){
            char message[] = "end";
            write(new_socket, message, strlen(message));
            args->clients[args->id].new_socket = EMPTY;
            (*(args->clients_no))--;
            running = 0;
        }
    }

    return NULL;
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Invalid number of arguments\n");
        return 0;
    }

    char* address = (char*)malloc(strlen(argv[1]) * sizeof(char));
    strcpy(address, argv[1]);
    int port = atoi(argv[2]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1){
        printf("Socket error\n");
        free(address);
        return 0;
    }

    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(address);

    int status = bind(server_fd, (struct sockaddr*)&srv, sizeof(srv));
    if (status == -1){
        printf("Bind error\n");
        close(server_fd);
        free(address);
        return 0;
    }

    status = listen(server_fd, BACKLOG);
    if (status == -1){
        printf("Listen error\n");
        close(server_fd);
        free(address);
        return 0;
    }

    pthread_t threads[MAX_CLIENTS];
    client_data clients[MAX_CLIENTS];
    thread_args args[MAX_CLIENTS];
    int client_no = 0;

    for (int i = 0; i < MAX_CLIENTS; i++){
        clients[i].new_socket = EMPTY;
    }

    while (1){
        struct sockaddr_in cli;
        memset(&cli, 0, sizeof(cli));
        int addrlen = sizeof(cli);
        int new_socket = accept(server_fd, (struct sockaddr*)&cli, (socklen_t*)&addrlen);

        if (client_no == MAX_CLIENTS){
            char comm[] = "rejected";
            write(new_socket, comm, strlen(comm));
            close(new_socket);
            continue;
        }

        char comm[] = "accepted";
        write(new_socket, comm, strlen(comm));

        int idx = 0;
        while (clients[idx].new_socket != EMPTY){
            idx++;
        }
        
        int len = read(new_socket, clients[idx].name, sizeof(clients[idx].name));
        clients[idx].name[len] = '\0';

        clients[idx].new_socket = new_socket;
        args[idx].id = idx;
        args[idx].clients_no = &client_no;
        args[idx].clients = clients;

        client_no++;
        pthread_create(&threads[idx], NULL, &thread_fun, &args[idx]);
    }

    close(server_fd);
    free(address);
    return 0;
}
