CC = gcc
CFLAGS = -std=c99 -fopenmp -Wall -g -DDEBUG
MATH= -lm 

randGen.o: randGen.h randGen.c
	$(CC) $(CFLAGS) -c randGen.c -o randGen.o $(MATH)

matrix.o: matrix.h matrix.c neuralnet.h
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o $(MATH)

fenToBoardState.o: fenToBoardState.c fenToBoardState.h
	$(CC) $(CFLAGS) -c fenToBoardState.c -o fenToBoardState.o $(MATH)

boardToVector.o: boardToVector.c boardToVector.h
	$(CC) $(CFLAGS) -c boardToVector.c -o boardToVector.o $(MATH)

neuralnet.o: neuralnet.c neuralnet.h matrix.h randGen.h
	$(CC) $(CFLAGS) -c neuralnet.c -o neuralnet.o $(MATH)

openMP_backprop.o: openMP_backprop.c openMP_backprop.h matrix.h neuralnet.h
	$(CC) $(CFLAGS) -c openMP_backprop.c -o openMP_backprop.o $(MATH)

dataEntry.o: dataEntry.c dataEntry.h fenToBoardState.h
	$(CC) $(CFLAGS) -c dataEntry.c -o dataEntry.o $(MATH)

autoencoder.o: autoencoder.c matrix.h fenToBoardState.h boardToVector.h neuralnet.h openMP_backprop.h dataEntry.h database.h randGen.h
	$(CC) $(CFLAGS) -c autoencoder.c -o autoencoder.o $(MATH)

auto: matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o autoencoder.o randGen.o
	$(CC) $(CFLAGS) matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o randGen.o autoencoder.o -o Auto $(MATH)

matrixTest.o: matrixTest.c matrix.h neuralnet.h
	$(CC) $(CFLAGS) -c matrixTest.c -o matrixTest.o

MT: matrixTest.o matrix.o neuralnet.o randGen.o
	$(CC) $(CFLAGS) matrixTest.o matrix.o neuralnet.o randGen.o -o MT -lm

ChessGUI.o: ChessGUI.h ChessGUI.c
	gcc -Wall -ansi -std=c99 -lSDL ChessGUI.c -g -c -o ChessGUI.o

ChessGUI: ChessGUI.o main.c
	gcc -Wall -ansi -std=c99 -lSDL main.c ChessGUI.o -g -o ChessGUI

clean:
	rm -f *o
	rm -f auto
	rm -f MT
	rm -f ChessGUI
