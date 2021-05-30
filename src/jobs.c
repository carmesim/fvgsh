#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include "jobs.h"

void joblist_init(jobList *jl){
    jl->size = 0;
}
void joblist_insert(jobList *jl, int pid, char* line){
    job_t *job = malloc(1*sizeof(job_t));
    job->pid = pid;
    job->line = strdup(line);
    jl->list[jl->size] = *job;
    jl->size ++;
}
void joblist_remove(jobList *jl, int pid){
    int i, found = 0;
    for(i = 0; i < jl->size; i++){
        if(jl->list[i].pid == pid){
            found = 1;   
        }
        if(found == 1 && i < jl->size-1){
            jl->list[i] = jl->list[i+1];
        }
    }
    if(found == 0){
        printf("fvgsh: não há trabalho a ser removido com pid %d\n", pid);
    }else{
        jl->size --;
    }
}

void joblist_verify(jobList *jl){
    int i;
    for (i = 0; i < jl->size; i++){
        int status;
        int ret = waitpid(jl->list[i].pid,&status, WNOHANG);
        if(ret == -1 ){
            printf("[%d] FINALIZADO %s\n", jl->list[i].pid, jl->list[i].line);
            joblist_remove(jl,jl->list[i].pid);
        }
    }
}

void joblist_print(jobList *jl){
    int i;
    for (i = 0; i < jl->size; i++){
        printf("[%d] EXECUTANDO %s\n", jl->list[i].pid, jl->list[i].line);
    }
}



