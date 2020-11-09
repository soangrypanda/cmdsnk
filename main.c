#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


FILE *fd1; 

#define collide(ox, oy, scr, cell) (scr.win[scr.w * (int)oy+(int)ox]==cell)

#define need_to_add_food(gs) ((gs)->foods < (gs)->max_food)
#define change_food_cntr(gs, how) (((gs)->foods)how)

#define update_txt(...) sprintf( __VA_ARGS__ );
#define init_txt(obj, width)      \
        struct txt_s obj = { 0 }; \
        obj.w = width;            \
        obj.txt = malloc(obj.w)  
#define mv_txt(txt, tx, ty);      \
        (txt)->x = tx;            \
        (txt)->y = ty               
#define SCORE_TXT "score: %04d"
#define SCORE_TXT_W (sizeof(SCORE_TXT)+4)
#define LEVEL_TXT "level: %04d"
#define LEVEL_TXT_W (sizeof(LEVEL_TXT)+4)
#define TITLE_TXT "CMDSNK by soangrypanda"
#define TITLE_TXT_W (sizeof(TITLE_TXT))

#define SNAKE_SPEED_INCR_STRIDE 2 
/* --- NEED TO THINK ABOUT POSSIBILITY OF DRAWING WINS SEPARATELLY --- */
#define init_win(wind, wx, wy, ww, wh, scrn)\
    struct win_s wind = { 0 };              \
    wind.x = wx;                            \
    wind.y = wy;                            \
    wind.w = ww;                            \
    wind.h = wh;                            \
    wind.win = &((scrn)->screen[wy*((scrn)->w)+wx])
    //wind.win =  calloc((wind.w) * (wind.h) + 1, sizeof(*(wind.win)));
    //memset(wind.win, blank, wind.w*wind.h)

#define draw_win_brdr(wind, bv, bh)                     \
    for(int h = 0; h < wind.h; ++h) {                   \
        wind.win[h * wind.w] = bv;                      \
        wind.win[h * wind.w + wind.w - 1] = bv;         \
    }                                                   \
    for(int w = 0; w < wind.w; ++w) {                   \
        wind.win[w] = bh;                               \
        wind.win[wind.w * wind.h - wind.w + w] = bh;    \
    }                                                   \
    wind.win[wind.w*wind.h] = '\0'

#define draw_cell(wind, x, y, cont) ((wind).win[(wind).w * (y) + (x)] = cont)


unsigned int seed;  

struct game_state_s {
    float elapsed_time;
    int game_on;
    int score;
    int level;
    int foods, max_food, max_food_cap;
};
struct game_state_s game_state = { 0 };

int scx, scy, lvx, lvy, tlx, tly;
unsigned int ui_score_win_h = 1;

struct screen_s {
    char *screen; 
    int w, h;
};

struct win_s {
    char *win;
    int w, h, x, y;
};

struct txt_s {
    char *txt;
    int x, y, w;
};

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

enum cells {blank = '.', s_head = '@', s_bdy = '0', food = '^', brd_h = '-', brd_v = '|'};


void initcurses(void);
void init_mainscreen(struct screen_s *screen);
void handle_key(int inp, struct snake_s * snake);
void handle_food(struct win_s *gs);
float get_elapsed_time(struct timespec *t1, struct timespec *t2);
void handle_snake_collision(struct snake_s *snake, struct win_s *screen);
void enlen_snake(struct snake_s *snake);
void handle_snake_speed(struct snake_s *snake);
void mv_snake(struct snake_part_s *snkprt, float x, float y, struct win_s *win);
void update_game_state();
int find_better_cell(int *px, int *py, struct win_s *scrn, int cell_needed);

int main(void)
{
    seed = time(NULL);
    initcurses();

    struct screen_s screen = { 0 };
    init_mainscreen(&screen);
    
    init_txt(score, sizeof(SCORE_TXT) + 2);
    update_txt(score.txt, SCORE_TXT, game_state.score); 
    init_txt(level, sizeof(LEVEL_TXT) + 2);
    update_txt(level.txt, LEVEL_TXT, game_state.level); 
    init_txt(title, TITLE_TXT_W);
    update_txt(title.txt, TITLE_TXT); 

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

    init_win(game_win, 0, win.h, screen.w, screen.h-game_win.y, &screen);

    /* draw_win_brdr */ 
    draw_win_brdr(win, brd_v, brd_h);
    draw_win_brdr(game_win, brd_v, brd_h);

    game_state.game_on = 1;
    game_state.max_food = 1; 
    game_state.max_food_cap = 5; 
    
    struct snake_part_s snkprt = { 0 };
    snkprt.next = NULL;
    snkprt.x = game_win.w / 2;
    snkprt.y = game_win.h / 2;
    snkprt.bdy = s_head;

    struct snake_s snake = { 0 };
    snake.head = &snkprt;
    snake.tail = &snkprt;
    snake.vx = 0;
    snake.vy = 0;
    snake.cvx = 12;
    snake.cvy = 8;
    snake.dvx = 1;
    snake.dvy = 0.6;
    snake.nx = snkprt.x; 
    snake.ny = snkprt.y;
    snake.be_moved = 0;
    snake.dir = none; 
    
    draw_cell(game_win, (int)snkprt.x, (int)snkprt.y, snkprt.bdy);
     
    FILE *fd = fopen("log.txt", "w");
    fd1 = fopen("log.txt", "w");
    fprintf(fd, "x - %d, y - %d, scr x - %d, scr y - %d\n", win.w , win.h, screen.w, screen.h);
    //fprintf(fd, "x - %d, y - %d, sx - %f, ys - %f, sizeof - %ld\n", screen.w, screen.h, snake.x, snake.y, sizeof((*screen.screen)));
    //fprintf(fd, "x - %d, y - %d, sx - %d, ys - %d\n", x, y, snake.x, snake.y);
    

    struct timespec t1 = { 0 };
    struct timespec t2 = { 0 };
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
    
    while(game_state.game_on) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        game_state.elapsed_time = get_elapsed_time(&t1, &t2);        
        memcpy(&t2, &t1, sizeof(t1));
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
        mvaddstr(0, 0, screen.screen);
        mvaddstr(score.y,score.x,score.txt);
        mvaddstr(level.y,level.x,level.txt);
        mvaddstr(title.y,title.x,title.txt);
        refresh();
    }
    free(screen.screen);
    //free(win.win);
    //free(game_win.win);

    free(score.txt);
    free(level.txt);
    free(title.txt);

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
void init_mainscreen(struct screen_s *screen)
{
    getmaxyx(stdscr, screen->h, screen->w);
    screen->screen = calloc((screen->h)*(screen->w)+1, sizeof(*(screen->screen)));
    memset(screen->screen, blank, screen->h * screen->w);
    screen->screen[screen->h*screen->w] = '\0';
}

void handle_key(int inp, struct snake_s *snake)
{
    switch(inp){
        case(' '):
            game_state.game_on = 0; 
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
void handle_food(struct win_s *scrn)
{
    int x = 1 + rand_r(&seed) / (RAND_MAX  / (scrn->w - 2)) ;
    int y = 1 + rand_r(&seed) / (RAND_MAX  / (scrn->h - 2)) ;
    if(scrn->win[scrn->w * y + x] != blank) 
        find_better_cell(&x, &y, scrn, blank);
    scrn->win[scrn->w * y + x] = food; 
}

int find_better_cell(int *px, int *py, struct win_s *scrn, int cell_needed)
{
    if(scrn->win[scrn->w * (*py) + (*px)] == cell_needed)
        return 1;
    return 0;
}

float get_elapsed_time(struct timespec *t1, struct timespec *t2)
{
    return (float)(t1->tv_sec-t2->tv_sec) + (float)(t1->tv_nsec-t2->tv_nsec)*1e-9;
}

void handle_snake_collision(struct snake_s *snake, struct win_s *screen) {
    if(collide(snake->nx, snake->ny, (*screen), brd_v) || 
       collide(snake->nx, snake->ny, (*screen), brd_h) ||
       collide(snake->nx, snake->ny, (*screen), s_bdy)) {

        snake->nx -= snake->vx * game_state.elapsed_time;
        snake->ny -= snake->vy * game_state.elapsed_time; 

        snake->vx = 0; snake->vy = 0;
        snake->be_moved = 0;
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

void update_game_state()
{
    if(game_state.score % 10 == 0) {
        game_state.level++;
        if(game_state.max_food < game_state.max_food_cap) {
            game_state.max_food++;
        }
    }
}
