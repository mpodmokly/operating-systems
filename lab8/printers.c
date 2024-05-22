#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define SHM_SIZE 16
#define MSG_SIZE 10

void user(int shm_id, int sem_id){
    char* shm_ptr = shmat(shm_id, NULL, 0);
    srand(time(NULL) + getpid());
    char message[MSG_SIZE];
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_flg = 0;

    while (1){
        for (int i = 0; i < MSG_SIZE; i++){
            message[i] = rand() % 26 + 97;
        }
        
        operation.sem_op = -1;
        semop(sem_id, &operation, 1);

        if (shm_ptr[0] == 0){
            sprintf(shm_ptr, "%s", message);
        }
        
        operation.sem_op = 1;
        semop(sem_id, &operation, 1);
        sleep(rand() % 20);
    }
}

void printer(int shm_id, int sem_id){
    char* shm_ptr = shmat(shm_id, NULL, 0);
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_flg = 0;
    char buffor[MSG_SIZE];

    while (1){
        operation.sem_op = -1;
        semop(sem_id, &operation, 1);

        if (shm_ptr[0] != 0){
            strcpy(buffor, shm_ptr);

            shm_ptr[0] = 0;
            operation.sem_op = 1;
            semop(sem_id, &operation, 1);

            for (int i = 0; i < MSG_SIZE; i++){
                printf("%c", buffor[i]);
                sleep(1);
            }
            printf("\n");
        }
        else {
            operation.sem_op = 1;
            semop(sem_id, &operation, 1);
        }
    }
}

int main(){
    //setbuf(stdout, NULL);
    int n, m;

    printf("Enter number of users:\n");
    scanf("%d", &n);
    printf("Enter number of printers:\n");
    scanf("%d", &m);

    key_t key = ftok("printers.c", 'p');
    int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    char* shm_ptr = shmat(shm_id, NULL, 0);
    shm_ptr[0] = 0;

    key = ftok("printers.c", 'q');
    int sem_id = semget(key, 1, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, 1);

    pid_t pid;
    for (int i = 0; i < n; i++){
        pid = fork();

        if (pid == 0){
            user(shm_id, sem_id);
            return 0;
        }
    }

    for (int i = 0; i < m; i++){
        pid = fork();

        if (pid == 0){
            printer(shm_id, sem_id);
            return 0;
        }
    }

    while (wait(NULL) > 0);
    return 0;
}
