
test: main.c ChessGUI.o 
	gcc -Wall -ansi -std=c99 -lSDL main.c ChessGUI.o -g -o ChessGUI
	./ChessGUI

ChessGUI.o: ChessGUI.c ChessGUI.h 
	gcc -Wall -ansi -std=c99 -lSDL ChessGUI.c -g -c -o ChessGUI.o
	
valgrind: ChessGUI
	valgrind --leak-check=full ./ChessGUI
