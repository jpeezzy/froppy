CC = gcc
CFLAGS = -std=c99 -fopenmp -Wall -g -o2
MATH= -lm

ChessGUI: ./src/main.c ./bin/ChessGUI.o ./bin/movelist.o ./bin/minmax.o ./bin/boardstate.o ./bin/basic_eval.o ./bin/userHint.o ./bin/spicyComments.o ./bin/boardPrep.o
	$(CC) $(CFLAGS) -g -lSDL ./src/main.c ./bin/movelist.o ./bin/minmax.o ./bin/boardstate.o ./bin/ChessGUI.o ./bin/basic_eval.o ./bin/userHint.o ./bin/spicyComments.o ./bin/boardPrep.o -o ./bin/Chess

all: testminmax

randGen.o: ./src/randGen.h ./src/randGen.c
	$(CC) $(CFLAGS) -c ./src/randGen.c -o ./bin/randGen.o $(MATH)

matrix.o: ./src/matrix.h ./src/matrix.c ./src/neuralnet.h
	$(CC) $(CFLAGS) -c ./src/matrix.c -o ./bin/matrix.o $(MATH)

fenToBoardState.o: ./src/fenToBoardState.c fenToBoardState.h
	$(CC) $(CFLAGS) -c ./src/fenToBoardState.c -o ./bin/fenToBoardState.o $(MATH)

boardToVector.o: ./src/boardToVector.c ./src/boardToVector.h
	$(CC) $(CFLAGS) -c ./src/boardToVector.c -o ./bin/boardToVector.o $(MATH)

neuralsave.o: ./src/neuralsave.h ./src/neuralnet.c
	$(CC) $(CFLAGS) -c ./src/neuralsave.c -o ./bin/neuralsave.o

neuralnet.o: ./src/neuralnet.c ./src/neuralnet.h ./src/matrix.h ./src/randGen.h
	$(CC) $(CFLAGS) -c ./src/neuralnet.c -o ./bin/neuralnet.o $(MATH)

openMP_backprop.o: ./src/openMP_backprop.c ./src/openMP_backprop.h ./src/matrix.h ./src/neuralnet.h
	$(CC) $(CFLAGS) -c ./src/openMP_backprop.c -o ./bin/openMP_backprop.o $(MATH)

dataEntry.o: ./src/dataEntry.c ./src/dataEntry.h ./src/fenToBoardState.h ./src/dataEntry.h
	$(CC) $(CFLAGS) -c ./src/dataEntry.c -o ./bin/dataEntry.o $(MATH)

autoencoder.o: ./src/autoencoder.c ./src/matrix.h ./src/fenToBoardState.h ./src/boardToVector.h ./src/neuralnet.h ./src/openMP_backprop.h ./src/dataEntry.h ./src/database.h ./src/randGen.h ./src/neuralsave.h
	$(CC) $(CFLAGS) -c ./src/autoencoder.c -o ./bin/autoencoder.o $(MATH)

boardstate.o: ./src/boardstate.c ./src/boardstate.h
	$(CC) $(CFLAGS) -c ./src/boardstate.c -o ./bin/boardstate.o 

movelist.o: ./src/movelist.c ./src/movelist.h ./src/boardstate.h
	$(CC) $(CFLAGS) -c ./src/movelist.c -o ./bin/movelist.o

minmax.o: ./src/minmax.c ./src/minmax.h ./src/movelist.h ./src/basic_eval.h ./src/boardstate.h
	$(CC) $(CFLAGS) -c ./src/minmax.c -o ./bin/minmax.o

testminmax.o: ./src/testminmax.c ./src/testgui.h ./src/minmax.h ./src/movelist.h ./src/basic_eval.h ./src/boardstate.h
	$(CC) $(CFLAGS) -c ./src/testminmax.c -o ./bin/testminmax.o 

testgui.o: ./src/testgui.c ./src/testgui.h
	$(CC) $(CFLAGS) -c testgui.c -o ./bin/testgui.o

userHint.o: ./src/userHint.c ./src/userHint.h ./src/minmax.h ./src/movelist.h ./src/boardstate.h 
	$(CC) $(CFLAGS) -c ./src/userHint.c -o ./bin/userHint.o 

boardPrep.o: ./src/boardPrep.c ./src/boardPrep.h ./src/boardstate.h ./src/fenToBoardState.h 
	$(CC) $(CFLAGS) -c ./src/boardPrep.c -o ./bin/boardPrep.o 

spicyComments.o: ./src/spicyComments.h spicyComments.c ./src/basic_eval.h ./src/boardstate.h 
	$(CC) $(CFLAGS) -c ./src/spicyComments.c -o ./bin/spicyComments.o 

matrixTest.o: ./src/matrixTest.c ./src/matrix.h ./src/neuralnet.h
	$(CC) $(CFLAGS) -c ./src/matrixTest.c -o matrixTest.o

MT: matrixTest.o matrix.o neuralnet.o randGen.o
	$(CC) $(CFLAGS) matrixTest.o matrix.o neuralnet.o randGen.o -o MT -lm

testminmax: ./bin/testminmax.o ./bin/minmax.o ./bin/basic_eval.o ./bin/boardstate.o ./bin/movelist.o ./bin/testgui.o
	$(CC) $(CFLAGS) ./bin/testminmax.o ./bin/minmax.o ./bin/movelist.o ./bin/testgui.o ./bin/basic_eval.o ./bin/boardstate.o -o testminmax

test.o: ./src/test.c ./src/boardstate.h ./src/movelist.h
	$(CC) $(CFLAGS) -c ./src/test.c -o ./bin/test.o

test: ./bin/test.o ./bin/boardstate.o ./bin/movelist.o ./bin/testgui.o
	$(CC) $(CFLAGS) ./bin/test.o ./bin/boardstate.o ./bin/movelist.o ./bin/testgui.o -o test

ChessGUI.o: ./src/ChessGUI.h ./src/ChessGUI.c
	$(CC) $(CFLAGS) -c -lSDL ./src/ChessGUI.c -o ./bin/ChessGUI.o

clean:
	rm -f *o
	rm -f auto
	rm -f MT
	rm -f testminmax
	rm -f test
	rm -f ChessGUI
