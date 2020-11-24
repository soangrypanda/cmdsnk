#ifndef CMDSNK_RAND_MOD_H_SENTRY
#define CMDSNK_RAND_MOD_H_SENTRY

#include <time.h>
#include <stdlib.h>

extern unsigned int seed;

#define get_rand_n(min, max) ( (min) + rand_r(&seed) / (RAND_MAX  / ((max) - 2)) )

void init_rand(void);

#endif
