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

    struct sockaddr_in ser;
    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(9000);
    ser.sin_addr.s_addr = htonl("127.0.0.1");

    int status = connect(gniazdo, (struct sockaddr*)&ser, sizeof(ser));
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
