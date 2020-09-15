#include <ncurses.h>
#include <stdlib.h>
#include "model.h"


void init_ncurses(void)
{
    initscr();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
}

void init_board(struct board *brd)
{
    brd->brd = malloc(brd->w*sizeof(*brd->brd));        
    for(int i = 0, w = brd->w; i < w; i++) {
        brd->brd[i] = malloc(brd->h*sizeof(**brd->brd)); 
    }
}

void init_snake(struct snk_prt *snk, int y, int x)
{
    snk->y = y / 2;
    snk->x = x / 2;
    snk->bdy = head;
}
