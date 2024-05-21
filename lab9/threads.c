#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_t watek01, watek02;
int i;

void* fun_watka(void* cos){
    static int a = 10;
    while (1){
        a = a + 1;//w sekcji krytycznej
        printf("%s %d %d\n", (char*)cos, a, i);
        fflush(stdout);
        sleep(1);
    }
}

int main(void){
    pthread_create (&watek01, NULL, &fun_watka, "A");
    pthread_create (&watek02, NULL, &fun_watka, "B");

    for (i = 1; i < 10000; i++){
        sleep(3);
    }

    return 0;
}
