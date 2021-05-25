#ifndef EXEC_H
#define EXEC_H

#include "jobs.h" // For jobList, job_t

extern jobList running_jobs;

int exec_simple_command(char * line);
int exec_piped_commands(char * line);
int exec_seq_commands(char * line);
int exec_log_commands(char * line);
int restore_command(job_t cmd_job);
#endif // EXEC_H
