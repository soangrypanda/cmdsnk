OBJECTS=twocoord.o twomap.o
LHEAD=-I ./ -I../oopc
LLIB=-L ./ -L../oopc
LIBS=-loopc
CFLAGS= -g -Wall

main: main.c $(OBJECTS)
	gcc $(CFLAGS) main.c $(OBJECTS)  $(LHEAD) $(LLIB) $(LIBS) -o main
twocoord.o: twocoord.c twocoord.h twocoord.r
	gcc $(CFLAGS) twocoord.c $(LHEAD) $(LLIB) $(LIBS) -c -o twocoord.o
twomap.o: twomap.c twomap.h twomap.r
	gcc $(CFLAGS) twomap.c $(LHEAD) $(LLIB) $(LIBS) -c -o twomap.o
clean:
	rm -f main *.o
