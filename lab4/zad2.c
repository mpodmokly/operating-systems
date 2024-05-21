#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Invalid number of arguments\n");
        return 0;
    }

    printf("Program name: %s\n\n", argv[0]);
    int local = 0;
    pid_t pid = fork();

    if (pid < 0){
        printf("Forking error\n");
        return pid;
    }
    else if (pid == 0){
        printf("child process\n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d\n", (int)getpid(), (int)getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        return execl("/bin/ls", "ls_name", argv[1], NULL);
    }
    
    int status;
    wait(&status);
    printf("\nparent process\n");
    printf("parent pid = %d, child pid = %d\n", (int)getpid(), (int)pid);
    printf("Child exit code: %d\n", WEXITSTATUS(status));
    printf("Parent's local = %d, parent's global = %d\n", local, global);
    return 0;
}
