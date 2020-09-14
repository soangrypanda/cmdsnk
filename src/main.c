#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>


/* THIS IS MODEL PART */

enum bdy_symb { head='H' , body='@' };
struct snk_prt {
    enum bdy_symb bdy;
    int y, x;
    struct snk_prt *next;
};

enum brd_symb { brdr='/', obstcl='*', food='^' } brd;        
struct board {
    enum brd_symb brd;
    enum brd_symb **brd_arr;
    int w, h, brd_size;
};

void first_init(void);
/* END OF MODEL PART */


/* THIS IS VIEW PART */
void draw_brd(struct board brd);
/* END OF VIEW PART */

int main(void)
{
    int key;
    struct board brd1;
    first_init();
    getmaxyx(stdscr, brd1.w , brd1.h);
    brd1.brd = brdr;
    brd1.brd_arr = malloc(brd1.w * brd1.h * sizeof(enum brd_symb));    
    //brd1.brd_arr[0][0] = brdr;
    move(10, 10);
    //addch(brd1.brd_arr[0][0]);
    draw_brd(brd1);

    struct snk_prt snk1;
    snk1.y = brd1.w / 2;
    snk1.x = brd1.h / 2;
    snk1.bdy = head;
    move(snk1.y, snk1.x);
    addch(snk1.bdy);
    curs_set(0);


    while((key = getch()) != ' ')
        {}
  
    endwin();
    return 0;
}

void first_init(void)
{
    initscr();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
}

void draw_brd(struct board brd) 
{
    if (brd.w <= 0 || brd.h <= 0)
        return;
    for (int w = 0; w <= brd.w; w++) {
        move(w, 0);
        addch(brd.brd);
        move(w, brd.h-1);
        addch(brd.brd); 
    }
    for (int h = 0; h <= brd.h; h++) {
        move(0, h);
        addch(brd.brd);
        move(brd.w-1, h);
        addch(brd.brd); 
    }
    curs_set(0);
}
