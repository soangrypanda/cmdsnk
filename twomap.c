#include "twomap.h"
#include "twomap.r"

void *twomap_ctor(void *const _self, va_list *app)
{
    struct twomap_class_s *const self = _self;
    if(self->vt != twomap)
        return 0;
    self->map = va_arg(*app, char*);
    self->objs_on_map = NULL;
    self->num_of_objs = 0;
    self->max_objs = va_arg(*app, int);
    self->w = va_arg(*app, int);
    self->h = va_arg(*app, int);
    self->max_w = va_arg(*app, int);
    self->max_h = va_arg(*app, int);
    self->no = va_arg(*app, int);
      
        
    return _self;
}

void twomap_print(const void *const _self)
{
    printf("%s\n", ((const struct twomap_class_s *const)_self)->map);
}

struct twomap_vt_s twomap_vt = {
    TWOMAP_VT_INIT
};

void *const twomap = &twomap_vt;
