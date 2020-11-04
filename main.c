#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
 
#define collide(obj, scr,  cell) (scr.screen[scr.w * (int)obj.y+(int)obj.x]==cell)

#define SCORE_TXT "score: %04d"
#define SCORE_OUT_W (sizeof(SCORE_TXT)+2)
#define LEVEL_TXT "level: %04d"
#define LEVEL_OUT_W (sizeof(LEVEL_TXT)+2)
#define TITLE_TXT "CMDSNK by soangrypanda"
#define TITLE_OUT_W (sizeof(TITLE_TXT)+2)


struct game_state_s {
    float elapsed_time;
    int game_on;
    int score;
    int level;
};
struct game_state_s game_state = { 0 };

int scx, scy, lvx, lvy, tlx, tly;
int score_win_y = 0;
unsigned int ui_score_win_h = 1;

struct screen_s {
    char *screen; 
    int w, h;
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
void initscreen(struct screen_s *screen);
void handle_key(int inp);
float get_elapsed_time(struct timespec *t1, struct timespec *t2);
void handle_snake_collision(struct snake_s *snake, struct screen_s *screen);

int main(void)
{
    initcurses();


    struct screen_s screen = { 0 };
    initscreen(&screen);

    char score_out[SCORE_OUT_W];
    sprintf(score_out, SCORE_TXT, game_state.score);
    char level_out[LEVEL_OUT_W];
    sprintf(level_out, LEVEL_TXT, game_state.level);
    char title_out[] = TITLE_TXT;
     
    if(SCORE_OUT_W+LEVEL_OUT_W+TITLE_OUT_W < screen.w-2) {
        ui_score_win_h = 4; 
        scx = 2;
        scy = score_win_y + 2;
        lvx = screen.w - LEVEL_OUT_W + 1;
        lvy = score_win_y + 2;
        tlx = screen.w / 2 - TITLE_OUT_W / 2;
        tly = score_win_y + 2;
    }
    else {
        ui_score_win_h = 6; 
 //FINALIZE THIS PART!!!
    }

    for(int h=0; h < screen.h; ++h) {
        screen.screen[h * screen.w] = brd_v;
        screen.screen[h * screen.w + screen.w-1] = brd_v;
    }
    for(int w=0; w < screen.w; ++w) {
        screen.screen[w] = brd_h;
        screen.screen[screen.w*screen.h-screen.w+w] = brd_h; 
        screen.screen[screen.w * score_win_y+w] = brd_h;
        screen.screen[screen.w * (score_win_y+ui_score_win_h)+w] = brd_h;
    }
   
    game_state.game_on = 1;
 
    snake.x = screen.w / 2;
    snake.y = screen.h / 2;
    snake.vx = 0;
    snake.vy = 0;
    snake.cvx = 12;
    snake.cvy = 8;
/*
 * velocity proportion 1 - 0.6
 */
    snake.dir = none; 
    screen.screen[screen.w * (int)snake.y + (int)snake.x] = s_head;
     
    //FILE *fd = fopen("log.txt", "w");
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
 
        screen.screen[screen.w * (int)snake.y + (int)snake.x] = blank;

        handle_key(getch());

        snake.x += snake.vx * game_state.elapsed_time;
        snake.y += snake.vy * game_state.elapsed_time; 
        
        handle_snake_collision(&snake, &screen);
            
        screen.screen[screen.w * (int)snake.y + (int)snake.x] = s_head;
        mvaddstr(0,0,screen.screen); 
        mvaddstr(scy,scx,score_out);
        mvaddstr(lvy,lvx,level_out); 
        mvaddstr(tly,tlx,title_out); 
        refresh();
    }
    free(screen.screen);
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
void initscreen(struct screen_s *screen)
{
    getmaxyx(stdscr, screen->h, screen->w);
    screen->screen = calloc(screen->h*screen->w, sizeof(*(screen->screen)));
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

float get_elapsed_time(struct timespec *t1, struct timespec *t2)
{
    return (float)(t1->tv_sec-t2->tv_sec) + (float)(t1->tv_nsec-t2->tv_nsec)*1e-9;
}

void handle_snake_collision(struct snake_s *snake, struct screen_s *screen) {
    if(collide((*snake), (*screen), brd_v) || collide((*snake), (*screen), brd_h)) {
        snake->x -= snake->vx * game_state.elapsed_time;
        snake->y -= snake->vy * game_state.elapsed_time; 
        snake->vx = 0; snake->vy = 0;
    }
    if(collide((*snake), (*screen), food)) {
        game_state.score++;
    }
}
