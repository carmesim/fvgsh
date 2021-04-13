#include <stdio.h>
#include "types.h"
#include "sighandler.h"

int main()
{
    // Starting up the signal handler
    init_signal_handler();

    char ch;

    while(!g_should_exit) {
        // line is initialized filled with \0
        char line[ARG_MAX] = { '\0' };

        for(int i = 0;
            (i < ARG_MAX - 1)                  // Make sure our buffer isn't bigger than ARG_MAX
                && ((ch = getchar()) != '\n')  // Stop reading if a newline is read
                && (ch != EOF);                // Stop reading if an EOF is read
            ++i)
        {
            line[i] = ch;
        }

        printf("%s", line);
    }

    return 0;
}
