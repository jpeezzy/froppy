CC = gcc
CFLAGS = -std=c99 -fopenmp -Wall
MATH= -lm
matrix.o: matrix.h matrix.c
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o $(MATH)

fenToBoardState.o: fenToBoardState.c fenToBoardState.h
	$(CC) $(CFLAGS) -c fenToBoardState.c -o fenToBoardState.o $(MATH)

boardToVector.o: boardToVector.c boardToVector.h
	$(CC) $(CFLAGS) -c boardToVector.c -o boardToVector.o $(MATH)

neuralnet.o: neuralnet.c neuralnet.h matrix.h
	$(CC) $(CFLAGS) -c neuralnet.c -o neuralnet.o $(MATH)

openMP_backprop.o: openMP_backprop.c openMP_backprop.h matrix.h neuralnet.h
	$(CC) $(CFLAGS) -c openMP_backprop.c -o openMP_backprop.o $(MATH)

dataEntry.o: dataEntry.c dataEntry.h fenToBoardState.h
	$(CC) $(CFLAGS) -c dataEntry.c -o dataEntry.o $(MATH)

autoencoder.o: autoencoder.c matrix.h fenToBoardState.h boardToVector.h neuralnet.h openMP_backprop.h dataEntry.h database.h
	$(CC) $(CFLAGS) -c autoencoder.c -o autoencoder.o $(MATH)

auto: matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o autoencoder.o
	$(CC) $(CFLAGS) matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o autoencoder.o -o Auto $(MATH)




clean:
	rm -f *o
	rm -f auto
