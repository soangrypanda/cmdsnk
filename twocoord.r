#ifndef CMDSNK_TWODCOORD_R_SENTRY
#define CMDSNK_TWODCOORD_R_SENTRY

#include <object.r>

#define TWOCOORD_VT          OBJECT_VT

#define TWOCOORD_VT_INIT     OBJECT_VT_INIT,                              \
                             .size = sizeof(struct twocoord_class_s),     \
                             .ctor = twocoord_ctor

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
