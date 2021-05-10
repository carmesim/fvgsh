#include "command.h"   // For fd_t, command_t
#include "string.h"    // For strtok_r, strstr
#include "stdlib.h"    // For malloc
#include "stdio.h"     // For printf, putchar

static inline bool contains(const char * haystack, const char * needle) {
    return !!strstr(haystack, needle);
}

command_type_t parse_command_type(const char * line) {
    if (contains(line, " | ")) {
        return Piped;
    }

    if (contains(line, " || ") || contains(line, " && ")) {
        return Logical;
    }

    if (contains(line, ";")) {
        return Sequential;
    }

    return Regular;
}
