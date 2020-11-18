#ifndef CMDSNK_SNAKE_H_SENTRY
#define CMDSNK_SNAKE_H_SENTRY

#include <string.h>

#include "cmdsnk_snake.h"
#include "cmdsnk_gamestate.h"
#include "cmdsnk_food.h"
#include "cell_mod.h"
#include "win_mod.h"

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

void handle_snake_collision(struct snake_s *snake, struct win_s *screen);
void enlen_snake(struct snake_s *snake);
void handle_snake_speed(struct snake_s *snake);
void mv_snake(struct snake_part_s *snkprt, float x, float y, struct win_s *win);
void delete_snake(struct snake_s *snake);


#endif
