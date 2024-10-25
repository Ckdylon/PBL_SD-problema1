LIBOBJS = lib.o

all: main

%.o : %.s
	as $< -o $@

lib.a: $(LIBOBJS)
	ar -cvq lib.a $(LIBOBJS)

main: main.c lib.a
	gcc -o main main.c lib.a
clean:
	rm -f main lib.a $(LIBOBJS)
