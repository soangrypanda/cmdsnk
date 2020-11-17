#ifndef CMD_SNK_CELL_MOD_H_CENTRY
#define CMD_SNK_CELL_MOD_H_CENTRY


#define draw_cell(wind, x, y, cont) ((wind).win[(wind).w * (y) + (x)] = cont)
#define in_scope(x,y,scrn) ( ((x)>0)&&((x)<(scrn)->w)&&((y)>0)&&((y)<(scrn)->h) )
int already_seen(int x, int y, int *xa, int *ya, int len);

int find_better_cell(int *px, int *py, int x, int y, struct win_s *scrn, int cell_needed)
{
    static int cnt = 0;
    cnt++;
    int size = (scrn->w)*(scrn->y);
    int *xfront = calloc(size, sizeof(*xfront));
    int *yfront = calloc(size, sizeof(*yfront));
    int *xseen = calloc(size, sizeof(*xseen));
    int *yseen = calloc(size, sizeof(*yseen));
/* -- TOO BIG ALLOC OVERHEAD - THINK ABOUT OPTIMIZATION -- */
 
    int fc = 0; int fh = 0;
    int sc = 0;
    int ret = 0;
    for(;;) {
        if(scrn->win[scrn->w * y + x] == cell_needed) {
            *px = x; *py = y; ret = 1; goto cleanup_n_return;
        }
        if(sc >= size) {
            ret = 0;
            goto cleanup_n_return;
        }
        xseen[sc] = x; yseen[sc] = y; ++sc;
 
        if((in_scope(x+1,y,scrn)) && (!already_seen(x+1, y, xseen, yseen, sc))) {
            xfront[fc] = x + 1; yfront[fc] = y; ++fc;
        }
        if((in_scope(x-1,y,scrn)) && (!already_seen(x-1, y, xseen, yseen, sc))) {
            xfront[fc] = x - 1; yfront[fc] = y; ++fc;
        }
        if((in_scope(x,y+1,scrn)) && (!already_seen(x, y+1, xseen, yseen, sc))) {
            xfront[fc] = x; yfront[fc] = y + 1; ++fc;
        }
        if((in_scope(x,y-1,scrn)) && (!already_seen(x, y-1, xseen, yseen, sc))) {
            xfront[fc] = x; yfront[fc] = y - 1; ++fc;
        }

    pop_new_coords:
        if(fh >= size) {
            ret = 0;
            goto cleanup_n_return;
        }
        x = xfront[fh]; y = yfront[fh]; ++fh;
        if(already_seen(x, y, xseen, yseen, sc))
            goto pop_new_coords;
    }

    cleanup_n_return:
        free(xfront); free(yfront); free(xseen); free(yseen);
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


#endif
