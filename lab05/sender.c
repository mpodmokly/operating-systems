#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

void service(int signal){
    printf("Signal confirmation received\n");
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("Invalid number of arguments\n");
        return 0;
    }

    pid_t pid = (pid_t)atoi(argv[1]);
    int mode = atoi(argv[2]);

    signal(SIGUSR1, service);

    union sigval value;
    value.sival_int = mode;
    sigqueue(pid, SIGUSR1, value);
    
    sigset_t mask;
    sigemptyset(&mask);
    sigsuspend(&mask);

    return 0;
}
