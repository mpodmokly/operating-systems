#include <stdio.h>
extern int test_collatz_convergence(int input, int max_iter);

int main(){
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

    printf("%d\n", test_collatz_convergence(input, max_iter));
    return 0;
}
