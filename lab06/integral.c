#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    const char* pipe_name = "pipe";
    int fd = open(pipe_name, O_RDONLY);

    double a, b;
    read(fd, &a, sizeof(double));
    read(fd, &b, sizeof(double));
    close(fd);

    double sum = 0;
    double size;
    double x = a;
    printf("Type rectangle size:\n");
    scanf("%lf", &size);

    for (int i = 0; i < (int)((b - a) / size); i++){
        sum += 4 / (x * x + 1) * size;
        x += size;
    }

    if ((b - a) / size > (int)((b - a) / size)){
        sum += 4 / (x * x + 1) * (b - a - (int)((b - a) / size) * size);
    }

    fd = open(pipe_name, O_WRONLY);
    write(fd, &sum, sizeof(double));
    close(fd);
    return 0;
}
