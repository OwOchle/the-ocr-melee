#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

#include "verbose.h"

#ifdef VERBOSE
#define verb true;
#endif

#ifndef VERBOSE
#define verb false;
#endif

bool __verbose = verb;

int verbose_printf(const char * restrict format, ...)
{
    if (!__verbose)
    {
        return 0;
    }

    va_list args;
    va_start(args, format);
    int ret = vprintf(format, args);
    va_end(args);

    return ret;
}

void set_verbose(bool verbose)
{
    __verbose = verbose;
}
