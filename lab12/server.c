#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(address);

    close(server_fd);
    free(address);
    return 0;
}
