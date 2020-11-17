#ifndef CMDSNK_TXT_MOD_H_SENTRY
#define CMDSNK_TXT_MOD_H_SENTRY

#include <stdio.h>
#include <stdlib.h>
/* some includes are in case macros are rewriten as functions */


struct txt_s {
    char *txt;
    int x, y, w;
};

#define update_txt(...) sprintf( __VA_ARGS__ );

#define init_txt(obj, width)      \
        struct txt_s obj = { 0 }; \
        obj.w = width;            \
        obj.txt = malloc(obj.w)   

#define mv_txt(txt, tx, ty);      \
        (txt)->x = tx;            \
        (txt)->y = ty   

#endif
