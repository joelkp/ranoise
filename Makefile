.POSIX:
CC=cc
CFLAGS=-W -Wall -O3 -Iinclude
LFLAGS=-lm
BIN=avalanche-test \
    ranoise32 ranoise32_old ranoise32a ranoise32b ranoise32c \
    lcg32 \
    lcg64 \
    splitmix32 splitmix32a splitmix32b \
    mulberry32 \
    xorshift32

all: $(BIN)
clean:
	rm -f $(BIN)

avalance-test: avalanche-test.c include/muvaror32.h include/testwrite.h
	$(CC) -o avalanche-test $(CFLAGS) avalanche-test.c

ranoise32: ranoise32.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32 $(CFLAGS) ranoise32.c

ranoise32_old: ranoise32_old.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32_old $(CFLAGS) ranoise32_old.c

ranoise32a: ranoise32a.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32a $(CFLAGS) ranoise32a.c

ranoise32b: ranoise32b.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32b $(CFLAGS) ranoise32b.c

ranoise32c: ranoise32c.c include/muvaror32.h include/testwrite.h
	$(CC) -o ranoise32c $(CFLAGS) ranoise32c.c

lcg32: lcg32.c include/testwrite.h
	$(CC) -o lcg32 $(CFLAGS) lcg32.c

lcg64: lcg64.c include/testwrite.h
	$(CC) -o lcg64 $(CFLAGS) lcg64.c

splitmix32: splitmix32.c include/muvaror32.h include/testwrite.h
	$(CC) -o splitmix32 $(CFLAGS) splitmix32.c

splitmix32a: splitmix32a.c include/muvaror32.h include/testwrite.h
	$(CC) -o splitmix32a $(CFLAGS) splitmix32a.c

splitmix32b: splitmix32b.c include/testwrite.h
	$(CC) -o splitmix32b $(CFLAGS) splitmix32b.c

mulberry32: mulberry32.c include/testwrite.h
	$(CC) -o mulberry32 $(CFLAGS) mulberry32.c

xorshift32: xorshift32.c include/testwrite.h
	$(CC) -o xorshift32 $(CFLAGS) xorshift32.c
