int collatz_conjecture(int input){
    if (input % 2 == 0){
        return input / 2;
    }
    else {
        return input * 3 + 1;
    }
}

int test_collatz_convergence(int input, int max_iter){
    int counter = 0;
    int value = input;

    while (value != 1){
        value = collatz_conjecture(value);
        counter++;

        if (counter > max_iter){
            return -1;
        }
    }

    return counter;
}
