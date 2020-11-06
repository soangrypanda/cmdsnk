CFLAGS= -g -Wall
CLIBS=-lncurses
main: main.c $(OBJECTS)
	gcc $(CFLAGS) main.c $(OBJECTS) $(CLIBS) -o main

clean:
	rm -f main *.o log*
