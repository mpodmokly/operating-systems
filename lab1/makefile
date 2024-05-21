CXX=gcc
CFLAGS = -Wall -std=c17 -g

all: countdown

countdown: countdown.c
	$(CXX) $(CFLAGS) -o countdown countdown.c

.PHONY: clean
clean:
	rm -f countdown
