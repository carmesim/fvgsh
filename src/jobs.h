#ifndef JOBS_H
#define JOBS_H
#define MAX_JOBS 10

typedef struct job_s{
    int pid;
    char * line;
}job_t;

typedef struct jobList{
    job_t list[MAX_JOBS];
    int size;
}jobList;

void joblist_init(jobList *jl);
void joblist_insert(jobList *jl, int pid, char* line);
void joblist_remove(jobList *jl, int pid);
void joblist_verify(jobList *jl);
void joblist_print(jobList *jl);

#endif // EXEC_H
