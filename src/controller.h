#ifndef CMDSNK_H_CNTRLR_SENTRY
#define CMDSNK_H_CNTRLR_SENTRY

#include "model.h"

#define MOVE_LEFT 0, -1
#define MOVE_RIGHT 0, 1
#define MOVE_UP -1, 0
#define MOVE_DOWN 1, 0 

void move_snk(struct snk_prt *snk, int off_y, int off_x);
void snk_move_handler(struct game_info *gi, int off_y, int off_x);
void game_handler(struct game_info *gi, char key);
void (*main_key_handler(struct game_info *gi, char key))(struct game_info *gi, char key);
void dummy_handler(struct game_info *gi, char key);
void handle_eat_food(struct game_info *gi, int y, int x);

#endif
