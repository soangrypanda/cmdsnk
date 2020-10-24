#ifndef CMDSNK_TWODCOORD_H_SENTRY
#define CMDSNK_TWODCOORD_H_SENTRY

#include <object.h>

extern void *const twocoord;

int get_x(const void *const _self);
int get_y(const void *const _self);
int set_x(void *const _self, int x);
int set_y(void *const _self, int y);

/* TODO OF PUBLIC METHODS
    * distance between two coords
    * angle between three coords
*/    

#endif
