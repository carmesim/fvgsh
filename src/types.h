#ifndef TYPES_H
#define TYPES_H

#include <limits.h>

#ifndef ARG_MAX
#   define ARG_MAX 4096 // minimum acceptable value according to the POSIX standard
#endif

#ifndef PATH_MAX
#   define PATH_MAX ARG_MAX
#endif

#ifdef __STDC_VERSION__
// __STDC_VERSION__ is defined from C99 and up
#   include <stdbool.h>
#else
// Not running on C99 (or later) so we'll have to define our own bool
    typedef enum { false, true, } bool;
#endif

#endif // TYPES_H
