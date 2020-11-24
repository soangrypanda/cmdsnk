OBJECTS=cmdsnk_gamestate.o cmdsnk_snake.o cmdsnk_food.o rand_mod.o win_mod.o cell_mod.o err_mod.o txt_mod.o elpsd_t.o
HEADERS=elpsd_t.h txt_mod.h win_mod.h cmdsnk_food.h cmdsnk_gamestate.h cmdsnk_snake.h
CFLAGS= -g -Wall -iquote./headers
CLIBS=-lncurses
cmdsnk: src/main.c $(OBJECTS)
	gcc $(CFLAGS) src/main.c $(OBJECTS) $(CLIBS) -o cmdsnk 
cmdsnk_gamestate.o: src/cmdsnk_gamestate.c
	gcc $(CFLAGS) $< -c $(CLIBS)
cmdsnk_snake.o: src/cmdsnk_snake.c
	gcc $(CFLAGS) $< -c $(CLIBS)
cmdsnk_food.o: src/cmdsnk_food.c
	gcc $(CFLAGS) $< -c $(CLIBS)
rand_mod.o: src/rand_mod.c
	gcc $(CFLAGS) $< -c $(CLIBS)
win_mod.o: src/win_mod.c
	gcc $(CFLAGS) $< -c $(CLIBS)
cell_mod.o: src/cell_mod.c
	gcc $(CFLAGS) $< -c $(CLIBS)
err_mod.o: src/err_mod.c
	gcc $(CFLAGS) $< -c $(CLIBS)
txt_mod.o: src/txt_mod.c
	gcc $(CFLAGS) $< -c $(CLIBS)
elpsd_t.o: src/elpsd_t.c
	gcc $(CFLAGS) $< -c $(CLIBS)
    

clean:
	rm -f *.o log* cmdsnk
