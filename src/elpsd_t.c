#include "elpsd_t.h"

static struct timespec t1 = { 0 };
static struct timespec t2 = { 0 };

void set_timer(void)
{
    clock_gettime(CLCK, &t1);
    clock_gettime(CLCK, &t2);
}

float get_elapsed_time(void)
{
    memcpy(&t2, &t1, sizeof(t2));
    clock_gettime(CLCK, &t1);
    return (float)(t1.tv_sec-t2.tv_sec) + (float)(t1.tv_nsec-t2.tv_nsec)*1e-9;
}
