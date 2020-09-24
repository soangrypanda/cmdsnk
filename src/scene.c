#include <stdlib.h>
#include "scene.h"

static void set_main_m(struct scene *scene, int y, int x);

struct scene * set_scenes(int y, int x)
{
    struct scene *scenes = malloc(sizeof(*scenes) * SCENES_NUM);
    for(enum scene_t i=0; i < SCENES_NUM; i++) {
        switch(i) {
            case main_m:
                set_main_m(&scenes[main_m], y, x);
                break;
            case game:
                break;
            case pause_g:
                break;
            case SCENES_NUM:
                break;
        }
    }
    return scenes;
}
static void set_main_m(struct scene *scene, int y, int x)
{
    scene->type = main_m;
    scene->content = MAIN_M_CONTENT;
    scene->y = y / 2;
    scene->x = x / 2;
}
