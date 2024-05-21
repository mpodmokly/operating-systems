#include <dlfcn.h>
#include <stdio.h>

int main(){
    void* link = dlopen("./libcollatz.so", RTLD_LAZY);
    if (!link){
        printf("Library opening error\n");
        return 0;
    }

    int (*collatz)(int, int);
    collatz = dlsym(link, "test_collatz_convergence");
    if (dlerror() != 0){
        printf("collatz function error\n");
        return 0;
    }

    int input, max_iter;
    printf("Type input value: ");
    scanf("%d", &input);
    if (input <= 0){
        printf("Bad input value\n");
        return 0;
    }

    printf("Type max iterations: ");
    scanf("%d", &max_iter);
    if (max_iter <= 0){
        printf("Bad max_iter value\n");
        return 0;
    }

    printf("%d\n", collatz(input, max_iter));
    dlclose(link);
    return 0;
}
