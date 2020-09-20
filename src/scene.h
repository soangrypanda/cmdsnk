#ifndef CMDSNK_SCENE_H_SENTRY
#define CMDSNK_SCENE_H_SENTRY

#define MAIN_M_CONTENT "press enter to play\n"\
                       "vim keys or arrows to move\n"\
                       "esc while in game to pause"

enum scene_t { main_m=0, game, pause, SCENES_NUM };
struct scene {
    enum scene_t type;
    char *content; 
        int y, x;
};

struct scene * set_scenes(int y, int x);
    
#endif
