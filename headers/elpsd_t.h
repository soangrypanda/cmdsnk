#ifndef CMDSNK_ELPSD_T_H_SENTRY
#define CMDSNK_ELPSD_T_H_SENTRY


#include <time.h>
#include <string.h>

#define CLCK CLOCK_MONOTONIC_RAW

void set_timer(void);
float get_elapsed_time(void);


#endif
