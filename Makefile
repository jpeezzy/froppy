CC = gcc
CFLAGS = -std=c99 -fopenmp -lm -Wall

matrix.o: matrix.h matrix.c
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

fenToBoardState.o: fenToBoardState.c fenToBoardState.h
	$(CC) $(CFLAGS) -c fenToBoardState.c -o fenToBoardState.o

boardToVector.o: boardToVector.c boardToVector.h
	$(CC) $(CFLAGS) -c boardToVector.c -o boardToVector.o

neuralnet.o: neuralnet.c neuralnet.h matrix.h
	$(CC) $(CFLAGS) -c neuralnet.c -o neuralnet.o








clean:
	rm -f *o
