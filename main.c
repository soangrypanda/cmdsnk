#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
 
#define collide(obj, cell) (screen[x * (int)obj.y+(int)obj.x]==cell)

#define SCORE_TXT "score: %04d"
#define SCORE_OUT_W (sizeof(SCORE_TXT)+2)
#define LEVEL_TXT "level: %04d"
#define LEVEL_OUT_W (sizeof(LEVEL_TXT)+2)
#define TITLE_TXT "CMDSNK by soangrypanda"
#define TITLE_OUT_W (sizeof(TITLE_TXT)+2)

void initcurses(void);

void key_handler(int inp);

float get_elapsed_time(struct timespec *t1, struct timespec *t2);


int x,y;
int b_game_on = 1;
int i_score = 0;
int i_level = 0;

int scx, scy, lvx, lvy, tlx, tly;
int score_win_y = 0;
unsigned int ui_score_win_h = 1;

struct snake_t {
    float x, y;
    float vx, vy;
    float cvx, cvy;
    char dir;
} snake;
enum snake_dir {up, left, down, right, none};
enum cells {blank = '.', s_head = '@', food = '^', brd_h = '-', brd_v = '|'};



int main(void)
{
    initcurses();
    char *screen = calloc(y*x, sizeof(*screen));
    memset(screen, blank, y*x);
    screen[y*x] = '\0';

    char score_out[SCORE_OUT_W];
    sprintf(score_out, SCORE_TXT, i_score);
    char level_out[LEVEL_OUT_W];
    sprintf(level_out, LEVEL_TXT, i_score);
    char title_out[] = TITLE_TXT;
     
    if(SCORE_OUT_W+LEVEL_OUT_W+TITLE_OUT_W < x-2) {
        ui_score_win_h = 4; 
        scx = 2;
        scy = score_win_y + 2;
        lvx = x - LEVEL_OUT_W + 1;
        lvy = score_win_y + 2;
        tlx = x / 2 - TITLE_OUT_W / 2;
        tly = score_win_y + 2;
    }
    else {
        ui_score_win_h = 6; 
 //FINALIZE THIS PART!!!
    }

    for(int h=0; h < y; ++h) {
        screen[h * x] = brd_v;
        screen[h * x + x-1] = brd_v;
    }
    for(int w=0; w < x; ++w) {
        screen[w] = brd_h;
        screen[x*y-x+w] = brd_h; 
        screen[x * score_win_y+w] = brd_h;
        screen[x * (score_win_y+ui_score_win_h)+w] = brd_h;
    }
   
     
    snake.x = x / 2;
    snake.y = y / 2;
    snake.vx = 0;
    snake.vy = 0;
    snake.cvx = 12;
    snake.cvy = 8;
/*
 * velocity proportion 1 - 0.6
 */
    snake.dir = none; 
    screen[x * (int)snake.y + (int)snake.x] = s_head;
    
    FILE *fd = fopen("log.txt", "w");
    //fprintf(fd, "x - %d, y - %d, sx - %d, ys - %d\n", x, y, snake.x, snake.y);

    float f_elapsed_time = 0;
    struct timespec t1 = { 0 };
    struct timespec t2 = { 0 };
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
    
    while(b_game_on) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        f_elapsed_time = get_elapsed_time(&t1, &t2);        
        memcpy(&t2, &t1, sizeof(t1));
        //fprintf(fd, "et = %f\n", elapsed_time);
 
        screen[x * (int)snake.y + (int)snake.x] = blank;
        key_handler(getch());
        snake.x += snake.vx * f_elapsed_time;
        snake.y += snake.vy * f_elapsed_time; 
/*
        if(snake.x > 0 && snake.x < x && snake.y > 0 && snake.y < y) {
        }        
        else {
            snake.x -= snake.vx * f_elapsed_time;
            snake.y -= snake.vy * f_elapsed_time; 
            snake.vx = 0; snake.vy = 0;
        }
*/

        if(collide(snake, brd_v) || collide(snake, brd_h)) {
            snake.x -= snake.vx * f_elapsed_time;
            snake.y -= snake.vy * f_elapsed_time; 
            snake.vx = 0; snake.vy = 0;
        }

        if(screen[x * (int)snake.y + (int)snake.x] == food) {
            i_score++;
        }
            
        screen[x * (int)snake.y + (int)snake.x] = s_head;
        mvaddstr(0,0,screen); 
        mvaddstr(scy,scx,score_out);
        mvaddstr(lvy,lvx,level_out); 
        mvaddstr(tly,tlx,title_out); 
        refresh();
    }
    free(screen);
    endwin();
    return EXIT_SUCCESS;
}

void initcurses(void)
{
    initscr();
    getmaxyx(stdscr, y, x);
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, 1); 
    timeout(0);
}

void key_handler(int inp)
{
    switch(inp){
        case(' '):
            b_game_on = 0; 
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

