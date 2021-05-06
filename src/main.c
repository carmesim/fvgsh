#include <stdio.h>
#include <stdlib.h>      // For free
#include <string.h>      // For strlen
#include <unistd.h>      // For fork, execvp
#include <errno.h>       // For errno
#include <wait.h>        // For wait
#include "basictypes.h"  // For ARG_MAX
#include "sighandler.h"  // For init_signal_handler
#include "userdata.h"    // For user_data_t, get_user_data
#include "command.h"     // For parse_line
#include "strvec.h"

#define RED_ANSI     "\x1b[31m" // ANSI escape code for red
#define BLUE_ANSI    "\x1b[34m" // ANSI escape code for blue
#define RESET_ANSI   "\x1b[0m"  // ANSI escape code to reset stdout's color

static inline void print_prompt(const user_data_t * ud)
{
    printf(BLUE_ANSI "%s@%s:" RED_ANSI "%s" BLUE_ANSI "$ " RESET_ANSI, ud->username, ud->hostname, ud->pretty_cwd);
}

int main()
{
    // Starting up the signal handler
    init_signal_handler();

    user_data_t ud = get_user_data();

    char ch;

    while(!g_should_exit && !g_waiting_for_child_proc) {

        // line is initialized filled with \0
        char line[ARG_MAX] = { '\0' };
        print_prompt(&ud);

        // Checking for EOF
        if (feof(stdin)) {
            fprintf(stderr, "EOF read. Exiting.\n");
            break;
        }

        for(int i = 0;
            (i < ARG_MAX - 1)                  // Make sure our buffer isn't bigger than ARG_MAX
                && ((ch = getchar()) != '\n')  // Stop reading if a newline is read
                && (ch != EOF);                // Stop reading if an EOF is read
            ++i)
        {
            line[i] = ch;
        }

        if (line[0] == '\0') {
            continue;
        }

        str_vec_t tokens = tokenize(line);
        for (usize_t i = 0; i < tokens.size; i++) {
            printf("Token: %s\n", tokens.data[i]);
        }

        // Input to execvp must be NULL-terminated
        append_null(&tokens);

//================= TEMPORARY STUFF =======================
// MAYBE WE SHOULD MOVE THIS TO A FUNCTION IN THE FUTURE

        int pid = fork();
        if(pid == 0){
            //child process
            char * last_token = tokens.data[tokens.size-1];
            char last_char = last_token[strlen(last_token)];
            /*
                TODO: handle "|" "<" ">"
                    
                design a logic that uses dup2 to overwrite
                the standard inputs and outputs to file descriptors
                associated with pipes (chain of commands)

            */
            if (last_char == '&') {
                // TODO: background execution
            }
            int status = execvp(tokens.data[0], tokens.data);
            if (status == -1){
                printf("Erro! Código do erro:%d\n", errno);
            }
        } else {
            //parent process
            g_waiting_for_child_proc = true;
            wait(NULL);
        }
// =================================================================
        vec_free(&tokens);
        g_waiting_for_child_proc = false;
        fflush(stdout);
    }

    // Freeing malloc'd memory
    free(ud.pretty_cwd);
    free(ud.username);

    return 0;
}
