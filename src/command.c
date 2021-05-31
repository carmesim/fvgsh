#include "command.h"   // For fd_t, command_t
#include "string.h"    // For strtok_r, strstr
#include "stdlib.h"    // For malloc
#include "stdio.h"     // For printf, putchar

#define STR(x) #x

static inline bool contains(const char * haystack, const char * needle) {
    return !!strstr(haystack, needle);
}

static inline bool xor_3(bool a, bool b, bool c) {
    return (a?1:0) + (b?1:0) + (c?1:0) == 1;
}

command_type_t parse_command_type(const char * line) {

    bool piped = contains(line, " | ");
    bool or = contains(line, " || ");
    bool and = contains(line, " && ");
    bool seq = contains(line, ";");
    bool logical = and || or;
    bool regular = !piped && !logical && !seq;

    if(!regular && !xor_3(piped, seq, logical)) {
        return Malformed;
    }

    if (piped) {
        return Piped;
    }

    if (logical) {
        return Logical;
    }

    if (seq) {
        return Sequential;
    }

    return Regular;
}
