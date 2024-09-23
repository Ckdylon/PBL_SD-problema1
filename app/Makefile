CC = gcc
CFLAGS = -I./src/tetris -I./src/acelerometro 
SRC = main.c src/tetris/tetris.c src/acelerometro/acelerometro.c  
OBJ = main.o tetris.o acelerometro.o

all: tetris

tetris: $(OBJ)
	$(CC) -o tetris $(OBJ) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

tetris.o: src/tetris/tetris.c
	$(CC) -c src/tetris/tetris.c $(CFLAGS)

acelerometro.o: src/acelerometro/acelerometro.c
	$(CC) -c src/acelerometro/acelerometro.c $(CFLAGS)

clean:
	rm -f *.o tetris
