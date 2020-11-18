OBJECTS=cmdsnk_gamestate.o cmdsnk_snake.o cmdsnk_food.o rand_mod.o win_mod.o cell_mod.o
HEADERS=elpsd_t.h txt_mod.h win_mod.h cmdsnk_food.h cmdsnk_gamestate.h cmdsnk_snake.h
CFLAGS= -g -Wall
CLIBS=-lncurses
main: main.c $(OBJECTS) $(HEADERS)
	gcc $(CFLAGS) main.c $(OBJECTS) $(CLIBS) -o main
cmdsnk_gamestate.o: cmdsnk_gamestate.c $(HEADERS) 
	gcc $(CFLAGS) cmdsnk_gamestate.c -c $(CLIBS)
cmdsnk_snake.o: cmdsnk_snake.c $(HEADERS) 
	gcc $(CFLAGS) cmdsnk_snake.c -c $(CLIBS)
cmdsnk_food.o: cmdsnk_food.c $(HEADERS) 
	gcc $(CFLAGS) cmdsnk_food.c -c $(CLIBS)
rand_mod.o: rand_mod.c $(HEADERS) 
	gcc $(CFLAGS) rand_mod.c -c $(CLIBS)
win_mod.o: win_mod.c $(HEADERS) 
	gcc $(CFLAGS) win_mod.c -c $(CLIBS)
cell_mod.o: cell_mod.c $(HEADERS) 
	gcc $(CFLAGS) cell_mod.c -c $(CLIBS)
    

clean:
	rm -f main *.o log*
