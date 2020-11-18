#include "win_mod.h"

void init_mainscreen(struct win_s *screen)
{
    screen->x = screen->y = 0;
    getmaxyx(stdscr, screen->h, screen->w);
    screen->win= calloc((screen->h)*(screen->w)+1, sizeof(*(screen->win)));
}
