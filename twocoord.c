#include "twocoord.h"
#include "twocoord.r"

void *twocoord_ctor(void *const _self, va_list *app)
{
    struct twocoord_class_s *const self = _self;
    self->x = va_arg(*app, int);
    self->y = va_arg(*app, int);
    return _self; 
}

int get_x(const void *const _self)
{
    const struct twocoord_class_s *const self = _self;
    if(CHECK_IF_OBJ(self) || !self->x)
        exit_error("get_x", EWRONGOBJ);
    return self->x; 
}

int get_y(const void *const _self)
{
    const struct twocoord_class_s *const self = _self;
    if(CHECK_IF_OBJ(self) || !self->y)
        exit_error("get_x", EWRONGOBJ);
    return self->y; 
}

struct twocoord_vt_s twocoord_vt = {
    TWOCOORD_VT_INIT,
    .size = sizeof(struct twocoord_class_s),
    .ctor = twocoord_ctor
};

void *const twocoord = &twocoord_vt;
