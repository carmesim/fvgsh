#include <stdio.h>
#include <stdlib.h>      // For free
#include <string.h>      // For strcmp
#include <unistd.h>
#include "basictypes.h"  // For ARG_MAX
#include "sighandler.h"  // For init_signal_handler
#include "userdata.h"    // For user_data_t, get_user_data
#include "command.h"     // For parse_line, parse_command_type
#include "strvec.h"      //
#include "strutils.h"    // For trim_semic
#include "exec.h"        // For exec_simple_command, exec_piped_commands, running_jobs

#define RED_ANSI     "\x1b[31m" // ANSI escape code for red
#define BLUE_ANSI    "\x1b[34m" // ANSI escape code for blue
#define RESET_ANSI   "\x1b[0m"  // ANSI escape code to reset stdout's color


const char* command_types_str[] = {
    [Piped] = "Piped",
    [Logical] = "Logical",
    [Sequential] = "Sequential",
    [Regular] = "Regular",
    [Malformed] = "Malformed"
};

static inline void print_prompt( user_data_t * ud)
{
    printf(BLUE_ANSI "%s@%s:" RED_ANSI "%s" BLUE_ANSI "$ " RESET_ANSI, ud->username, ud->hostname, ud->pretty_cwd);
}

int main()
{
    // Starting up the signal handler
    init_signal_handler();

    user_data_t ud = get_user_data();

    char ch;
    
    joblist_init(&running_jobs);

    while(!g_should_exit && !g_waiting_for_child_proc) {

        // line is initialized filled with \0
        char line[ARG_MAX] = { '\0' };

        // Checking for EOF
        if (feof(stdin)) {
            fprintf(stderr, "EOF read. Exiting.\n");
            break;
        }

        print_prompt(&ud);

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

        // =============  background execution handling ============
        if(line[0] == 'f' && line[1] == 'g'){
            if(running_jobs.size == 0){
                printf("fg: não já processos rodando em segundo plano\n");
            }else{
                restore_command(running_jobs.list[running_jobs.size - 1]);
            }
            continue;
        }

        //always checks if there is something to remove from running_jobs
        joblist_verify(&running_jobs);

        if(strcmp(line,"jobs") == 0){
            joblist_print(&running_jobs);
            continue;
        }
        // =============  end of background execution handling ============

        trim_semic(line);//remove final semicolon

        command_type_t cmd_type = parse_command_type(line);

        //printf("Command type: %s\n", command_types_str[cmd_type]);
        //chdir("/home/gustavo/UNIFESP/Sistemas Operacionais/Labs/Shell/unix-shell/nova_pasta");
        //printf("Command type: %s\n", ud.cwd);

        switch (cmd_type) {
            case Piped: exec_piped_commands(line, &ud); break;
            case Sequential: exec_seq_commands(line, &ud); break;
            case Logical: exec_log_commands(line, &ud); break;
            case Malformed: fprintf(stderr, "fvgsh: erro: no momento não é possível mistura de operadores além de '&&' com '||'.\n"); break;
            case Regular: exec_simple_command(line, &ud); break;
        }

        fflush(stdout);
    }

    // Freeing malloc'd memory
    free(ud.pretty_cwd);
    free(ud.username);
    free(ud.home_dir);

    return 0;
}
