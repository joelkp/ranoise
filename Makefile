.POSIX:
CC=cc
CFLAGS=-W -Wall -O3
LFLAGS=-lm
BIN=ranoise32 avalanche-test

all: $(BIN)
clean:
	rm -f $(BIN)

avalance-test: avalanche-test.c
	$(CC) -o avalanche-test $(CFLAGS) avalanche-test.c

ranoise32: ranoise32.c
	$(CC) -o ranoise32 $(CFLAGS) ranoise32.c
