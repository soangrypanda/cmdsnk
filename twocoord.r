#ifndef CMDSNK_TWODCOORD_R_SENTRY
#define CMDSNK_TWODCOORD_R_SENTRY

#include <object.r>

void *twocoord_ctor(void *const _self, va_list *app);
void twocoord_print(const void *const _self);
int twocoord_compare(const void *const _self, const void *const _other);

#define TWOCOORD_VT          OBJECT_VT

#define TWOCOORD_VT_INIT     OBJECT_VT_INIT,                              \
                             .size = sizeof(struct twocoord_class_s),     \
                             .ctor = twocoord_ctor,                       \
                             .print= twocoord_print,                      \
                             .compare = twocoord_compare

#define TWOCOORD_DATA        OBJECT_DATA;          \
                             int x, y


struct twocoord_vt_s {
    TWOCOORD_VT;
};

struct twocoord_class_s {
    struct twocoord_vt_s *const vt;
    TWOCOORD_DATA;
};




#endif
