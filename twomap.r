#ifndef CMDSNK_TWOMAP_R_SENTRY
#define CMDSNK_TWOMAP_R_SENTRY

#include <object.r>

#define TWOMAP_VT           OBJECT_VT

#define TWOMAP_VT_INIT      OBJECT_VT_INIT,                     \
                            .ctor=twomap_ctor,                  \
                            .print=twomap_print,                \
                            .size=sizeof(struct twomap_class_s)

#define TWOMAP_DATA         OBJECT_DATA;            \
                            char *map;              \
                            void **objs_on_map;     \
                            int num_of_objs;        \
                            int max_objs;           \
                            int w, h, max_w, max_h; \
                            int no

struct twomap_vt_s {
    TWOMAP_VT;
}; 

struct twomap_class_s {
    struct twomap_vt_s *vt;
    TWOMAP_DATA;
};

/* 2D MAP SHOULD CONSTIST OF:
    * pointer to a map (in a form of string pointer?)
    * max width and hight
    * collection of objects on the map
    * number of objects on the map
    * max number of objects allowed on the map?
*/

#endif
