#include <stdio.h>
#include <unistd.h>      // For fork, execvp
#include <string.h>      // For strlen
#include <errno.h>       // For errno
#include <wait.h>        // For wait
#include <stdlib.h>      // For malloc
#include <assert.h>
#include "strvec.h"
#include "sighandler.h" // For g_waiting_for_child_proc

#define WRITE_END 1
#define READ_END  0

static inline int count_ch(const char * line, const char ch) {
    int counter = 0;
    for (usize_t i = 0; i < strlen(line); i++) {
        if (line[i] == ch) {
            counter++;
        }
    }
    return counter;
}

static inline int exec(str_vec_t * tokens) {
    execvp(tokens->data[0], tokens->data);
    fprintf(stderr, "fvgsh: erro ao executar '%s', código %d.\n", tokens->data[0], errno);
    return errno;
}

int exec_piped_commands(char * line) {
    int n_pipes = count_ch(line,'|');

    // If there were N pipe characters then there must be
    // N+1 commands to be run
    int n_commands = n_pipes + 1;

    // We only enter this function if a pipe
    // was found. This is basically a sanity check to
    // ensure we can't end up with a malloc(0)
    assert(n_pipes > 0);

    str_vec_t commands = tokenize(line, "|");
    if (commands.size != (usize_t) n_commands) {
        fprintf(stderr, "fvgsh: erro de sintaxe.\n");
        return 127;
    }

    int (*file_descriptors)[2];
    file_descriptors = malloc(n_pipes * sizeof *file_descriptors);

    // The status code of each command executed
    // The last one will be returned
    int status_code = 127;

    int wait_status;

    for (int i = 0; i < n_commands; i++) {
        if (i != n_pipes) {
            int rv = pipe(file_descriptors[i]);
            if (rv < 0) {
                fprintf(stderr, "fvgsh: erro ao inicializar pipe!");                
                status_code = -1;
                break;
            }
        }

        char * command = commands.data[i];
        str_vec_t tokens = tokenize(command, " ");
        append_null(&tokens);

        pid_t pid = fork();
        if (pid == 0)
        {
            // Running on the child process
            if (i != n_commands-1)
            {
                dup2(file_descriptors[i][WRITE_END], STDOUT_FILENO);
                close(file_descriptors[i][READ_END]);
                close(file_descriptors[i][WRITE_END]);
            }

            if (i != 0) {
                dup2(file_descriptors[i-1][READ_END], STDIN_FILENO);
                close(file_descriptors[i-1][WRITE_END]);
                close(file_descriptors[i-1][READ_END]);
            }

            free(file_descriptors);
            return exec(&tokens);
        }
        // Running on the parent process
        if (i > 0) {
            // Closing the file descriptors previously used
            close(file_descriptors[i - 1][READ_END]);
            close(file_descriptors[i - 1][WRITE_END]);
        }


        g_waiting_for_child_proc = true;

        if (waitpid(pid, &wait_status, 0) == -1) {
            fprintf(stderr, "waitpid falhou em exec_piped_commands!\n");
            status_code = 127;
        } else if WIFEXITED(wait_status) {
            status_code = WEXITSTATUS(wait_status);
        } else {
            fprintf(stderr, "fvgsh: aviso: processo-filho não terminou normalmente!");
            status_code = 127;
        }
        g_waiting_for_child_proc = false;

        if (status_code != 0) {
            printf("fvgsh: parando execução de cadeia de comandos uma vez que '%s' falhou ou não existe.\n", tokens.data[0]);
            vec_free(&tokens);
            break;
        }

        vec_free(&tokens);
    }


    free(file_descriptors);
    vec_free(&commands);
    return status_code;
}

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
        // Runs on the child process
        return exec(&tokens);
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

int exec_seq_commands(char * line){
    int i=0, semic_count, n_commands;
    semic_count = count_ch(line,';'); // count semicolons in command
    n_commands = semic_count + 1; // N-1 semicolons => N commands to be executed

    assert(semic_count > 0); // ensure that at least one semicolon was found

    str_vec_t commands = tokenize(line, ";"); // break individual commands
    if (commands.size != (usize_t) n_commands) {
        fprintf(stderr, "fvgsh: erro de sintaxe.\n");
        return 127;
    }
    for(i=0;i<n_commands;i++){
        exec_simple_command(commands.data[i]);    
    }
    
    vec_free(&commands);
    return 0;// success
}
