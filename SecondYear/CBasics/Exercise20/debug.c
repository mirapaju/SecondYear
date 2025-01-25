#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

static int level = 0;

void set_debug_level(int debug_level){
    level = debug_level;
}

//if the given debug_level is lower than stored level, prints stderr msg
int dprintf(int debug_level, const char *fmt, ...){
    if (debug_level <= level){
        va_list argptr;
        va_start(argptr, fmt);
        fprintf(stderr, "[DBG%d] ", debug_level);
        vfprintf(stderr, fmt, argptr);
        va_end(argptr);
    }
    return 0;
}

