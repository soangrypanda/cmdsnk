#include "view.h"
#include "model.h"

void render_scn(struct game_info *gi)
{
    struct scene *scn_to_rndr = &(gi->scenes[scn]);
    if(gi->stage == game)
        render_game(gi);
    else
        render_non_game_scene(gi); 
