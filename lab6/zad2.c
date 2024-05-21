#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    double a, b;
    const char* pipe_name = "pipe";
    printf("Type interval:\n");
    scanf("%lf", &a);
    scanf("%lf", &b);

    mkfifo(pipe_name, 0666);
    int fd = open(pipe_name, O_WRONLY);

    write(fd, &a, sizeof(double));
    write(fd, &b, sizeof(double));
    close(fd);

    double integral = 0;
    fd = open(pipe_name, O_RDONLY);
    read(fd, &integral, sizeof(double));
    
    printf("Integral value: %lf\n", integral);
    close(fd);
    return 0;
}
