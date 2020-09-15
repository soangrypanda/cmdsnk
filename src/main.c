#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "model.h"

int main(void)
{
    int key;
    struct board *brd1 = malloc(sizeof(*brd1));
    struct snk_prt *snk1 = malloc(sizeof(*snk1));

    init_ncurses();
    getmaxyx(stdscr, brd1->w , brd1->h);
    
    init_board(brd1);
    init_snake(snk1, brd1->w, brd1->h);

    
    int w = brd1->w;
    int h = brd1->h; 
    enum brd_symb **brdtst = brd1->brd; 
    for(int i=0; i < w ; i++) {
        for(int j=0; j < h; j++) {
            brdtst[i][j] = obstcl; 
       } 
    }

    for(int i=0; i < w ; i++) {
        for(int j=0; j < h; j++) {
            move(i, h);
            addch(brdtst[i][j]);
       } 
    }
    
    move(snk1->y, snk1->x); 
    addch(snk1->bdy);
    curs_set(0);

    while((key = getch()) != ' ')
        {}
  
    endwin();
    return 0;
}
