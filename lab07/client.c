#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>

#define MSG_TYPE 1
#define MAX_MESSAGE 200

struct msg_buffer {
    long msg_type;
    char msg_content[MAX_MESSAGE];
    int key;
};

int main(){
    key_t key = ftok("server.c", 'p');
    int queue_id = msgget(key, IPC_CREAT | 0600);

    struct msg_buffer message;
    message.msg_type = MSG_TYPE;
    strcpy(message.msg_content, "INIT");

    msgsnd(queue_id, &message, sizeof(message), 0);
    msgrcv(queue_id, &message, sizeof(message), MSG_TYPE, 0);

    if (strcmp(message.msg_content, "NO_SPACE") != 0){
        pid_t pid = fork();

        if (pid == 0){
            queue_id = msgget(ftok("server.c", message.key), IPC_CREAT | 0600);

            while (1){
                msgrcv(queue_id, &message, sizeof(message), MSG_TYPE, 0);
                printf("%s\n", message.msg_content);
            }
        }
        else {
            while (1){
                printf("Type your message:\n");
                scanf("%s", message.msg_content);
                msgsnd(queue_id, &message, sizeof(message), 0);
                sleep(1);
            }
        }
    }
    else {
        printf("No available resources in server\n");
    }

    return 0;
}
