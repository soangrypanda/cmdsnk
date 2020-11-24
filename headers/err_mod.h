#ifndef ERR_MOD_H_SENTRY
#define ERR_MOD_H_SENTRY

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void exit_error(int errn, char *err_mes);
void check_alloc(void *ret, int errn, char *err_mes);


#endif
