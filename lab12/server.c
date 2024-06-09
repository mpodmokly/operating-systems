#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define MAX_NAME 20
#define MAX_MSG 100

typedef struct {
    char name[MAX_NAME];
    struct sockaddr_in addr;
} ClientData;

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Invalid number of arguments\n");
        return 0;
    }

    char* address = (char*)malloc(strlen(argv[1]) * sizeof(char));
    strcpy(address, argv[1]);
    int port = atoi(argv[2]);

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

    int status = bind(server_fd, (struct sockaddr*)&srv, sizeof(srv));
    if (status == -1){
        printf("Bind error\n");
        close(server_fd);
        free(address);
        return 0;
    }

    struct sockaddr_in cli;
    memset(&cli, 0, sizeof(cli));
    ClientData clients[MAX_CLIENTS];
    int clients_no = 0;
    char buff[MAX_MSG];

    while (1){
        int addr_len = sizeof(cli);
        int bytes = recvfrom(server_fd, buff, sizeof(buff), 0,
            (struct sockaddr*)&cli, (socklen_t*)&addr_len);

        if (bytes > 0){
            buff[bytes] = '\0';

            if (strncmp(buff, "NEW ", 4) == 0){
                if (clients_no < MAX_CLIENTS){
                    strcpy(clients[clients_no].name, buff + 4);
                    clients[clients_no].addr = cli;
                    printf("%s connected\n", clients[clients_no].name);
                    clients_no++;
                }
                else {
                    //char message[] = ""
                    //sendto(server_fd, )
                }
            }
            else if (strcmp(buff, "LIST") == 0){
                char* names = (char*)malloc(clients_no * (MAX_NAME + 1) * sizeof(char) + 1);
                int offset = 0;

                for (int i = 0; i < clients_no; i++){
                    strcpy(names + offset, clients[i].name);
                    offset += strlen(clients[i].name);
                    names[offset] = '\n';
                    offset++;
                }

                names[strlen(names) - 1] = '\0';
                //sendto(server_fd, names, strlen(names), 0,
                    //(struct sockaddr*)&clients[])
                free(names);
            }
            else {
                printf("Wrong command\n%s\n", buff);
            }
        }
    }


    close(server_fd);
    free(address);
    return 0;
}
