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

#include "cmdsnk_food.h"
#include "cmdsnk_gamestate.h"

FILE *fd1; 

#define collide(ox, oy, scr, cell) (scr.win[scr.w * (int)oy+(int)ox]==cell)

#define SNAKE_SPEED_INCR_STRIDE 2 


#define MAINLOOP_RESTART_POSITION restart_pos:
#define RESTART_MAINLOOP goto restart_pos
#define ASK_FOR_RESTART do {                \
    timeout(-1);                            \
    int k;                                  \
    while( (k = getch()) ) {                \
        if(k == 'y' || k == 'Y') {          \
            timeout(0);                     \
            RESTART_MAINLOOP;               \
        }                                   \
        else if(k == 'n' || k == 'N') {     \
            break;                          \
        }                                   \
    }                                       \
    } while(0)


int scx, scy, lvx, lvy, tlx, tly;
unsigned int ui_score_win_h = 1;


struct snake_part_s {
    struct snake_part_s *next;
    float x, y;
    char bdy;
};
struct snake_s {
    struct snake_part_s *head;
    struct snake_part_s *tail;
    float nx, ny;
    float vx, vy;
    float cvx, cvy;
    float dvx, dvy;
    char dir;
    char be_moved; 
};

enum snake_dir {up, left, down, right, none};


void initcurses(void);

void init_mainscreen(struct win_s *screen);

void handle_key(int inp, struct snake_s * snake);

void handle_snake_collision(struct snake_s *snake, struct win_s *screen);
void enlen_snake(struct snake_s *snake);
void handle_snake_speed(struct snake_s *snake);
void mv_snake(struct snake_part_s *snkprt, float x, float y, struct win_s *win);
void delete_snake(struct snake_s *snake);


int main(void)
{
    init_rand();
    initcurses();

    struct win_s screen = { 0 };
    init_mainscreen(&screen);
    
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


    //struct snake_part_s snkprt = { 0 };
    struct snake_s snake = { 0 };

MAINLOOP_RESTART_POSITION

    fill_win_with(blank, &screen);
    draw_win_brdr(win, brd_v, brd_h);
    draw_win_brdr(game_win, brd_v, brd_h);

    game_state.state = on;
    game_state.foods = 0;
    game_state.max_food = 1; 
    game_state.max_food_cap = 5; 
    game_state.score= 0;
    game_state.level= 0;
    
    struct snake_part_s *snkprt = calloc(1, sizeof(struct snake_part_s));
    snkprt->next = NULL;
    snkprt->x = game_win.w / 2;
    snkprt->y = game_win.h / 2;
    snkprt->bdy = s_head;

    snake.head = snkprt;
    snake.tail = snkprt;
    snake.vx = 0;
    snake.vy = 0;
    snake.cvx = 12;
    snake.cvy = 8;
    snake.dvx = 1;
    snake.dvy = 0.6;
    snake.nx = snkprt->x; 
    snake.ny = snkprt->y;
    snake.be_moved = 0;
    snake.dir = none; 
    
    draw_cell(game_win, (int)snkprt->x, (int)snkprt->y, snkprt->bdy);
     
    FILE *fd = fopen("log.txt", "w");
    fd1 = fopen("log.txt", "w");
    //fprintf(fd, "x - %d, y - %d, scr x - %d, scr y - %d\n", win.w , win.h, screen.w, screen.h);
    //fprintf(fd, "x - %d, y - %d, sx - %f, ys - %f, sizeof - %ld\n", screen.w, screen.h, snake.x, snake.y, sizeof((*screen.screen)));
    //fprintf(fd, "x - %d, y - %d, sx - %d, ys - %d\n", x, y, snake.x, snake.y);


    set_timer();
 
    while(game_state.state == on) {
        game_state.elapsed_time = get_elapsed_time();        
        //fprintf(fd, "et = %f\n", elapsed_time);
 
        handle_key(getch(), &snake);

        if(need_to_add_food(&game_state)) {
            handle_food(&game_win);
            change_food_cntr(&game_state, ++);
        }

        //fprintf(fd, "%d - %d\n", game_state.foods, game_state.max_food);

        //draw_cell(game_win, (int)snkprt.x, (int)snkprt.y, blank);

        snake.nx += snake.vx * game_state.elapsed_time;
        snake.ny += snake.vy * game_state.elapsed_time; 
        handle_snake_collision(&snake, &game_win);
        if(((int)snake.nx != (int)snake.head->x) || ((int)snake.ny != (int)snake.head->y))
            snake.be_moved = 1;
        if(snake.be_moved)
            mv_snake(snake.head, snake.nx, snake.ny, &game_win);
        snake.be_moved = 0;
            
        //draw_cell(game_win, (int)snkprt.x, (int)snkprt.y, snkprt.bdy);

        //update_game_state(&game_state);

        update_txt(score.txt, SCORE_TXT, game_state.score); 
        update_txt(level.txt, LEVEL_TXT, game_state.level); 

        //mvaddstr(win.y, win.x, win.win);
        //mvaddstr(game_win.y, game_win.x, game_win.win);
        mvaddstr(0, 0, screen.win);
        mvaddstr(score.y,score.x,score.txt);
        mvaddstr(level.y,level.x,level.txt);
        mvaddstr(title.y,title.x,title.txt);
        refresh();
    }
    
    delete_snake(&snake);
    switch(game_state.state) {
        case(ext):
            break;
        case(won):
            mvaddstr(g_won.y, g_won.x, g_won.txt);        
            mvaddstr(retry.y, retry.x, retry.txt);        
            break;
        case(lost):
            mvaddstr(g_los.y, g_los.x, g_los.txt);        
            mvaddstr(retry.y, retry.x, retry.txt);        
            break;
        case(on):
        default:
            break;
    }

    if(game_state.state != ext)
        ASK_FOR_RESTART;

    free(screen.win);
    //free(win.win);
    //free(game_win.win);

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
void init_mainscreen(struct win_s *screen)
{
    screen->x = screen->y = 0;
    getmaxyx(stdscr, screen->h, screen->w);
    screen->win= calloc((screen->h)*(screen->w)+1, sizeof(*(screen->win)));
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





void handle_snake_collision(struct snake_s *snake, struct win_s *screen) {
    if(collide(snake->nx, snake->ny, (*screen), brd_v) || 
       collide(snake->nx, snake->ny, (*screen), brd_h) ||
       collide(snake->nx, snake->ny, (*screen), s_bdy)) {

        snake->nx -= snake->vx * game_state.elapsed_time;
        snake->ny -= snake->vy * game_state.elapsed_time; 

        snake->vx = 0; snake->vy = 0;
        snake->be_moved = 0;
        declare_game(lost);
    }
    else if(collide(snake->nx, snake->ny, (*screen), food)) {
        game_state.score++;
        change_food_cntr(&game_state, --);
        enlen_snake(snake);
        update_game_state();
        handle_snake_speed(snake);
    }
}

void enlen_snake(struct snake_s *snake) 
{
    struct snake_part_s *tmp = calloc(1, sizeof(*tmp)); 
    //memcpy(tmp, snake->tail, sizeof(*tmp));
    tmp->next = NULL;
    tmp->x = snake->tail->x;
    tmp->y = snake->tail->y;
    tmp->bdy = s_bdy;
    snake->tail->next = tmp;
    snake->tail = tmp;
}

void mv_snake(struct snake_part_s *snkprt, float x, float y, struct win_s *win) {
    draw_cell(*win, (int)snkprt->x, (int)snkprt->y, blank);
    if(snkprt->next)
        mv_snake(snkprt->next, snkprt->x, snkprt->y, win); 
    snkprt->x = x;
    snkprt->y = y;
    draw_cell(*win, (int)snkprt->x, (int)snkprt->y, snkprt->bdy);
}

void handle_snake_speed(struct snake_s *snake)
{
    if((game_state.level + game_state.score) % 8 == 0) {
        snake->cvx += snake->dvx * SNAKE_SPEED_INCR_STRIDE;
        snake->cvy += snake->dvy * SNAKE_SPEED_INCR_STRIDE;
    }
}

void delete_snake(struct snake_s *snake)
{
    struct snake_part_s *tmp;
    while(snake->head) {
        tmp = snake->head;
        snake->head = snake->head->next;
        free(tmp);
    } 
    snake->head = snake->tail = NULL; 
}
