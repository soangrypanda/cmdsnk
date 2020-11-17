#ifndef CMDSNK_WIN_MOD_H_SENTRY
#define CMDSNK_WIN_MOD_H_SENTRY

#include <string.h>
/* some includes are in case macros are rewriten as functions */

/* --- NEED TO THINK ABOUT POSSIBILITY OF DRAWING WINS SEPARATELLY --- */

struct win_s {
    char *win;
    int w, h, x, y;
};

#define init_win(wind, wx, wy, ww, wh, scrn)            \
    struct win_s wind = { 0 };                          \
    wind.x = wx;                                        \
    wind.y = wy;                                        \
    wind.w = ww;                                        \
    wind.h = wh;                                        \
    wind.win = &((scrn)->win[wy*((scrn)->w)+wx])

#define draw_win_brdr(wind, bv, bh)                     \
    for(int h = 0; h < wind.h; ++h) {                   \
        wind.win[h * wind.w] = bv;                      \
        wind.win[h * wind.w + wind.w - 1] = bv;         \
    }                                                   \
    for(int w = 0; w < wind.w; ++w) {                   \
        wind.win[w] = bh;                               \
        wind.win[wind.w * wind.h - wind.w + w] = bh;    \
    }                                                   \
    wind.win[wind.w*wind.h] = '\0'
 
#define fill_win_with(what, where)                      \
    memset((where)->win, what, (where)->h * (where)->w);\
    (where)->win[(where)->h*(where)->w] = '\0'


#endif
