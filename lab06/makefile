CFLAGS = -Wall

all: zad1 zad2

zad1: zad1.c
	gcc $(CFLAGS) -o zad1 zad1.c

zad2: zad2.c integral.c
	gcc $(CFLAGS) -o zad2 zad2.c
	gcc $(CFLAGS) -o integral integral.c

.PHONY: clean
clean:
	rm -f zad1 zad2 pipe integral
