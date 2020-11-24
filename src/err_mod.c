#include "err_mod.h"

void exit_error(int errn, char *err_mes)
{
    perror(err_mes);
    exit(EXIT_FAILURE);
}

void check_alloc(void *ret, int errn, char *err_mes)
{
    if( (ret==NULL) && (errno==ENOMEM) )
        exit_error(errno, err_mes);
}
