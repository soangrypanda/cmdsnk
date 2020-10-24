OBJECTS=twocoord.o
LHEAD=-I ./ -I../oopc
LLIB=-L ./ -L../oopc
LIBS=-loopc
CFLAGS= -g -Wall

main: main.c $(OBJECTS)
	gcc $(CFLAGS) main.c $(OBJECTS)  $(LHEAD) $(LLIB) $(LIBS) -o main
twocoord.o: twocoord.c twocoord.h twocoord.r
	gcc $(CFLAGS) twocoord.c $(LHEAD) $(LLIB) $(LIBS) -c -o twocoord.o
clean:
	rm -f main *.o
