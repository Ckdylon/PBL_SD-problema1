LIBOBJS = lab.o

all: main

%.o : %.s
	    as $< -o $@   # Assemble .s files into .o files

lab.a: $(LIBOBJS)
	    ar -cvq lab.a $(LIBOBJS)

main: main.c lab.a
	    gcc -o main main.c lab.a

clean:
	    rm -f main lab.a $(LIBOBJS)

