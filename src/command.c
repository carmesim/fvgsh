#include "command.h"   // For fd_t, command_t
#include "string.h"    // For strtok_r
#include "stdlib.h"    // For malloc
#include "stdio.h"     // For printf, putchar

void parse_line(char * line) {
    // Context pointer for strtok_r
    char * ctx;
    char * first_token = strtok_r(line, " ", &ctx);
    char * token;

    printf("Tokens: '%s' ", first_token);

    while ((token = strtok_r(NULL, " ", &ctx)))
    {
        printf("'%s' ", token);
    }
    putchar('\n');
}
