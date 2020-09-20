#ifndef CMDSNK_MODEL_H_SENTRY
#define CMDSNK_MODEL_H_SENTRY

#include "scene.h"

struct game_info {
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
struct board * init_board(int y, int x);
void add_brdrs_2_brd(struct board *brd);
void set_brd(struct board *brd, enum brd_symb symb);
struct snk_prt * init_snake(int y, int x);
struct game_info * init_game_info(void);
#endif
