#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void service(int signum){
    printf("Signal handled\n");
}

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Invalid number of arguments\n");
        return 0;
    }

    if (strcmp(argv[1], "ignore") == 0){
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(argv[1], "handler") == 0){
        signal(SIGUSR1, service);
    }
    else if (strcmp(argv[1], "mask") == 0){
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        printf("Signal masked\n");
    }
    else if (strcmp(argv[1], "none") != 0){
        printf("Invalid argument\n");
        return 0;
    }

    raise(SIGUSR1);

    if (strcmp(argv[1], "mask") == 0){
        sigset_t pending_signals;
        sigpending(&pending_signals);

        if (sigismember(&pending_signals, SIGUSR1)){
            printf("Signal pending\n");
        }
        else {
            printf("Signal not pending\n");
        }
    }

    return 0;
}
