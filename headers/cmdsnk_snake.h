#ifndef CMDSNK_SNAKE_H_SENTRY
#define CMDSNK_SNAKE_H_SENTRY


#include <string.h>

#include "cmdsnk_snake.h"
#include "cmdsnk_gamestate.h"
#include "cmdsnk_food.h"
#include "cell_mod.h"
#include "win_mod.h"
#include "err_mod.h"

struct snake_part_s {
    struct snake_part_s *next;
    float x, y;
    char bdy;
};

struct snake_s {
    struct snake_part_s *head;
    struct snake_part_s *tail;
    float nx, ny;
    float vx, vy;
    float cvx, cvy;
    float dvx, dvy;
    char dir;
    char be_moved;
};

enum snake_dir {up, left, down, right, none};

#define init_snkprt(snkprt, win)                                \
    struct snake_part_s *snkprt = calloc(1, sizeof(*snkprt));   \
    check_alloc(snkprt, errno, "init_snkprt");                  \
    snkprt->next = NULL;                                        \
    snkprt->x = win.w / 2;                                      \
    snkprt->y = win.h / 2;                                      \
    snkprt->bdy = s_head

#define init_snake(snake, snkprt)                               \
    struct snake_s *snake = calloc(1, sizeof(*snake));          \
    check_alloc(snake, errno, "init_snake");                    \
    snake->head = snkprt;                                       \
    snake->tail = snkprt;                                       \
    snake->vx = 0;                                              \
    snake->vy = 0;                                              \
    snake->cvx = 12;                                            \
    snake->cvy = 8;                                             \
    snake->dvx = 1;                                             \
    snake->dvy = 0.6;                                           \
    snake->nx = snkprt->x;                                      \
    snake->ny = snkprt->y;                                      \
    snake->be_moved = 0;                                        \
    snake->dir = none


void handle_snake_collision(struct snake_s *snake, struct win_s *screen);
void handle_snake_movement(struct snake_s *snake, struct win_s *win);
void enlen_snake(struct snake_s *snake);
void handle_snake_speed(struct snake_s *snake);
void mv_snake(struct snake_part_s *snkprt, float x, float y, struct win_s *win);
void delete_snake(struct snake_s *snake);
void update_snake_new_coords(struct snake_s *snake, float et);
void kill_snake(struct snake_s *snake, struct win_s *win);


#endif
