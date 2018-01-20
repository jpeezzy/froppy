
all: test

clean:
	rm -f *o
	rm -f test

testgui.o: testgui.c testgui.h
	gcc -c -std=c99 -Wall testgui.c -o testgui.o

boardstate.o: boardstate.c boardstate.h
	gcc -c -std=c99 -Wall boardstate.c -o boardstate.o

movelist.o: movelist.c movelist.h
	gcc -c -std=c99 -Wall movelist.c -o movelist.o

test.o: movelist.h boardstate.h test.c
	gcc -c -std=c99 -Wall test.c -o test.o

test: test.o boardstate.o movelist.o testgui.o
	gcc -std=c99 test.o movelist.o boardstate.o testgui.o -o test


