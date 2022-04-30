CC = gcc
CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline

objs = main.o minmax.o stack.o tabuleiro.o shared_funcs.o

all: main

main: $(objs)
	$(CC) $(CFLAGS) $(objs) -o floodit $(LDLIBS)

minmax: minmax.c minmax.h stack.h shared_funcs.h
	$(CC) $(CFLAGS) minmax.c $(LDLIBS)

stack: stack.c stack.h shared_funcs.h
	$(CC) $(CFLAGS) stack.c $(LDLIBS)

tabuleiro: tabuleiro.c tabuleiro.h
	$(CC) $(CFLAGS) tabuleiro.c $(LDLIBS)

shared_funcs: shared_funcs.c shared_funcs.h
	$(CC) $(CFLAGS) shared_funcs.c $(LDLIBS)

clean:
	rm -f *.o 

purge: clean
	rm -f main