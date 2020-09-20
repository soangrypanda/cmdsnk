#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "controller.h"

int main(void)
{
    init_ncurses();
    struct game_info *gi = init_game_info();
    struct snk_prt *snk = gi->snk;
    struct board *brd = gi->brd;
    struct scene *scenes = gi->scenes;
    
    for(int i=0; i < brd->w; i++) {
        for(int j=0; j < brd->h; j++) {
            move(j, i);
            addch(brd->brd[i][j]);
        }
    }
    move(snk->y, snk->x);
    addch(snk->bdy);
/*
    move(scenes[main_m].y, scenes[main_m].x);
    addstr(scenes[main_m].content);
*/
    FILE *f = fopen("celln.txt", "w");
    fprintf(f, "W is %d and H is %d\n", brd->w, brd->h); 
    curs_set(0);

    int key;
    while((key = getch()) != ' ') {
        main_key_handler(gi, key);
    }
    curs_set(0);  
    endwin();
    return 0;
}
