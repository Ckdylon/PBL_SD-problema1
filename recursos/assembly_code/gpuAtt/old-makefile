all: build link

build:
	as -o megedMain.o megedMain.s

link:
	ld -o megedMain megedMain.o

clean:
	rm megedMain megedMain.o
