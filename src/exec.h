#ifndef EXEC_H
#define EXEC_H

int exec_simple_command(char * line);
int exec_piped_commands(char * line);
int exec_seq_commands(char * line);
int exec_log_commands(char * line);
#endif // EXEC_H
