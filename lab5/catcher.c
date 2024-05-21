#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int signals = 0;

void service(int signum, siginfo_t* info, void* context){
    printf("Signal received\n");
    signals++;

    if (info->si_value.sival_int == 1){
        for (int i = 1; i <= 100; i++){
            printf("%d\n", i);
        }
        printf("\n");
    }
    else if (info->si_value.sival_int == 2){
        printf("Received signals: %d\n\n", signals);
    }
    else if (info->si_value.sival_int == 3){
        printf("STOP catcher\n");
        kill(info->si_pid, SIGUSR1);
        exit(EXIT_SUCCESS);
    }
    else {
        printf("Invalid mode\n\n");
    }

    kill(info->si_pid, SIGUSR1);
}

int main(){
    printf("PID: %d\n", (int)getpid());

    struct sigaction sa;
    sa.sa_sigaction = service;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);

    while (1){
        sleep(1);
    }

    return 0;
}
