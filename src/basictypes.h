#ifndef BASICTYPES_H
#define BASICTYPES_H

#include <limits.h>
#include <stdint.h>

#ifndef ARG_MAX
#   define ARG_MAX 4096 // minimum acceptable value according to the POSIX standard
#endif

#ifndef PATH_MAX
#   define PATH_MAX ARG_MAX
#endif


#if UINTPTR_MAX == 0xffffffff
typedef uint32_t usize_t;
#elif UINTPTR_MAX == 0xffffffffffffffff
typedef uint64_t usize_t;
#else
#   warning "Could not detect memory address width. Assuming 64 bits."
typedef uint64_t usize_t;
#endif

#ifdef __STDC_VERSION__
// __STDC_VERSION__ is defined from C99 and up
#   include <stdbool.h>
#else
// Not running on C99 (or later) so we'll have to define our own bool
    typedef enum { false, true, } bool;
#endif

#endif // BASICTYPES_H
