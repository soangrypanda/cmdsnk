#ifndef CMDSNK_FOOD_H_SENTRY
#define CMDSNK_FOOD_H_SENTRY

#include "cmdsnk_gamestate.h"
#include "rand_mod.h"
#include "cell_mod.h"

#define need_to_add_food(gs) ((gs)->foods < (gs)->max_food)
#define change_food_cntr(gs, how) (((gs)->foods)how)

void handle_food(struct win_s *scrn)
{
/*
    int x = 1 + rand_r(&seed) / (RAND_MAX  / (scrn->w - 2)) ;
    int y = 1 + rand_r(&seed) / (RAND_MAX  / (scrn->h - 2)) ;
*/
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


#endif
