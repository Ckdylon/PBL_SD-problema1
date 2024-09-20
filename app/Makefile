CC = gcc
CFLAGS = -I./src/tetris -lncurses  
SRC = main.c src/tetris/tetris.c  
OBJ = main.o tetris.o

all: tetris
	./tetris

tetris: $(OBJ)
	$(CC) -o tetris $(OBJ) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

tetris.o: src/tetris/tetris.c
	$(CC) -c src/tetris/tetris.c $(CFLAGS)

clean:
	rm -f *.o tetris
