.POSIX:
CC=cc
CFLAGS=-W -Wall -O3 -Iinclude
LFLAGS=-lm
BIN=ranoise32 ranoise32b avalanche-test

all: $(BIN)
clean:
	rm -f $(BIN)

avalance-test: avalanche-test.c include/muvaror32.h include/testwrite.h
	$(CC) -o avalanche-test $(CFLAGS) avalanche-test.c

ranoise32: ranoise32.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32 $(CFLAGS) ranoise32.c

ranoise32b: ranoise32b.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32b $(CFLAGS) ranoise32b.c
