#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
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
    brd->cells_n = brd->w * brd->h;
}

void init_snake(struct snk_prt *snk, int y, int x)
{
    snk->y = y / 2;
    snk->x = x / 2;
    snk->bdy = head;
    snk->next = NULL;
}

void set_brd(struct board *brd, enum brd_symb symb)
{
    for(int i=0, w=brd->w; i<w; i++) {
        for(int j=0, h=brd->h; j < h; j++) {
            brd->brd[i][j] = symb; 
        }
    }
}


void add_brdrs_2_brd(struct board *brd)
{
    int w = brd->w;
    int h = brd->h;
    enum brd_symb **brdptr = brd->brd;
    for(int i = 0; i < w; i++) {
        brdptr[i][0] = brdr;
        brdptr[i][h-1] = brdr;       
    }
    for(int j = 0; j < h; j++) {
        brdptr[0][j] = brdr;
        brdptr[w-1][j] = brdr;
    }
}
