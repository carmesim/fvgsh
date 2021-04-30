#ifndef SIGHANDLER_H
#define SIGHANDLER_H

#include "basictypes.h" // For bool

void init_signal_handler();

//! Signals that the shell must exit
//! We use this flag instead of an exit(0) because it allows for freeing up allocated memory.
extern bool g_should_exit;

//! Signals that the shell is waiting for a child process to finish
extern bool g_waiting_for_child_proc;



#endif // SIGHANDLER_H
