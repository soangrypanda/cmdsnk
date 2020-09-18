#ifndef CMDSNK_H_CNTRLR_SENTRY
#define CMDSNK_H_CNTRLR_SENTRY

#include "model.h"

#define MOVE_LEFT 0, -1
#define MOVE_RIGHT 0, 1
#define MOVE_UP -1, 0
#define MOVE_DOWN 1, 0 

void move_snk(struct snk_prt *snk, int off_y, int off_x);
void snk_move_handler(struct board *brd, struct snk_prt *snk, int off_y, int off_x);
void game_handler(struct board *brd, struct snk_prt *snk, char key);


#endif
