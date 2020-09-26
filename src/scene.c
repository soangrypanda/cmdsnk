#include <stdlib.h>
#include "scene.h"

struct scene * set_scenes(int y, int x)
{
    struct scene *scenes = malloc(sizeof(*scenes) * SCENES_NUM);
    for(enum scene_t i=0; i < SCENES_NUM; i++) {
        switch(i) {
            case main_m:
                set_a_scene(i, &scenes[i], y, x, MAIN_M_CONTENT);
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
void set_a_scene(int scn, struct scene *scene, int y, int x, char *cont)
{
    scene->type = scn;
    scene->content = cont;
    scene->y = y / 2;
    scene->x = x / 2;
}

void change_scene_to(int scene, struct game_info *gi) {
    gi->stage = scene;
}
