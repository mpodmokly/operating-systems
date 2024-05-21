#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#define BUFFER_SIZE 10

int main(){
    FILE* in_file = fopen("input.txt", "r");
    if (!in_file){
        printf("Failed to open file\n");
        return 0;
    }

    fseek(in_file, 0, SEEK_END);
    int size = ftell(in_file);
    fseek(in_file, -1, SEEK_CUR);

    FILE* out_file_1 = fopen("input_reversed_1.txt", "w");
    if (!out_file_1){
        printf("Failed to open file\n");
        return 0;
    }

    for (int i = 0; i < size; i++){
        fputc(fgetc(in_file), out_file_1);
        if (i != size - 1){
            fseek(in_file, -2, SEEK_CUR);
        }
    }

    fclose(out_file_1);
    FILE* out_file_1024 = fopen("input_reversed_1024.txt", "w");
    if (!out_file_1024){
        printf("Failed to open file\n");
        return 0;
    }

    fseek(in_file, -BUFFER_SIZE, SEEK_END);
    int iter = size / BUFFER_SIZE;
    char buffer[BUFFER_SIZE];

    for (int i = 0; i < iter; i++){
        fread(buffer, sizeof(char), BUFFER_SIZE, in_file);

        for (int j = 0; j < BUFFER_SIZE / 2; j++){
            char temp;
            temp = buffer[j];
            buffer[j] = buffer[BUFFER_SIZE - j - 1];
            buffer[BUFFER_SIZE - j - 1] = temp;
        }

        fwrite(buffer, sizeof(char), BUFFER_SIZE, out_file_1024);

        if (i != iter - 1){
            fseek(in_file, -BUFFER_SIZE * (i + 2), SEEK_END);
        }
    }

    fseek(in_file, 0, SEEK_SET);
    fread(buffer, sizeof(char), size - iter * BUFFER_SIZE, in_file);

    for (int j = 0; j < (size - iter * BUFFER_SIZE) / 2; j++){
        char temp;
        temp = buffer[j];
        buffer[j] = buffer[size - iter * BUFFER_SIZE - j - 1];
        buffer[size - iter * BUFFER_SIZE - j - 1] = temp;
    }

    fwrite(buffer, sizeof(char), size - iter * BUFFER_SIZE, out_file_1024);
    fclose(out_file_1024);
    fclose(in_file);

    DIR* directory = opendir(".");
    if (!directory){
        printf("Failed to open directory\n");
        return 0;
    }

    struct dirent* entry;
    struct stat bufor_stat;
    long long int total_size = 0;

    while ((entry = readdir(directory))){
        stat(entry->d_name, &bufor_stat);

        if (!S_ISDIR(bufor_stat.st_mode)){
            total_size += bufor_stat.st_size;
            printf("%ld %s\n", bufor_stat.st_size, entry->d_name);
        }
    }

    printf("%lld total\n", total_size);
    closedir(directory);
    return 0;
}
