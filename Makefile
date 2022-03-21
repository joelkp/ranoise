.POSIX:
CC=cc
CFLAGS=-W -Wall -O3
LFLAGS=-lm
BIN=ranoise32
OBJ=ranoise32.o

all: $(BIN)
clean:
	rm -f $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $(BIN)

ranoise32.o: ranoise32.c
	$(CC) -c $(CFLAGS) ranoise32.c
