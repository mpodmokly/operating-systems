#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Invalid number of arguments\n");
        return 0;
    }

    int count = atoi(argv[1]);

    for (int i = 0; i < count; i++){
        pid_t pid = fork();

        if (pid == 0){
            printf("Parent process ID: %d, process ID: %d\n", (int)getppid(), (int)getpid());
            return 0;
        }
    }
    
    //while (wait(NULL) > 0);
    printf("argv[1] = %d\n", count);
    return 0;
}
