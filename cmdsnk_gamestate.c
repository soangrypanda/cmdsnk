#include "cmdsnk_gamestate.h"

struct game_state_s game_state = { 0 };

void update_game_state()
{
    if(game_state.score % 10 == 0) {
        game_state.level++;
        if(game_state.max_food < game_state.max_food_cap) {
            game_state.max_food++;
        }
    }
}

void declare_game(int what)
{   
    game_state.state = what;
}
