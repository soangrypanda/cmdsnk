#include "twocoord.h"
#include "twocoord.r"

void *twocoord_ctor(void *const _self, va_list *app)
{
    struct twocoord_class_s *const self = _self;
    self->x = va_arg(*app, int);
    self->y = va_arg(*app, int);
    return _self; 
}

void twocoord_print(const void *const _self)
{
    printf("%d, %d\n", get_x(_self), get_y(_self));
}

int twocoord_compare(const void *const _self, const void *const _other)
{
    return (get_x(_self) == get_x(_other)) && (get_y(_self) == get_y(_other));
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

int set_x(void *const _self, int x)
{
    struct twocoord_class_s *const self = _self;
        if(CHECK_IF_OBJ(self) || !self->x)
    exit_error("get_x", EWRONGOBJ);
    return self->x = x;
}

int set_y(void *const _self, int y)
{
    struct twocoord_class_s *const self = _self;
        if(CHECK_IF_OBJ(self) || !self->x)
    exit_error("get_x", EWRONGOBJ);
    return self->y = y;
}


struct twocoord_vt_s twocoord_vt = {
    TWOCOORD_VT_INIT,
};

void *const twocoord = &twocoord_vt;
