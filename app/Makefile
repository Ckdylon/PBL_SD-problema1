CC = gcc
CFLAGS = -I./src/acelerometro -lintelfpgaup
SRC = main.c src/acelerometro/acelerometro.c  
OBJ = main.o acelerometro.o

all: tetris

tetris: $(OBJ)
	$(CC) -o tetris $(OBJ) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)


acelerometro.o: src/acelerometro/acelerometro.c
	$(CC) -c src/acelerometro/acelerometro.c $(CFLAGS)

clean:
	rm -f *.o tetris