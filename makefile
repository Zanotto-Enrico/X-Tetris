FLAGS = -O3
COMP = gcc -std=c89

all: X-Tetris clean

debug: FLAGS = -g 
debug: X-Tetris clean

varGlobali.o: varGlobali.c
	$(COMP) varGlobali.c -o varGlobali.o -c $(FLAGS)

grafica.o: grafica.c
	$(COMP) grafica.c -o grafica.o -c $(FLAGS)

eventiPartita.o: eventiPartita.c
	$(COMP) eventiPartita.c -o eventiPartita.o -c $(FLAGS)

funzioniCpu.o: funzioniCpu.c
	$(COMP) funzioniCpu.c -o funzioniCpu.o -c $(FLAGS)

X-Tetris: varGlobali.o grafica.o eventiPartita.o funzioniCpu.o main.c 
	$(COMP) varGlobali.o grafica.o eventiPartita.o main.c funzioniCpu.o -no-pie -o X-Tetris $(FLAGS)

clean:
	rm *.o