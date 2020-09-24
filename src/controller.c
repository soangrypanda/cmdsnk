#include <ncurses.h>
#include <unistd.h>
#include "controller.h"
#include "controller.h"

void move_snk(struct snk_prt *snk, int new_y, int new_x)
{
    if(!snk) {
        curs_set(0);
        return;
    }
    move(snk->y, snk->x);
    addch(empty);
    snk->prev_y = snk->y;
    snk->prev_x = snk->x;
    snk->y = new_y;
    snk->x = new_x; 
    move(snk->y, snk->x);
    addch(snk->bdy);
    refresh();
    move_snk(snk->next, snk->prev_y, snk->prev_x); 
}

void handle_eat_food(struct game_info *gi, int y, int x)
{
    lengthen_snk(gi->snk, y, x); 
    make_cell(y, x, empty, gi);
    gi->fud->cntr -= 1;
    gi->score += 1;
}
void snk_move_handler(int delay, struct game_info *gi, int off_y, int off_x)
{
    struct board *brd = gi->brd;
    struct snk_prt *snk = gi->snk;
    int new_y = snk->y + off_y;
    int new_x = snk->x + off_x;
    switch(brd->brd[new_x][new_y]) {
        case(brdr):
        case(obstcl):
            return;
        case(food):
            handle_eat_food(gi, new_y, new_x);
        default:
            move_snk(snk, new_y, new_x); 
            usleep(delay);
    }
}

void game_handler(struct game_info *gi, char key)
{
    static int move_delay = 150000;
    switch(key) {
        case 'l':
        case 'd':
            snk_move_handler(move_delay, gi, MOVE_RIGHT);
            break;
        case 'h':
        case 'a':
            snk_move_handler(move_delay, gi, MOVE_LEFT);
            break;
        case 'k':
        case 'w':
            snk_move_handler(move_delay, gi, MOVE_UP);
            break;
        case 'j':
        case 's':
            snk_move_handler(move_delay, gi, MOVE_DOWN); 
            break;
        case 'q':
            break;
        default:
            break;
    }    
}

void (*main_key_handler(struct game_info *gi, char key))(struct game_info *gi, char key)
{
    switch(gi->state) {
        case(game):
            return &game_handler;
    }
}

void dummy_handler(struct game_info *gi, char key)
{}
