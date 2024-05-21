#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Invalid number of arguments\n");
        return 0;
    }

    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);
    long double size = strtod(argv[1], NULL);
    int n = atoi(argv[2]);
    long double interval = 1 / (long double)n;

    int fd[2];
    pipe(fd);
    pid_t pid;

    for (int i = 0; i < n; i++){
        pid = fork();

        if (pid == 0){
            close(fd[0]);
            long double start = (1 / (long double)n) * i;
            long double sum = 0;
            long double x = start;

            for (int j = 0; j < (int)(interval / size); j++){
                sum += 4 / (x * x + 1) * size;
                x += size;
            }

            if (interval / size > (int)(interval / size)){
                sum += 4 / (x * x + 1) * (interval - (long long)(interval / size) * size);
            }
            

            write(fd[1], &sum, sizeof(long double));
            close(fd[1]);
            return 0;
        }
    }

    close(fd[1]);
    long double integral = 0;
    long double value;

    for (int i = 0; i < n; i++){
        read(fd[0], &value, sizeof(long double));
        integral += value;
    }

    close(fd[0]);
    printf("Integral value: %0.8Lf\n", integral);
    printf("PI = 3.14159265\n");

    clock_gettime(CLOCK_REALTIME, &end);
    long double used_time = (end.tv_sec - begin.tv_sec) + ((long double)end.tv_nsec - begin.tv_nsec) * 1e-9;
    printf("Time used: %Lf sec.\n", used_time);
    return 0;
}
