# for testing minmax

all: minmax

minmax.o: minmax.c minmax.h movelist.h basic_eval.h boardstate.h
	gcc -g -c -fopenmp -Wall -ansi -std=c99 minmax.c -o minmax.o

clean:
	rm minmax.o
