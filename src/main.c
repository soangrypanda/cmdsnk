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
    
    void (*key_handler)(struct game_info *, char);
    char tmp_key;
    key_handler = &dummy_handler;
    while((key = getch()) != ' ') {
        if(key != ERR) {
            key_handler = main_key_handler(gi, key);
            tmp_key = key;
        } 
        food_handler(gi);
        key_handler(gi, tmp_key);
    }
    
    curs_set(0);  
    endwin();
    return 0;
}
