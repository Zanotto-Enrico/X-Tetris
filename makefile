FLAGS = -O3
COMP = gcc

all: X-Tetris clean

debug: FLAGS = -g 
debug: X-Tetris clean

varGlobali.o: varGlobali.c
	$(COMP) varGlobali.c -o varGlobali.o -c $(FLAGS)

X-Tetris: varGlobali.o main.c
	gcc varGlobali.o main.c -no-pie -o X-Tetris $(FLAGS)

clean:
	rm *.o