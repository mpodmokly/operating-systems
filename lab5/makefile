CFLAGS = -Wall

all: zad1 zad2

zad1: zad1.c
	gcc $(CFLAGS) -o zad1 zad1.c

zad2: catcher.c sender.c
	gcc $(CFLAGS) -o catcher catcher.c
	gcc $(CFLAGS) -o sender sender.c

.PHONY: clean
clean:
	rm -f zad1 catcher sender
