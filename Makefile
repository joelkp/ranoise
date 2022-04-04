.POSIX:
CC=cc
CFLAGS=-W -Wall -O3 -Iinclude
LFLAGS=-lm
BIN=avalanche-test \
    ranoise32 ranoise32b \
    xorshift32

all: $(BIN)
clean:
	rm -f $(BIN)

avalance-test: avalanche-test.c include/muvaror32.h include/testwrite.h
	$(CC) -o avalanche-test $(CFLAGS) avalanche-test.c

ranoise32: ranoise32.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32 $(CFLAGS) ranoise32.c

ranoise32b: ranoise32b.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32b $(CFLAGS) ranoise32b.c

xorshift32: xorshift32.c include/testwrite.h
	$(CC) -o xorshift32 $(CFLAGS) xorshift32.c
