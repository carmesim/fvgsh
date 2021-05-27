#include <stdio.h>
#include <unistd.h>      // For fork, execvp
#include <string.h>      // For strlen
#include <errno.h>       // For errno
#include <wait.h>        // For wait
#include <stdlib.h>      // For malloc
#include <assert.h>
#include "strvec.h"
#include "userdata.h"
#include "sighandler.h"  // For g_waiting_for_child_proc
#include "jobs.h"        // For jobList
#include "strutils.h"    // For get_pretty_cwd


#define WRITE_END 1
#define READ_END  0

int lastpid = -1;
jobList running_jobs;
user_data_t ud;

static inline int count_ch(const char * line, const char ch) {
    int counter = 0;
    for (usize_t i = 0; i < strlen(line); i++) {
        if (line[i] == ch) {
            counter++;
        }
    }
    return counter;
}


static inline int change_dir(str_vec_t * tokens, user_data_t * ud) {
    //here care with cd command with
    //ud.cwd is the current working directory

    if (tokens->size == 1) {
        fprintf(stderr, "fvgsh: cd: falta argumento\n");
        return 1;
    }

    if (tokens->size > 2) {
        fprintf(stderr, "fvgsh: cd: numero excessivo de argumentos\n");
        return 1;
    }

    if (!getcwd(ud->cwd, sizeof ud->cwd))
    {
        int err = 1;
        switch (errno) {
            case EACCES: fprintf(stderr, "fvgsh: permissao negada.\n"); err = 2; break;

            case ELOOP: fprintf(stderr, "fvgsh: Loop de link simbolico encontrado.\n"); err = 3; break;

            case ENAMETOOLONG: fprintf(stderr, "fvgsh: nome do caminho eh longo demais.\n"); err = 4; break;

            case ENOENT: fprintf(stderr, "fvgsh: caminho %s nao existe.\n", tokens->data[1]); err = 5; break;

            case ENOTDIR: fprintf(stderr, "fvgsh: %s nao e um diretorio e nem symlink pra diretorio.\n", tokens->data[1]); err = 6; break;
        default: break;
        }
        return err;
    }


    chdir(tokens->data[1]);

    free(ud->pretty_cwd);
    ud->pretty_cwd = strdup(ud->cwd);
    get_pretty_cwd(ud->pretty_cwd, ud->home_dir);
    printf("home: %s\n", ud->home_dir);
    printf("cwd: %s\n", ud->cwd);
    printf("pcwd: %s\n", ud->pretty_cwd);

    return 0;
}

static inline int exec(str_vec_t * tokens, user_data_t * ud) {

    if (!strcmp(tokens->data[0], "cd")) {
        return change_dir(tokens, ud);
    }


    execvp(tokens->data[0], tokens->data);
    fprintf(stderr, "fvgsh: erro ao executar '%s', código %d.\n", tokens->data[0], errno);
    return errno;
}

int exec_piped_commands(char * line, user_data_t * ud) {
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
            return exec(&tokens, ud);
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

int exec_simple_command(char * line, user_data_t * ud) {

    bool bg_exec = false;
    int i = strlen(line) - 1;
    while(i != 0){
        if(line[i] == '&'){
            line[i] = '\0';
            bg_exec = true;
            break;
        }else if(line[i] != ' ' && line[i] != '\t'){
            bg_exec = false;
            break;
        }
        i--;
    }

    str_vec_t tokens = tokenize(line, " ");

    // only for see if the tokens are correct
    /*for (usize_t i = 0; i < tokens.size; i++) {
        printf("Token[%ld]: %s\n", i, tokens.data[i]);
    }*/

    // Input to execvp must be NULL-terminated
    append_null(&tokens);

    int pid = fork();
    int wait_status;



    if(pid == 0){
        // Runs on the child process
        return exec(&tokens, ud);
    }
    //parent process
    if(bg_exec){
        joblist_insert(&running_jobs, pid, line);
        printf("[%d] - %d\n",running_jobs.size, pid);
    }
    g_waiting_for_child_proc = true;

    int ret_val = 0; // remove this 0 later 
    if(!bg_exec){
        if (waitpid(pid, &wait_status, 0) == -1) {
            fprintf(stderr, "waitpid falhou em exec_simple_command!\n");
            ret_val = 127;
        } else if WIFEXITED(wait_status) {
            ret_val = WEXITSTATUS(wait_status);
        } else {
            fprintf(stderr, "fvgsh: aviso: processo-filho não terminou normalmente!");
            ret_val = 127;
        }
    }

    vec_free(&tokens);
    g_waiting_for_child_proc = false;

    return ret_val;
}

int exec_seq_commands(char * line, user_data_t * ud){
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
        exec_simple_command(commands.data[i], ud);
    }
    
    vec_free(&commands);
    return 0;// success
}

int exec_log_commands(char * line, user_data_t * ud){
    char prev_ch = ' ', *buffer, *rest;
    size_t i, blen = 0;
    bool hasCommandOR = false;
    bool hasCommandAND = false;
    rest = line;
    buffer = malloc(strlen(line)*sizeof(char));
    for (i=0;i<strlen(line);i++){
        if(prev_ch == '|' && line[i] == '|'){
            hasCommandOR = true;
            rest = &line[i+1];
            break;
        }else if(prev_ch == '&' && line[i] == '&'){
            hasCommandAND = true;
            rest = &line[i+1];
            break;
        }else{
            hasCommandOR = false;
            hasCommandAND = false;
            // If we assume that no pipes will be sent, we can 
            // just skip the next '|' or '&' by doing i++, don't we ?
            // ----> I'll let someone braver than me do that
            if (line[i] == '|'){
                prev_ch = '|';
            }else if (line[i]== '&'){
                prev_ch = '&';
            }else{
                buffer[blen++] = line[i];
            }
        }
    }
    buffer[blen] = '\0';
    if(hasCommandOR){
        //printf("COMANDO OR-- BUFF: %s REST: %s\n", buffer, rest);
        
        int buffer_ret = exec_simple_command(buffer, ud);
        free(buffer);
        if(buffer_ret != 0){
            int rest_ret = exec_log_commands(rest, ud);
            return buffer_ret || rest_ret;
        }else{
            return buffer_ret;
        }
    }else if(hasCommandAND){
        //printf("COMANDO AND:-- BUFF: %s REST: %s\n", buffer, rest);
        
        int buffer_ret = exec_simple_command(buffer, ud);
        free(buffer);
        if(buffer_ret != 0){
            return buffer_ret;
        }else{
            int rest_ret = exec_log_commands(rest, ud);
            return buffer_ret && rest_ret;
        }
    }else{
        free(buffer);
        
        return exec_simple_command(line, ud);
    }

}

int restore_command(){
    int wstatus;
    job_t cmd_job = running_jobs.list[running_jobs.size -1];
    printf("%s\n", cmd_job.line);
    waitpid(cmd_job.pid, &wstatus, 0);
    printf("[1]+ Concluído\n");
    if(WIFEXITED(wstatus)){
        joblist_remove(&running_jobs, cmd_job.pid);
        return 0;
    }else{
        return -1;
    }
}
