#ifndef MAINLOOP_MOD_H_SENTRY
#define MAINLOOP_MOD_H_SENTRY


#include <curses.h>

#define MAINLOOP_RESTART_POSITION restart_pos:
#define RESTART_MAINLOOP goto restart_pos
#define ASK_FOR_RESTART do {                \
    timeout(-1);                            \
    int k;                                  \
    while( (k = getch()) ) {                \
        if(k == 'y' || k == 'Y') {          \
            timeout(0);                     \
            RESTART_MAINLOOP;               \
        }                                   \
        else if(k == 'n' || k == 'N') {     \
            break;                          \
        }                                   \
    }                                       \
  } while(0)


#endif
