#ifndef CMDSNK_FOOD_H_SENTRY
#define CMDSNK_FOOD_H_SENTRY


#include "cmdsnk_gamestate.h"
#include "rand_mod.h"
#include "cell_mod.h"
#include "win_mod.h"

#define need_to_add_food(gs) ((gs)->foods < (gs)->max_food)
#define change_food_cntr(gs, how) (((gs)->foods)how)

void handle_food(struct win_s *scrn);


#endif
