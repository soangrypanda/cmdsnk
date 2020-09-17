#ifndef CMDSNK_MODEL_H_SENTRY
#define CMDSNK_MODEL_H_SENTRY

#include "scene.h"

enum bdy_symb { head='H' , body='@' };
struct snk_prt {
    enum bdy_symb bdy;
    int y, x;
    struct snk_prt *next;
};

enum brd_symb { empty = ' ', brdr='/', obstcl='*', food='^' };        
struct board {
    enum brd_symb **brd;
    int w, h, cells_n;
};

void init_ncurses(void);
void init_board(struct board *brd);
void add_brdrs_2_brd(struct board *brd);
void set_brd(struct board *brd, enum brd_symb symb);
void init_snake(struct snk_prt *snk, int y, int x);
#endif
