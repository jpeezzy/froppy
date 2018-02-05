CC = gcc
CFLAGS = -std=c99 -fopenmp -Wall -g -o2
MATH= -lm

ChessGUI: ./src/main.c ./bin/ChessGUI.o ./bin/movelist.o ./bin/minmax.o ./bin/boardstate.o ./bin/basic_eval.o ./bin/userHint.o ./bin/spicyComments.o ./bin/boardPrep.o
	$(CC) $(CFLAGS) -g -lSDL ./src/main.c ./bin/movelist.o ./bin/minmax.o ./bin/boardstate.o ./bin/ChessGUI.o ./bin/basic_eval.o ./bin/userHint.o ./bin/spicyComments.o ./bin/boardPrep.o -o ./bin/ChessGUI

all: testminmax

./bin/randGen.o: ./src/randGen.h ./src/randGen.c
	$(CC) $(CFLAGS) -c ./src/randGen.c -o ./bin/randGen.o $(MATH)

./bin/basic_eval.o: ./src/basic_eval.h ./src/boardstate.h ./src/movelist.h ./src/basic_eval.c
	$(CC) $(CFLAGS) -c ./src/basic_eval.c -o ./bin/basic_eval.o

./bin/fenToBoardState.o: ./src/fenToBoardState.c fenToBoardState.h
	$(CC) $(CFLAGS) -c ./src/fenToBoardState.c -o ./bin/fenToBoardState.o $(MATH)

./bin/dataEntry.o: ./src/dataEntry.c ./src/dataEntry.h ./src/fenToBoardState.h ./src/dataEntry.h
	$(CC) $(CFLAGS) -c ./src/dataEntry.c -o ./bin/dataEntry.o $(MATH)

./bin/boardstate.o: ./src/boardstate.c ./src/boardstate.h
	$(CC) $(CFLAGS) -c ./src/boardstate.c -o ./bin/boardstate.o 

./bin/movelist.o: ./src/movelist.c ./src/movelist.h ./src/boardstate.h
	$(CC) $(CFLAGS) -c ./src/movelist.c -o ./bin/movelist.o

./bin/minmax.o: ./src/minmax.c ./src/minmax.h ./src/movelist.h ./src/basic_eval.h ./src/boardstate.h
	$(CC) $(CFLAGS) -c ./src/minmax.c -o ./bin/minmax.o

./bin/userHint.o: ./src/userHint.c ./src/userHint.h ./src/minmax.h ./src/movelist.h ./src/boardstate.h 
	$(CC) $(CFLAGS) -c ./src/userHint.c -o ./bin/userHint.o 

./bin/boardPrep.o: ./src/boardPrep.c ./src/boardPrep.h ./src/boardstate.h ./src/fenToBoardState.h 
	$(CC) $(CFLAGS) -c ./src/boardPrep.c -o ./bin/boardPrep.o 

./bin/spicyComments.o: ./src/spicyComments.h ./src/spicyComments.c ./src/basic_eval.h ./src/boardstate.h 
	$(CC) $(CFLAGS) -c ./src/spicyComments.c -o ./bin/spicyComments.o 

./bin/ChessGUI.o: ./src/ChessGUI.h ./src/ChessGUI.c
	$(CC) $(CFLAGS) -c -lSDL ./src/ChessGUI.c -o ./bin/ChessGUI.o

clean:
	rm -f ./bin/*.o
	rm -f ./bin/auto
	rm -f ./bin/MT
	rm -f ./bin/testminmax
	rm -f ./bin/test
	rm -f ./bin/Chess
