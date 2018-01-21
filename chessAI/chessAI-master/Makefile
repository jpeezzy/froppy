#Makefile
FLAGS = -std=c99 -Wall
LINK = -lncurses
all: Chess

ChessGUI.o: ChessGUI.c ChessGUI.h
	gcc $(FLAGS) -c ChessGUI.c -o ChessGUI.o $(LINK)

ChessMov.o: ChessMov.c ChessMov.h
	gcc $(FLAGS) -c ChessMov.c -o ChessMov.o

Chess.o: Chess.c ChessGUI.h ChessMov.h
	gcc $(FLAGS) -c Chess.c -o Chess.o

Chess: Chess.o ChessGUI.o ChessMov.o 
	gcc $(FLAGS) Chess.o ChessGUI.o ChessMov.o -o Chess -WL $(LINK)

clean:
	rm -f *o
	rm -f Chess
