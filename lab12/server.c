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
            char client_name[MAX_NAME];

            int ptr = 0;
            while (buff[ptr] != ' '){
                client_name[ptr] = buff[ptr];
                ptr++;
            }

            client_name[ptr] = '\0';
            strcpy(buff, buff + ptr + 1);

            if (strncmp(buff, "NEW", 3) == 0){
                if (clients_no < MAX_CLIENTS){
                    strcpy(clients[clients_no].name, client_name);
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
                int ptr = 0;
                while (strcmp(clients[ptr].name, client_name) != 0){
                    ptr++;
                }

                sendto(server_fd, names, strlen(names), 0,
                    (struct sockaddr*)&clients[ptr].addr, sizeof(clients[ptr].addr));
                free(names);
            }
            else if (strncmp(buff, "2ALL ", 5) == 0 && strlen(buff) > 5){
                for (int i = 0; i < clients_no; i++){
                    if (strcmp(clients[i].name, client_name) != 0){
                        sendto(server_fd, buff + 5, strlen(buff + 5), 0,
                        (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
                    }
                }
            }
            else if (strncmp(buff, "2ONE ", 5) == 0 && strlen(buff) > 5){
                char destination[MAX_NAME];
                int ptr = 5;

                while (buff[ptr] != ' '){
                    destination[ptr - 5] = buff[ptr];
                    ptr++;
                }
                destination[ptr - 5] = '\0';

                int found = 0;
                for (int i = 0; i < clients_no; i++){
                    if (strcmp(clients[i].name, destination) == 0){
                        sendto(server_fd, buff + ptr + 1, strlen(buff + ptr + 1), 0,
                            (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
                        found = 1;
                        break;
                    }
                }

                if (!found){
                    char message[MAX_MSG] = "No client named ";
                    strcat(message, destination);
                    message[strlen(message)] = '\0';
                    
                    ptr = 0;
                    while (strcmp(clients[ptr].name, client_name) != 0){
                        ptr++;
                    }

                    sendto(server_fd, message, strlen(message), 0,
                        (struct sockaddr*)&clients[ptr].addr, sizeof(clients[ptr].addr));
                }
            }
            else if (strcmp(buff, "STOP") == 0){
                printf("%s disconnected\n", client_name);
                int idx = 0;

                while (strcmp(clients[idx].name, client_name) != 0){
                    idx++;
                }

                sendto(server_fd, buff, strlen(buff), 0,
                    (struct sockaddr*)&clients[idx].addr, sizeof(clients[idx].addr));

                for (int i = idx; i < clients_no - 1; i++){
                    strcpy(clients[i].name, clients[i + 1].name);
                    clients[i].addr = clients[i + 1].addr;
                }

                clients_no--;
            }
            else {
                printf("Wrong command %s\n", buff);
            }
        }
    }

    close(server_fd);
    free(address);
    return 0;
}
