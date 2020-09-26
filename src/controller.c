#include <ncurses.h>
#include <unistd.h>
#include "controller.h"
#include "controller.h"

void move_snk(struct snk_prt *snk, int new_y, int new_x, struct game_info *gi)
{
    if(!snk) {
        curs_set(0);
        return;
    }
    move(snk->y, snk->x);
    turn_cell_into(empty, snk->y, snk->x, gi);
    addch(empty);
    snk->prev_y = snk->y;
    snk->prev_x = snk->x;
    snk->y = new_y;
    snk->x = new_x; 
    turn_cell_into(snk->bdy, snk->y, snk->x, gi);
    move(snk->y, snk->x);
    addch(snk->bdy);
    refresh();
    move_snk(snk->next, snk->prev_y, snk->prev_x, gi);
}

void handle_eat_food(struct game_info *gi, int y, int x)
{
    lengthen_snk(gi->snk, y, x); 
    turn_cell_into(empty, y, x, gi);
    gi->fud->cntr -= 1;
    gi->score += 1;
    make_snk_faster(gi);
}
void snk_move_handler(struct game_info *gi, int off_y, int off_x)
{
    struct board *brd = gi->brd;
    struct snk_prt *snk = gi->snk;
    int delay = snk->move_delay;
    int new_y = snk->y + off_y;
    int new_x = snk->x + off_x;
    switch(brd->brd[new_x][new_y]) {
        case(food):
            handle_eat_food(gi, new_y, new_x);
        case(empty):
            move_snk(snk, new_y, new_x, gi); 
            usleep(delay);
            break;
        default:
            break;
    }
}


int snk_direction_handled(struct game_info *gi, int dir)
{
    if( (gi->snk->cur_dir != nowhere) &&
        (gi->snk->cur_dir == (-dir)) )
    {
        return 0;
    }
    gi->snk->cur_dir = dir;
    return 1;
}

void game_handler(struct game_info *gi, char *key)
{
    food_handler(gi);
    switch(*key) {
        case 'l':
        case 'd':
            if(snk_direction_handled(gi, right))
                snk_move_handler(gi, MOVE_RIGHT);
            else
                *key = 'a';
            break;
        case 'h':
        case 'a':
            if(snk_direction_handled(gi, left))
                snk_move_handler(gi, MOVE_LEFT);
            else
                *key = 'd';
            break;
        case 'k':
        case 'w':
            if(snk_direction_handled(gi, up))
                snk_move_handler(gi, MOVE_UP);
            else
                *key = 's';
            break;
        case 'j':
        case 's':
            if(snk_direction_handled(gi, down))
                snk_move_handler(gi, MOVE_DOWN); 
            else
                *key = 'w';
            break;
        case 'q':
            break;
        default:
            break;
    }    
}

void main_m_handler(struct game_info *gi, char *key)
{
    switch(*key) {
        case ' ':
            change_scene_to(game, gi);
            render_scn(gi);
    }
}

void (*main_key_handler(struct game_info *gi, char *key))(struct game_info *gi, char *key)
{
    switch(gi->state) {
        case(game):
            return &game_handler;
            break;
        case(main_m):
        default:
            break;
    }
}

void dummy_handler(struct game_info *gi, char *key)
{}
