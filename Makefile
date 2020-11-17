OBJECTS=cmdsnk_gamestate.o rand_mod.o
HEADERS=elpsd_t.h txt_mod.h win_mod.h cmdsnk_food.h cmdsnk_gamestate.h
CFLAGS= -g -Wall
CLIBS=-lncurses
main: main.c $(OBJECTS) $(HEADERS)
	gcc $(CFLAGS) main.c $(OBJECTS) $(CLIBS) -o main
cmdsnk_gamestate.o: cmdsnk_gamestate.c $(HEADERS) 
	gcc $(CFLAGS) cmdsnk_gamestate.c -c $(CLIBS)
rand_mod.o: rand_mod.c $(HEADERS) 
	gcc $(CFLAGS) rand_mod.c -c $(CLIBS)
    

clean:
	rm -f main *.o log*
