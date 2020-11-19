#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "elpsd_t.h"
#include "txt_mod.h"
#include "texts.h"
#include "win_mod.h"
#include "rand_mod.h"
#include "cell_mod.h"
#include "mainloop_mod.h"

#include "cmdsnk_food.h"
#include "cmdsnk_gamestate.h"
#include "cmdsnk_snake.h"


void initcurses(void);
void handle_key(int inp, struct snake_s * snake);


int main(void)
{
    init_rand();
    initcurses();

    init_mainscreen(screen);
    
    init_txt(score, sizeof(SCORE_TXT) + 2);
    update_txt(score.txt, SCORE_TXT, game_state.score); 
    init_txt(level, sizeof(LEVEL_TXT) + 2);
    update_txt(level.txt, LEVEL_TXT, game_state.level); 
    init_txt(title, TITLE_TXT_W);
    update_txt(title.txt, TITLE_TXT); 

    init_txt(g_won, G_WON_TXT_W + 2);
    update_txt(g_won.txt, G_WON_TXT); 
    init_txt(g_los, G_LOS_TXT_W);
    update_txt(g_los.txt, G_LOS_TXT); 
    init_txt(retry, RETRY_TXT_W);
    update_txt(retry.txt, RETRY_TXT); 

/* if I would rewrite this ugly peace below later then don't forget
 * to increase abstartion level and make functions not ad hoc and
 * actually usable in other places too */
    int win_h = 6;
    if(score.w+level.w+title.w< screen.w-2) 
        win_h = 4; 
    init_win(win, 0,0,screen.w,win_h, &screen);

    if(score.w+level.w+title.w< screen.w-2) {
        mv_txt(&score, 2, win.y + 2);
        mv_txt(&level, win.w - level.w + 1, win.y + 2);
        mv_txt(&title, win.w / 2 - title.w / 2, win.y);
    }
    else {
        mv_txt(&score, 2, win.y + 2);
        mv_txt(&level, 2, win.y + 4);
        mv_txt(&title, win.w / 2 - title.w / 2, win.y);
    }
    mv_txt(&g_won, screen.w / 2 - g_won.w / 2, screen.h / 2);
    mv_txt(&g_los, screen.w / 2 - g_los.w / 2, screen.h / 2);
    mv_txt(&retry, screen.w / 2 - retry.w / 2, screen.h / 2 + 1);

    init_win(game_win, 0, win.h, screen.w, screen.h-game_win.y, &screen);

MAINLOOP_RESTART_POSITION

    fill_win_with(blank, &screen);
    draw_win_brdr(win, brd_v, brd_h);
    draw_win_brdr(game_win, brd_v, brd_h);

    reset_gamestate();
    
    init_snkprt(snkprt, game_win);
    init_snake(snake, snkprt);
    
    draw_cell(game_win, (int)snkprt->x, (int)snkprt->y, snkprt->bdy);

    set_timer();
 
    while(game_state.state == on) {
        game_state.elapsed_time = get_elapsed_time();        
 
        handle_key(getch(), snake);

        if(need_to_add_food(&game_state)) {
            handle_food(&game_win);
            change_food_cntr(&game_state, ++);
        }

        update_snake_new_coords(snake, game_state.elapsed_time);
        handle_snake_collision(snake, &game_win);
        handle_snake_movement(snake, &game_win);
            
        update_txt(score.txt, SCORE_TXT, game_state.score); 
        update_txt(level.txt, LEVEL_TXT, game_state.level); 

        mvaddstr(0, 0, screen.win);
        mvaddstr(score.y,score.x,score.txt);
        mvaddstr(level.y,level.x,level.txt);
        mvaddstr(title.y,title.x,title.txt);
        refresh();
    }
    
    switch(game_state.state) {
        case(won):
            mvaddstr(g_won.y, g_won.x, g_won.txt);        
            mvaddstr(retry.y, retry.x, retry.txt);        
            break;
        case(lost):
            kill_snake(snake, &game_win);
            mvaddstr(g_los.y, g_los.x, g_los.txt);        
            mvaddstr(retry.y, retry.x, retry.txt);        
            break;
        case(ext):
        case(on):
        default:
            break;
    }
    delete_snake(snake);

    if(game_state.state != ext)
        ASK_FOR_RESTART;

    free(screen.win);

    free(score.txt);
    free(level.txt);
    free(title.txt);
    free(g_won.txt);
    free(g_los.txt);
    free(retry.txt);

    endwin();
    return EXIT_SUCCESS;
}

void initcurses(void)
{
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, 1); 
    timeout(0);
}

void handle_key(int inp, struct snake_s *snake)
{
    switch(inp){
        case(' '):
            game_state.state = ext; 
            break;
        case('l'):
            if(snake->dir == left)
                break;
            snake->vx = snake->cvx; 
            snake->vy = 0; 
            snake->dir = right;
            snake->be_moved = 1;
            break;
        case('h'):
            if(snake->dir == right)
                break;
            snake->vx = -snake->cvx;
            snake->vy = 0;
            snake->dir = left;
            snake->be_moved = 1;
            break;
        case('k'):
            if(snake->dir == down)
                break;
            snake->vx = 0;
            snake->vy = -snake->cvy;
            snake->dir = up;
            snake->be_moved = 1;
            break;
        case('j'):
            if(snake->dir == up)
                break;
            snake->vx = 0;
            snake->vy = snake->cvy;
            snake->dir = down;
            snake->be_moved = 1;
            break;
        default:
            break;
    }
}
