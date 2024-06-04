#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1){
        printf("Socket error\n");
        return 0;
    }

    struct sockaddr_in srv, cli;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(9000);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("127.0.0.1")

    int status = bind(server_fd, (struct sockaddr*)&srv, sizeof srv);
    if (status == -1){
        printf("Bind error\n");
        return 0;
    }

    status = listen(server_fd, 10);
    if (status == -1){
        printf("Listen error\n");
        return 0;
    }

    char buff[200];
    int no = 0;
    int end = 1;
    while (end){
        memset(&cli, 0, sizeof(cli));
        int len = sizeof cli;
        int new_socket = accept(server_fd, (struct sockaddr*) &cli, (socklen_t*) &len);
        if (new_socket == -1){
            printf("Accept error\n");
            return 0;
        }

        read(new_socket, buff, sizeof(buff));
        if (buff[0] == 'Q'){
            sprintf(buff, "ZGODA, SERWER KONCZY PRACE");
            end = 0;
        }
        else if (buff[0] == 'N'){
            sprintf(buff, "Jestes klientem nr %d", no++);
        }
        else {
            sprintf(buff, "Nie rozumiem pytania");
        }

        write(new_socket, buff, strlen(buff));
        close(new_socket);
    }

    close(server_fd);
    printf("KONIEC DZIALANIA SERWERA");
    return 0;
}
