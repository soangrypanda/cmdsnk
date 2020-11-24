#ifndef CMD_SNK_CELL_MOD_H_CENTRY
#define CMD_SNK_CELL_MOD_H_CENTRY


#include <string.h>
#include "win_mod.h"
#include "err_mod.h"

#define draw_cell(wind, x, y, cont) ( (wind).win[(wind).w * (y) + (x)] = cont )
#define in_scope(x,y,scrn) ( ((x)>0)&&((x)<(scrn)->w)&&((y)>0)&&((y)<(scrn)->h) )

int find_better_cell(int *px, int *py, int x, int y, struct win_s *scrn, int cell_needed);
int already_seen(int x, int y, int *xa, int *ya, int len);
void cell_mod_cleanup(void);


#endif
