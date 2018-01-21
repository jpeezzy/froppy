# for testing minmax
all: minmax

minmax.o: minmax.c minmax.h basic_eval.h boardstate.h movelist.h
	gcc -g -c -Wall -ansi -std=c99 minmax.c -o minmax.o

minmax: minmax.o
	gcc -g  minmax.o -o minmax
