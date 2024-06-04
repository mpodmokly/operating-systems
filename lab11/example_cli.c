#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    int gniazdo = socket(AF_INET, SOCK_STREAM, 0);
    if (gniazdo == -1){
        printf("Socket error\n");
        return 0;
    }

    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(9000);
    srv.sin_addr.s_addr = htonl("127.0.0.1");

    int status = connect(gniazdo, (struct sockaddr*)&srv, sizeof(srv));
    if (status == -1){
        printf("Connect error\n");
        return 0;
    }

    char buff[200];
    printf("Podaj tekst:");
    fgets(buff, sizeof(buff), stdin);
    status = write(gniazdo, buff, strlen(buff));
    status = read(gniazdo, buff, sizeof(buff) - 1);
    buff[status] = '\0';
    printf("Otrzymalem %s\n", buff);

    close(gniazdo);
    printf("KONIEC DZIALANIA KLIENTA\n");
    return 0;
}
