#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


FILE *fd1; 

#define collide(obj, scr,  cell) (scr.win[scr.w * (int)obj.y+(int)obj.x]==cell)
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

unsigned int seed;  

struct game_state_s {
    float elapsed_time;
    int game_on;
    int score;
    int level;
    int foods, max_food;
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

struct snake_s {
    float x, y;
    float vx, vy;
    float cvx, cvy;
    char dir;
} snake;
enum snake_dir {up, left, down, right, none};

enum cells {blank = '.', s_head = '@', food = '^', brd_h = '-', brd_v = '|'};


void initcurses(void);
void init_mainscreen(struct screen_s *screen);
void handle_key(int inp);
void handle_food(struct win_s *gs);
float get_elapsed_time(struct timespec *t1, struct timespec *t2);
void handle_snake_collision(struct snake_s *snake, struct win_s *screen);

int main(void)
{
    seed = time(NULL);
    initcurses();

    struct screen_s screen = { 0 };
    init_mainscreen(&screen);
    
    struct win_s win = { 0 };
    win.w = screen.w; 
    win.x = 0;
    win.y = 0;
    
    init_txt(score, sizeof(SCORE_TXT) + 2);
    update_txt(score.txt, SCORE_TXT, game_state.score); 
    
    init_txt(level, sizeof(LEVEL_TXT) + 2);
    update_txt(level.txt, LEVEL_TXT, game_state.level); 

    init_txt(title, TITLE_TXT_W);
    update_txt(title.txt, TITLE_TXT); 

    if(score.w+level.w+title.w< screen.w-2) {
        win.h = 4; 
        mv_txt(&score, 2, win.y + 2);
        mv_txt(&level, win.w - level.w + 1, win.y + 2);
        mv_txt(&title, win.w / 2 - title.w / 2, win.y);
    }
    else {
        win.h = 6; 
        mv_txt(&score, 2, win.y + 2);
        mv_txt(&level, 2, win.y + 4);
        mv_txt(&title, win.w / 2 - title.w / 2, win.y);
    }

    struct win_s game_win = { 0 };
    game_win.x = 0;
    game_win.y = win.h;
    game_win.w = screen.w;
    game_win.h = screen.h - game_win.y; 
   
    /* draw_win_brdr */ 
    win.win = calloc((win.w) * (win.h) + 1, sizeof(*(win.win)));
    memset(win.win, blank, win.w*win.h);
    for(int h = 0; h < win.h; ++h) {
        win.win[h * win.w] = brd_v;
        win.win[h * win.w + win.w - 1] = brd_v; 
    }
    for(int w = 0; w < win.w; ++w) {
        win.win[w] = brd_h;
        win.win[win.w * win.h - win.w + w] = brd_h;
    } 
    win.win[win.w*win.h] = '\0';

    game_win.win = calloc((game_win.w) * (game_win.h) + 1, sizeof(*(game_win.win)));
    memset(game_win.win, blank, game_win.w*game_win.h);
    for(int h = 0; h < game_win.h; ++h) {
        game_win.win[h * game_win.w] = brd_v;
        game_win.win[h * game_win.w + game_win.w - 1] = brd_v; 
    }
    for(int w = 0; w < game_win.w; ++w) {
        game_win.win[w] = brd_h;
        game_win.win[game_win.w * game_win.h - game_win.w + w] = brd_h;
    } 
    game_win.win[game_win.w*game_win.h] = '\0';
/*
    for(int h=0; h < screen.h; ++h) {
        screen.screen[h * screen.w] = brd_v;
        screen.screen[h * screen.w + screen.w-1] = brd_v;
    }
    for(int w=0; w < screen.w; ++w) {
        screen.screen[w] = brd_h;
        screen.screen[screen.w*screen.h-screen.w+w] = brd_h; 
        screen.screen[screen.w * win.y+w] = brd_h;
        screen.screen[screen.w * (win.y+win.h)+w] = brd_h;
    }
*/   
    game_state.game_on = 1;
    game_state.max_food = 1; 

    snake.x = game_win.w / 2;
    snake.y = game_win.h / 2;
    snake.vx = 0;
    snake.vy = 0;
    snake.cvx = 12;
    snake.cvy = 8;
/*
 * velocity proportion 1 - 0.6
 */
    snake.dir = none; 
    game_win.win[game_win.w * (int)snake.y + (int)snake.x] = s_head;
     
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
 
        handle_key(getch());
        if(need_to_add_food(&game_state)) {
            handle_food(&game_win);
            change_food_cntr(&game_state, ++);
        }
        //fprintf(fd, "%d - %d\n", game_state.foods, game_state.max_food);
        game_win.win[game_win.w * (int)snake.y + (int)snake.x] = blank;
        snake.x += snake.vx * game_state.elapsed_time;
        snake.y += snake.vy * game_state.elapsed_time; 
        
        handle_snake_collision(&snake, &game_win);
            
        game_win.win[game_win.w * (int)snake.y + (int)snake.x] = s_head;

        update_txt(score.txt, SCORE_TXT, game_state.score); 
        update_txt(level.txt, LEVEL_TXT, game_state.level); 

        mvaddstr(win.y, win.x, win.win);
        mvaddstr(game_win.y, game_win.x, game_win.win);
        //mvaddstr(0,0,screen.screen); 
        mvaddstr(score.y,score.x,score.txt);
        mvaddstr(level.y,level.x,level.txt);
        mvaddstr(title.y,title.x,title.txt);
        refresh();
    }
    free(screen.screen);
    free(win.win);
    free(game_win.win);

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

void handle_key(int inp)
{
    switch(inp){
        case(' '):
            game_state.game_on = 0; 
            break;
        case('l'):
            if(snake.dir == left)
                break;
            snake.vx = snake.cvx; 
            snake.vy = 0; 
            snake.dir = right;
            break;
        case('h'):
            if(snake.dir == right)
                break;
            snake.vx = -snake.cvx;
            snake.vy = 0;
            snake.dir = left;
            break;
        case('k'):
            if(snake.dir == down)
                break;
            snake.vx = 0;
            snake.vy = -snake.cvy;
            snake.dir = up;
            break;
        case('j'):
            if(snake.dir == up)
                break;
            snake.vx = 0;
            snake.vy = snake.cvy;
            snake.dir = down;
            break;
        default:
            break;
    }
}

void handle_food(struct win_s *scrn)
{
/*
 * DELETE IT AFTERWARDS!
 
    int r = rand_r(&seed);
    int x = 1 + (int) (((float)(scrn->w - 2) * rand_r(&seed)) / (RAND_MAX ));
    int y = 1 + (int) (((float)(scrn->h - 2) * rand_r(&seed)) / (RAND_MAX ));
 */

    int x = 1 + rand_r(&seed) / (RAND_MAX  / (scrn->w - 2)) ;
    int y = 1 + rand_r(&seed) / (RAND_MAX  / (scrn->h - 2)) ;

/*
    fprintf(fd1, "x - %d, y - %d, scrn w - %d, scrn h - %d, r  - %d\n", x, y, scrn->w, scrn->h, x);
    int x = scrn->x + rand_r(&seed) * (scrn->w - scrn->y) / RAND_MAX; 
    int y = scrn->y + rand_r(&seed) * (scrn->h - scrn->y) / RAND_MAX; 
*/
    scrn->win[scrn->w * y + x] = food; 
}

float get_elapsed_time(struct timespec *t1, struct timespec *t2)
{
    return (float)(t1->tv_sec-t2->tv_sec) + (float)(t1->tv_nsec-t2->tv_nsec)*1e-9;
}

void handle_snake_collision(struct snake_s *snake, struct win_s *screen) {
    if(collide((*snake), (*screen), brd_v) || collide((*snake), (*screen), brd_h)) {
        snake->x -= snake->vx * game_state.elapsed_time;
        snake->y -= snake->vy * game_state.elapsed_time; 
        snake->vx = 0; snake->vy = 0;
    }
    if(collide((*snake), (*screen), food)) {
        game_state.score++;
        change_food_cntr(&game_state, --);
    }
}
