#include "cell_mod.h"

static int *xfront; static int *yfront; static int *xseen; static int *yseen;
static size_t size;
static int misses;

int find_better_cell(int *px, int *py, int x, int y, struct win_s *scrn, int cell_needed)
{
    if(size != (scrn->w)*(scrn->h)) {
        misses = 0;
        size = (scrn->w)*(scrn->h);
        xfront = realloc(xfront, size * sizeof(*xfront));
        check_alloc(xfront, errno, "find_better_cell");
        yfront = realloc(yfront, size * sizeof(*yfront));
        check_alloc(yfront, errno, "find_better_cell");
        xseen  = realloc(xseen,  size * sizeof(*xseen ));
        check_alloc(xseen, errno, "find_better_cell");
        yseen  = realloc(yseen,  size * sizeof(*yseen ));
        check_alloc(yseen, errno, "find_better_cell");
    }
    else {
        memset(xfront, 0, size * sizeof(*xfront));
        memset(yfront, 0, size * sizeof(*yfront));
        memset(xseen,  0, size * sizeof(*xseen ));
        memset(yseen,  0, size * sizeof(*yseen ));
    }
/* -- USE REALLOCARRAY INSTEAD? -- */

    FILE *tfd = fopen("test", "a");
    char *test = malloc(size * sizeof(*test));
    for(int i = 0; i < size; ++i)
        test[i] = 'B'; 
    size_t fc = 0; size_t fh = 0;
    size_t sc = 0;
    size_t ret = 0;
    for(;;) {
        if(scrn->win[scrn->w * y + x] == cell_needed) {
            *px = x; *py = y; ret = -1; goto cleanup_n_return;
        }
        if(sc >= size) {
            fprintf(tfd, "%s\n", "SH!");
            misses++;
            ret = 0;
            goto cleanup_n_return;
        }
        if(!already_seen(x, y, xseen, yseen, sc)) {
            xseen[sc] = x; yseen[sc] = y; ++sc;
        }

        if((fc<size)&&(in_scope(x+1,y,scrn))&&(!already_seen(x+1, y, xseen, yseen, sc))){
            xfront[fc] = x + 1; yfront[fc] = y; ++fc; test[(scrn->w)*(y)+(x+1)]='A';
        }
        if((fc<size)&&(in_scope(x-1,y,scrn))&&(!already_seen(x-1, y, xseen, yseen, sc))){
            xfront[fc] = x - 1; yfront[fc] = y; ++fc; test[(scrn->w)*(y)+(x-1)]='A';
        }
        if((fc<size)&&(in_scope(x,y+1,scrn))&&(!already_seen(x, y+1, xseen, yseen, sc))){
            xfront[fc] = x; yfront[fc] = y + 1; ++fc; test[(scrn->w)*(y+1)+(x)]='A';
        }
        if((fc<size)&&(in_scope(x,y-1,scrn))&&(!already_seen(x, y-1, xseen, yseen, sc))){
            xfront[fc] = x; yfront[fc] = y - 1; ++fc; test[(scrn->w)*(y-1)+(x)]='A';
        }
 
    pop_new_coords:
        if(fh >= size) {
            fprintf(tfd, "%s\n", "FH!");
            misses++;
            ret = 0;
            goto cleanup_n_return;
        }
        x = xfront[fh]; y = yfront[fh]; ++fh;
        if(already_seen(x, y, xseen, yseen, sc))
            goto pop_new_coords;
    }

    cleanup_n_return:
        //fprintf(tfd, "%s", test);
        free(test);
        //free(xfront); free(yfront); free(xseen); free(yseen);
        return ret;
}

int already_seen(int x, int y, int *xa, int *ya, int len)
{
    for(int i = 0; i < len; ++i) {
        if((xa[i] == x) && (ya[i] == y))
            return 1;
    }
    return 0;
}

void cell_mod_cleanup(void)
{
    free(xfront); free(yfront); free(xseen); free(yseen);
    xfront=NULL;  yfront=NULL;  xseen=NULL;  yseen=NULL;
    size = 0; misses = 0;
}
