all: server.c client.c
	gcc -Wall server.c -o server -lpthread
	gcc -Wall client.c -o client -lpthread

.PHONY: clean
clean:
	rm -f server client
