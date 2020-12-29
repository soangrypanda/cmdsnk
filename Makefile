SRCDIR = src
OBJDIR = obj
SRC = $(wildcard ./$(SRCDIR)/*.c)
OBJ = $(patsubst ./$(SRCDIR)/%.c,./$(OBJDIR)/%.o,$(SRC))
HEADERS=elpsd_t.h txt_mod.h win_mod.h cmdsnk_food.h cmdsnk_gamestate.h cmdsnk_snake.h
CFLAGS= -g -Wall -iquote./headers -lncurses

vpath %.c src
vpath %.h headers

cmdsnk: $(OBJ) $(HEADERS)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

$(OBJ): ./$(OBJDIR)/%.o: ./$(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)  

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -f log* cmdsnk
	rm -fr $(OBJDIR) 
