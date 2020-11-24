#include "rand_mod.h"

unsigned int seed;

void init_rand(void)
{   
    seed = time(NULL);
}
