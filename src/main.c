#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include "controller.h"

int main(void)
{
    int key, y, x;
    struct board *brd1 = malloc(sizeof(*brd1));
    struct snk_prt *snk1 = malloc(sizeof(*snk1));

    init_ncurses();
    getmaxyx(stdscr, y, x);
    brd1->h = y;
    brd1->w = x; 
    init_board(brd1, y, x);
    init_snake(snk1, y, x);

     
    int w = brd1->w;
    int h = brd1->h; 
 
    set_brd(brd1, empty);
    add_brdrs_2_brd(brd1);
    enum brd_symb **brdtst = brd1->brd; 

    for(int i=0; i < w ; i++) {
        for(int j=0; j < h; j++) {
            move(j, i);
            addch(brdtst[i][j]);
       } 
    }
/*    
    struct scene scenes[SCENES_NUM];
    set_scenes(scenes, y, x);
    move(scenes[main_m].y, scenes[main_m].x);
    addstr(scenes[main_m].content);
*/
    move(snk1->y, snk1->x); 
    addch(snk1->bdy);
    curs_set(0);

    
    FILE *f = fopen("celln.txt", "w");
    fprintf(f, "W is %d and H is %d\n", brd1->w, brd1->h); 
    curs_set(0);

    while((key = getch()) != ' ')
        {}
  
    endwin();
    return 0;
}
