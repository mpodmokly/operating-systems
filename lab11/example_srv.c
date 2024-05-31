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

    struct sockaddr_in ser, cli;
    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(9000);
    ser.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("127.0.0.1")

    int status = bind(gniazdo, (struct sockaddr*)&ser, sizeof ser);
    if (status == -1){
        printf("Bind error\n");
        return 0;
    }

    status = listen(gniazdo, 10);
    if (status == -1){
        printf("Listen error\n");
        return 0;
    }

    char buff[200];
    int no = 0;
    int end = 1;
    while (end){
        int dlugosc = sizeof cli;
        int gniazdo2 = accept(gniazdo, (struct sockaddr *) &cli, (socklen_t *) &dlugosc);
        if (gniazdo2 == -1){
            printf("Accept error\n");
            return 0;
        }

        read(gniazdo2, buff, sizeof(buff));
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

        write(gniazdo2, buff, strlen(buff));
        close(gniazdo2);
    }

    close(gniazdo);
    printf("KONIEC DZIALANIA SERWERA");
    return 0;
}
