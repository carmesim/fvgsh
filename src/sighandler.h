#ifndef SIGHANDLER_H
#define SIGHANDLER_H

#include "types.h" // For bool

void init_signal_handler();

// Signals that the shell must exit
// We use this flag instead of an exit(0) because it allows for freeing up allocated memory.
extern bool g_should_exit;



#endif // SIGHANDLER_H
