#include "cmdsnk_food.h"

void handle_food(struct win_s *scrn)
{
    int x = get_rand_n(1, scrn->w);
    int y = get_rand_n(1, scrn->h);
 
    if(scrn->win[scrn->w * y + x] != blank) {
        if(0 == find_better_cell(&x, &y, x, y, scrn, blank)) {
            declare_game(won);
            return;
        }
    }
    scrn->win[scrn->w * y + x] = food;
}
