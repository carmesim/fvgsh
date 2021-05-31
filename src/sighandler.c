#include "sighandler.h"
#include "basictypes.h"

#include <signal.h>   // For struct sigaction, sigemptyset, SIGINT, SIGHUP
#include <stdio.h>    // for fprintf, stderr

struct sigaction signal_action;

// Extern variables defined in sighandler.h
bool g_should_exit = false;
bool g_waiting_for_child_proc = false;
bool g_fg_sighand = false;

void signal_handler(int sig)
{
    if (g_waiting_for_child_proc || g_fg_sighand) {
        return;
    }
    if (sig==SIGINT)
    {
        fprintf(stderr, "\nSIGINT (Ctrl+C) received. Code %d. Exiting.\n", sig);
        g_should_exit = true;
    }
}


void init_signal_handler()
{
    signal_action.sa_handler = signal_handler;
    sigemptyset(&signal_action.sa_mask);
    signal_action.sa_flags = 0;
    sigaction(SIGINT, &signal_action, NULL);
}
