CC = gcc
CFLAGS = -std=c99 -fopenmp -Wall -g -o2
MATH= -lm

all: testminmax

ChessGUI: main.c ChessGUI.o movelist.o minmax.o boardstate.o basic_eval.o userHint.o spicyComments.o boardPrep.o
	$(CC) $(CFLAGS) -g -lSDL main.c movelist.o minmax.o boardstate.o ChessGUI.o basic_eval.o userHint.o spicyComments.o boardPrep.o -o ChessGUI

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

boardstate.o: boardstate.c boardstate.h
	$(CC) $(CFLAGS) -c boardstate.c -o boardstate.o 

movelist.o: movelist.c movelist.h boardstate.h
	$(CC) $(CFLAGS) -c movelist.c -o movelist.o

minmax.o: minmax.c minmax.h movelist.h basic_eval.h boardstate.h
	$(CC) $(CFLAGS) -c minmax.c -o minmax.o

testminmax.o: testminmax.c testgui.h minmax.h movelist.h basic_eval.h boardstate.h
	$(CC) $(CFLAGS) -c testminmax.c -o testminmax.o 

testgui.o: testgui.c testgui.h
	$(CC) $(CFLAGS) -c testgui.c -o testgui.o

userHint.o: userHint.c userHint.h minmax.h movelist.h boardstate.h 
	$(CC) $(CFLAGS) -c userHint.c -o userHint.o 

boardPrep.o: boardPrep.c boardPrep.h boardstate.h fenToBoardState.h 
	$(CC) $(CFLAGS) -c boardPrep -o boardPrep.o 

spicyComments.o: spicyComments.h spicyComments.c basic_eval.h boardstate.h 
	$(CC) $(CFLAGS) -c spicyComments.c -o spicyComments.o 

auto: matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o autoencoder.o randGen.o neuralsave.o
	$(CC) $(CFLAGS) matrix.o fenToBoardState.o boardToVector.o neuralnet.o openMP_backprop.o dataEntry.o randGen.o autoencoder.o neuralsave.o -o Auto $(MATH)

matrixTest.o: matrixTest.c matrix.h neuralnet.h
	$(CC) $(CFLAGS) -c matrixTest.c -o matrixTest.o

MT: matrixTest.o matrix.o neuralnet.o randGen.o
	$(CC) $(CFLAGS) matrixTest.o matrix.o neuralnet.o randGen.o -o MT -lm

testminmax: testminmax.o minmax.o basic_eval.o boardstate.o movelist.o testgui.o
	$(CC) $(CFLAGS) testminmax.o minmax.o movelist.o testgui.o basic_eval.o boardstate.o -o testminmax

test.o: test.c boardstate.h movelist.h
	$(CC) $(CFLAGS) -c test.c -o test.o

test: test.o boardstate.o movelist.o testgui.o
	$(CC) $(CFLAGS) test.o boardstate.o movelist.o testgui.o -o test

ChessGUI.o: ChessGUI.h ChessGUI.c
	$(CC) $(CFLAGS) -c -lSDL ChessGUI.c -o ChessGUI.o

clean:
	rm -f *o
	rm -f auto
	rm -f MT
	rm -f testminmax
	rm -f test
	rm -f ChessGUI
