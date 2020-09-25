#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "model.h"


/* GENERAL */
void init_ncurses(void)
{
    initscr();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    timeout(0);
    srand(time(NULL));
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
    gi->score = 0;
    gi->fud = init_food();
    return gi;
}

void turn_cell_into(char ch, int y, int x, struct game_info *gi)
{
    struct board *brd = gi->brd;
    brd->brd[x][y] = ch;
}

int cell_is(char ch, struct game_info *gi, int y, int x)
{
    if((gi->brd->brd[x][y] = ch))
        return 1;
    return 0;
}

int cell_is_within_brd(struct game_info *gi, int y, int x)
{
    if( (y > 0 && y < (gi->brd->h - 1)) &&
        (x > 0 && x < (gi->brd->w - 1)) )
            return 1;
    return 0;
}

int find_empty_cell(struct game_info *gi, int y, int x, 
                    int *y_to_return, int *x_to_return)
{
    if(!cell_is_within_brd(gi, y, x)) {
        return 0;
    }
    if(cell_is(empty, gi, y, x)) {
        *y_to_return = y;
        *x_to_return = x;
        return 1;
    }
    if (find_empty_cell(gi, y+1, x, y_to_return, x_to_return))
        return 1;
    if (find_empty_cell(gi, y, x+1, y_to_return, x_to_return))
        return 1;
    if (find_empty_cell(gi, y-1, x, y_to_return, x_to_return))
        return 1;
    if (find_empty_cell(gi, y, x-1, y_to_return, x_to_return))
        return 1;
    return 0;
}


/* SNAKE */
struct snk_prt * init_snake(int y, int x)
{
    struct snk_prt *snk = malloc(sizeof(*snk));
    snk->y = snk->prev_y = y / 2;
    snk->x = snk->prev_x = x / 2;
    snk->bdy = head;
    snk->next = NULL;
    snk->tail = snk;
    snk->move_delay = snk_max_move_delay;
    return snk;
}

void lengthen_snk(struct snk_prt *snk, int y, int x)
{
    struct snk_prt *tmp = malloc(sizeof(*tmp));
    tmp->y = y;
    tmp->x = x;
    tmp->bdy = body;
    tmp->next = NULL;
    tmp->tail = NULL;  
    snk->tail->next = tmp;
    snk->tail = tmp; 
}

void make_snk_faster(struct game_info *gi)
{
    struct snk_prt *snk = gi->snk;
    if(snk->move_delay > snk_min_move_delay)
        snk->move_delay -= delay_decrease; 
}

/* BOARD */
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


/* FOOD */
struct food *  init_food(void)
{
    struct food *fud = malloc(sizeof(*fud));
    fud->cntr = 0;
    fud->max = 1;
    fud->cell_srch_attempts = 0;
    return fud;
}

void food_handler(struct game_info *gi)
{
    struct food *fud = gi->fud;
    int score = gi->score;
    if((score != 0) && (score % food_mltplr == 0))
        fud->max++; 
    while((need_to_add_food(gi)))
        try_to_add_food_on_brd(gi);
}

int need_to_add_food(struct game_info *gi)
{
    struct food *fud = gi->fud;
    if(fud->cntr < fud->max)
        return 1;
    return 0;
}

void try_to_add_food_on_brd(struct game_info *gi)
{
    struct board *brd = gi->brd;
    int x = brd->w;
    int y = brd->h;
    int fud_x = 1 + (int)( ((double)x-1.0) * (rand()+1.0) / (RAND_MAX) );
    int fud_y = 1 + (int)( ((double)y-1.0) * (rand()+1.0) / (RAND_MAX) );
    if( cell_is_within_brd(gi, fud_y, fud_x) &&
        cell_is(empty, gi, fud_y, fud_x) ) 
    {
        add_food_on_brd(gi, fud_y, fud_x);
    }
    else
    {
        int new_y = fud_y;
        int new_x = fud_x;
        if((find_empty_cell(gi, fud_y, fud_x, &new_y, &new_x))) {
            add_food_on_brd(gi, new_y, new_x);
        }
        else {
         /* DECLARE WIN GAME */ 
        } 
    }
}

void add_food_on_brd(struct game_info *gi, int y, int x)
{
    turn_cell_into(food, y, x, gi);
    move(y,x);
    addch(food);
    curs_set(0); 
    gi->fud->cntr += 1;
}

