#include "sighandler.h"
#include "types.h"

#include <signal.h>   // For struct sigaction, sigemptyset, SIGINT, SIGHUP
#include <stdio.h>    // for fprintf, stderr

struct sigaction signal_action;

// Extern variable defined in sighandler.h
bool g_should_exit = false;

void signal_handler(int sig)
{
    if (sig==SIGINT || sig==SIGHUP)
    {
        fprintf(stderr, "\nSIGINT (Ctrl+C) received. Code %d. Exiting.", sig);
        g_should_exit = true;
    }

    if (sig==SIGHUP)
    {
        fprintf(stderr, "\nSIGHUP received. Code %d. Exiting.", sig);
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
