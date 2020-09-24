#ifndef CMDSNK_MODEL_H_SENTRY
#define CMDSNK_MODEL_H_SENTRY

#include "scene.h"

struct game_info {
    enum scene_t state;
    struct snk_prt *snk;
    struct board *brd;
    struct scene *scenes;  
    int score;
    struct food *fud;
};
enum bdy_symb { head='H' , body='@' };
struct snk_prt {
    enum bdy_symb bdy;
    int y, x, prev_y, prev_x;
    struct snk_prt *next, *tail;
};

enum brd_symb { empty = ' ', brdr='/', obstcl='*', food='^' };        
struct board {
    enum brd_symb **brd;
    int w, h;
};

enum consts { max_cell_search_attempts = 200, food_mltplr = 20 };
struct food {
    int cntr;
    int max;    
    int cell_srch_attempts;
};

void init_ncurses(void);
struct game_info * init_game_info(void);
void make_cell(int y, int x, char ch, struct game_info *gi);

struct snk_prt * init_snake(int y, int x);
void lengthen_snk(struct snk_prt *snk, int y, int x);

struct board * init_board(int y, int x);
void add_brdrs_2_brd(struct board *brd);
void set_brd(struct board *brd, enum brd_symb symb);

struct food * init_food(void);
void food_handler(struct game_info *gi);
int need_to_add_food(struct game_info *gi);
void try_to_add_food_on_brd(struct game_info *gi);
void add_food_on_brd(struct game_info *gi, int y, int x);
int cells_ok_and_empty(struct game_info *gi, int y, int x);
int find_empty_cell(struct game_info *gi, int old_y, int old_x, int *y, int *x);

#endif
