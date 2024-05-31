#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include <locale.h>
#include <stdbool.h>
#include <ncurses.h>
#include <pthread.h>

#include "grid.h"

typedef struct {
	int start;
	int width;
	char* foreground;
	char* background;
} thread_args;

void sig_handler(int sig){}

void* thread_fun(void* thread_arg){
	thread_args* args = (thread_args*)thread_arg;
	char* tmp;

	while (true){
		pause();

		for (int i = 0; i < args->width; i++){
			int cell = args->start + i;
			(args->background)[cell] = is_alive(cell / GRID_WIDTH,
				cell % GRID_WIDTH, args->foreground);
		}

		tmp = args->foreground;
		args->foreground = args->background;
		args->background = tmp;
	}
	
	return NULL;
}

int main(int argc, char* argv[]){
	if (argc != 2){
        printf("Invalid number of arguments\n");
        return 0;
    }

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	init_grid(foreground);

	int n = atoi(argv[1]);
	int update_width = GRID_WIDTH * GRID_HEIGHT / n;
	pthread_t* threads = malloc(n * sizeof(pthread_t));
	thread_args* args = malloc(n * sizeof(thread_args));
	signal(SIGUSR1, sig_handler);

	for (int i = 0; i < n; i++){
		args[i].width = update_width;
		if (i == n - 1){
			int rest = (GRID_WIDTH * GRID_HEIGHT) % n;
			for (int j = 0; j < rest; j++){
				args[j].width++;
				args[j].start += j;
			}
			args[rest].start += rest;
		}

		args[i].start = i * update_width;
		args[i].background = background;
		args[i].foreground = foreground;
		pthread_create(&threads[i], NULL, &thread_fun, &args[i]);
	}


	while (true){
		draw_grid(foreground);

		for (int i = 0; i < n; i++){
			pthread_kill(threads[i], SIGUSR1);
		}

		usleep(500 * 1000);

		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	free(threads);
	free(args);
	return 0;
}
