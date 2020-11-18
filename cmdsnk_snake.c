#include "cmdsnk_snake.h"

#define SNAKE_SPEED_INCR_STRIDE 2
#define collide(ox, oy, scr, cell) (scr.win[scr.w * (int)oy+(int)ox]==cell) 

void handle_snake_collision(struct snake_s *snake, struct win_s *screen) { 
    if(collide(snake->nx, snake->ny, (*screen), brd_v) ||  
       collide(snake->nx, snake->ny, (*screen), brd_h) || 
       collide(snake->nx, snake->ny, (*screen), s_bdy)) { 

        snake->nx -= snake->vx * game_state.elapsed_time; 
        snake->ny -= snake->vy * game_state.elapsed_time;  

        snake->vx = 0; snake->vy = 0; 
        snake->be_moved = 0; 
        declare_game(lost); 
    } 
    else if(collide(snake->nx, snake->ny, (*screen), food)) { 
        game_state.score++; 
        change_food_cntr(&game_state, --); 
        enlen_snake(snake); 
        update_game_state(); 
        handle_snake_speed(snake); 
    } 
} 

void enlen_snake(struct snake_s *snake)
{   
    struct snake_part_s *tmp = calloc(1, sizeof(*tmp));
    //memcpy(tmp, snake->tail, sizeof(*tmp));
    tmp->next = NULL;
    tmp->x = snake->tail->x;
    tmp->y = snake->tail->y;
    tmp->bdy = s_bdy; 
    snake->tail->next = tmp;
    snake->tail = tmp;
}

void mv_snake(struct snake_part_s *snkprt, float x, float y, struct win_s *win) {
    draw_cell(*win, (int)snkprt->x, (int)snkprt->y, blank);
    if(snkprt->next)
        mv_snake(snkprt->next, snkprt->x, snkprt->y, win);
    snkprt->x = x;
    snkprt->y = y;
    draw_cell(*win, (int)snkprt->x, (int)snkprt->y, snkprt->bdy);
}
 
void handle_snake_speed(struct snake_s *snake)
{
    if((game_state.level + game_state.score) % 8 == 0) {
        snake->cvx += snake->dvx * SNAKE_SPEED_INCR_STRIDE;
        snake->cvy += snake->dvy * SNAKE_SPEED_INCR_STRIDE;
    }
}
 
void delete_snake(struct snake_s *snake)
{
    struct snake_part_s *tmp;
    while(snake->head) {
        tmp = snake->head;
        snake->head = snake->head->next;
        free(tmp);
    }
    snake->head = snake->tail = NULL;
}
