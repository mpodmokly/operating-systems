#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>

#define MSG_TYPE 1
#define MAX_CLIENTS 10
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

    int clients[MAX_CLIENTS];
    int clients_no = 0;

    while (1){
        int is_msg = msgrcv(queue_id, &message, sizeof(message), MSG_TYPE, IPC_NOWAIT);

        if (is_msg == -1){
            for (int i = 0; i < clients_no; i++){
                if (msgrcv(clients[i], &message, sizeof(message), MSG_TYPE, IPC_NOWAIT) != -1){
                    break;
                }
            }
        }
        
        if (strcmp(message.msg_content, "INIT") == 0){
            if (clients_no < MAX_CLIENTS){
                clients[clients_no] = msgget(ftok("server.c", clients_no), IPC_CREAT | 0600);
                message.key = clients_no;

                msgsnd(queue_id, &message, sizeof(message), 0);
                clients_no++;
            }
            else {
                strcpy(message.msg_content, "NO_SPACE");
                msgsnd(queue_id, &message, sizeof(message), 0);
            }
        }
        else if (is_msg != -1) {
            for (int i = 0; i < clients_no; i++){
                msgsnd(clients[i], &message, sizeof(message), 0);
            }
        }

        strcpy(message.msg_content, "");
    }

    return 0;
}
