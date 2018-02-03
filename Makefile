
CC = gcc
CFLAGS = -std=c99 -fopenmp -Wall -g -o2
MATH= -lm

ChessGUI: main.c ChessGUI.o
	$(CC) $(CFLAGS) -lSDL main.c ChessGUI.o -o ChessGUI 
	./ChessGUI	

randGen.o: randGen.h randGen.c
	$(CC) $(CFLAGS) -c randGen.c -o randGen.o $(MATH)

matrix.o: matrix.h matrix.c neuralnet.h
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o $(MATH)

fenToBoardState.o: fenToBoardState.c fenToBoardState.h
	$(CC) $(CFLAGS) -c fenToBoardState.c -o fenToBoardState.o $(MATH)

boardToVector.o: boardToVector.c boardToVector.h
	$(CC) $(CFLAGS) -c boardToVector.c -o boardToVector.o $(MATH)

neuralsave.o: neuralsave.h neuralnet.c
	$(CC) $(CFLAGS) -c neuralsave.c -o neuralsave.o

neuralnet.o: neuralnet.c neuralnet.h matrix.h randGen.h
	$(CC) $(CFLAGS) -c neuralnet.c -o neuralnet.o $(MATH)

openMP_backprop.o: openMP_backprop.c openMP_backprop.h matrix.h neuralnet.h
	$(CC) $(CFLAGS) -c openMP_backprop.c -o openMP_backprop.o $(MATH)

dataEntry.o: dataEntry.c dataEntry.h fenToBoardState.h dataEntry.h
	$(CC) $(CFLAGS) -c dataEntry.c -o dataEntry.o $(MATH)

autoencoder.o: autoencoder.c matrix.h fenToBoardState.h boardToVector.h neuralnet.h openMP_backprop.h dataEntry.h database.h randGen.h neuralsave.h
	$(CC) $(CFLAGS) -c autoencoder.c -o autoencoder.o $(MATH)

auto: matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o autoencoder.o randGen.o neuralsave.o
	$(CC) $(CFLAGS) matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o randGen.o autoencoder.o neuralsave.o -o Auto $(MATH)

matrixTest.o: matrixTest.c matrix.h neuralnet.h
	$(CC) $(CFLAGS) -c matrixTest.c -o matrixTest.o

MT: matrixTest.o matrix.o neuralnet.o randGen.o
	$(CC) $(CFLAGS) matrixTest.o matrix.o neuralnet.o randGen.o -o MT -lm

ChessGUI.o: ChessGUI.h ChessGUI.c
	$(CC) $(CFLAGS) -c -lSDL ChessGUI.c -o ChessGUI.o

clean:
	rm -f *o
	rm -f auto
	rm -f MT
