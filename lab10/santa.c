#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define REINDEERS 9
#define LOOPS 4

int waiting = 0;
int gifts = 0;

pthread_mutex_t waiting_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gifts_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waiting_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t gifts_cond = PTHREAD_COND_INITIALIZER;

typedef struct {
    int id;
    pthread_t santa_thread;
} reindeer_args;

void sig_handler(int sig){}

void* reindeer(void* arg){
    reindeer_args* args = (reindeer_args*)arg;
    srand(time(NULL) + args->id);

    for (int i = 0; i < LOOPS; i++){
        pthread_mutex_lock(&waiting_mutex);
        while (waiting != 0){
            pthread_cond_wait(&waiting_cond, &waiting_mutex);
        }

        pthread_mutex_unlock(&waiting_mutex);
        sleep(rand() % 6 + 5);// wait 5-10s (holiday)

        pthread_mutex_lock(&waiting_mutex);
        waiting++;
        pthread_cond_broadcast(&waiting_cond);
        printf("Renifer: czeka %d reniferow na Mikolaja, ID: %d\n", waiting, args->id);

        if (waiting == REINDEERS){
            printf("Renifer: wybudzam Mikolaja, ID: %d\n", args->id);
            pthread_kill(args->santa_thread, SIGUSR1);
        }
        else {
            while (waiting != REINDEERS){
                pthread_cond_wait(&waiting_cond, &waiting_mutex);
            }
        }

        pthread_mutex_unlock(&waiting_mutex);

        pthread_mutex_lock(&gifts_mutex);
        while (gifts == 0){
            pthread_cond_wait(&gifts_cond, &gifts_mutex);
        }

        pthread_mutex_unlock(&gifts_mutex);
        sleep(gifts);
    }
    
    return NULL;
}

void* santa(void* arg){
    signal(SIGUSR1, sig_handler);

    for (int i = 0; i < LOOPS; i++){
        pause();
        printf("Mikolaj: budze sie\n");
        printf("Mikolaj: dostarczam zabawki...\n");
        
        gifts = rand() % 3 + 2;// wait 2-4s (gifts)
        pthread_cond_broadcast(&gifts_cond);
        sleep(gifts);
        
        pthread_mutex_lock(&waiting_mutex);
        waiting = 0;
        pthread_cond_broadcast(&waiting_cond);
        pthread_mutex_unlock(&waiting_mutex);

        printf("Mikolaj: zasypiam\n");
    }

    return NULL;
}

int main(){
    pthread_t santa_thread;
    pthread_t reindeers_threads[REINDEERS];
    reindeer_args args[REINDEERS];

    pthread_create(&santa_thread, NULL, &santa, NULL);
    for (int i = 0; i < REINDEERS; i++){
        args[i].id = i;
        args[i].santa_thread = santa_thread;
        pthread_create(&reindeers_threads[i], NULL, &reindeer, &args[i]);
    }

    pthread_join(santa_thread, NULL);
    for (int i = 0; i < REINDEERS; i++){
        pthread_join(reindeers_threads[i], NULL);
    }

    printf("Koniec\n");
    return 0;
}
