#ifndef CMDSNK_GAMESTATE_H_SENTRY
#define CMDSNK_GAMESTATE_H_SENTRY


struct game_state_s {
    float elapsed_time;
    int score;
    int level;
    int foods, max_food, max_food_cap;
    enum state_t { won, lost, on, ext } state;
};

extern struct game_state_s game_state;

enum cells { blank = '.', s_head = '@', s_bdy = '0', s_dead = 'X',
             food = '^', brd_h = '-', brd_v = '|' };

void update_game_state();
void declare_game(int what);
void reset_gamestate(void);


#endif
