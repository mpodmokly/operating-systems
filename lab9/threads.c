#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_t thread01, thread02;
int i;

void* thread_fun(void* arg){
    static int a = 10;
    while (1){
        a = a + 1;// in critical section
        printf("%s %d %d\n", (char*)arg, a, i);
        fflush(stdout);
        sleep(1);
    }
}

int main(void){
    pthread_create(&thread01, NULL, &thread_fun, "A");
    pthread_create(&thread02, NULL, &thread_fun, "B");

    for (i = 1; i < 10000; i++){
        sleep(3);
    }

    return 0;
}
