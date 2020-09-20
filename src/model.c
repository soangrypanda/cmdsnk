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

struct game_info * init_game_info(void)
{
    int y, x;
    getmaxyx(stdscr, y, x);
    struct game_info *gi = malloc(sizeof(*gi));
    gi->state = game;
    gi->snk = init_snake(y, x); 
    gi->brd = init_board(y, x);
    gi->scenes = set_scenes(y, x);
    return gi;
}

struct board * init_board(int y, int x)
{
    struct board *brd = malloc(sizeof(*brd));
    brd->h = y;
    brd->w = x;
    brd->brd = malloc(brd->w*sizeof(*brd->brd));        
    for(int i = 0, w = brd->w; i < w; i++) {
        brd->brd[i] = malloc(brd->h*sizeof(**brd->brd)); 
    }
    set_brd(brd, empty);
    add_brdrs_2_brd(brd);
    return brd;
}

struct snk_prt * init_snake(int y, int x)
{
    struct snk_prt *snk = malloc(sizeof(*snk));
    snk->y = snk->prev_y = y / 2;
    snk->x = snk->prev_x = x / 2;
    snk->bdy = head;
    snk->next = NULL;
    return snk;
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
