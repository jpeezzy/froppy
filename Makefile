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

openMP_backprop.o: openMP_backprop.c openMP_backprop.h matrix.h neuralnet.h
	$(CC) $(CFLAGS) -c openMP_backprop.c -o openMP_backprop.o

dataEntry.o: dataEntry.c dataEntry.h database.h fenToBoardState.h
	$(CC) $(CFLAGS) -c dataEntry.c -o dataEntry.o

autoencoder.o: autoencoder.c matrix.h fenToBoardState.h boardToVector.h neuralnet.h openMP_backprop.h dataEntry.h database.h
	$(CC) $(CFLAGS) -c autoencoder.c -o autoencoder.o

auto: matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o autoencoder.o
	$(CC) $(CFLAGS) matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o autoencoder.o -o Auto




clean:
	rm -f *o
