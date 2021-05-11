#include <stdio.h>
#include <unistd.h>      // For fork, execvp
#include <string.h>      // For strlen
#include <errno.h>       // For errno
#include <wait.h>        // For wait
#include "strvec.h"
#include "sighandler.h" // For g_waiting_for_child_proc

int exec_simple_command(char * line) {

    str_vec_t tokens = tokenize(line, " ");
    for (usize_t i = 0; i < tokens.size; i++) {
        printf("Token: %s\n", tokens.data[i]);
    }

    // Input to execvp must be NULL-terminated
    append_null(&tokens);

    int pid = fork();
    int wait_status;
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
        if (status == -1) {
            fprintf(stderr, "fvgsh: erro ao executar '%s', código %d.\n", tokens.data[0], errno);
        }
    }
    //parent process
    g_waiting_for_child_proc = true;

    int ret_val;

    if (waitpid(pid, &wait_status, 0) == -1) {
        fprintf(stderr, "waitpid falhou em exec_simple_command!\n");
        ret_val = 127;
    } else if WIFEXITED(wait_status) {
        ret_val = WEXITSTATUS(wait_status);
    } else {
        fprintf(stderr, "fvgsh: aviso: processo-filho não terminou normalmente!");
        ret_val = 127;
    }

    vec_free(&tokens);
    g_waiting_for_child_proc = false;

    return ret_val;
}
