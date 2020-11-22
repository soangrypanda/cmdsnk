#include "cmdsnk_food.h"

void handle_food(struct win_s *scrn)
{
    int x = get_rand_n(1, scrn->w);
    int y = get_rand_n(1, scrn->h);
 
    if(scrn->win[scrn->w * y + x] != blank) {
        int cell = find_better_cell(&x, &y, x, y, scrn, blank); 
        if(cell == 0) {
            game_state.max_food--;
            game_state.max_food_cap--;
            game_state.foods = game_state.max_food;
            if(game_state.max_food == 0)
                declare_game(won);
            return;
        }
        else if(cell == -1) {
            scrn->win[scrn->w * y + x] = food;
            change_food_cntr(&game_state, ++);
            return;
        }
        else {
            return; 
        }
    }
    else {
        scrn->win[scrn->w * y + x] = food;
        change_food_cntr(&game_state, ++);
    }
}
