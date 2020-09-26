SRCMODULES = model.c scene.c controller.c view.c
OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -g -Wall
CLIBS = -lncurses

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

cmd_snk: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@ $(CLIBS)

clean:
	rm -f *.o cmd_snk celln.txt
