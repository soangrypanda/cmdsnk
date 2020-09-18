#ifndef CMDSNK_MODEL_H_SENTRY
#define CMDSNK_MODEL_H_SENTRY

#include "scene.h"

struct game {
    enum scene_t state;
    struct snk_prt *snk;
    struct board *brd;
    struct scene *scenes;  
};
enum bdy_symb { head='H' , body='@' };
struct snk_prt {
    enum bdy_symb bdy;
    int y, x, prev_y, prev_x;
    struct snk_prt *next;
};

enum brd_symb { empty = ' ', brdr='/', obstcl='*', food='^' };        
struct board {
    enum brd_symb **brd;
    int w, h;
};

void init_ncurses(void);
void init_board(struct board *brd, int y, int x);
void add_brdrs_2_brd(struct board *brd);
void set_brd(struct board *brd, enum brd_symb symb);
void init_snake(struct snk_prt *snk, int y, int x);
#endif
