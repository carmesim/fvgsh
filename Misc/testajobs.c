#include <stdio.h>
#include "jobs.h"

int main(int argc, char** argv){

    printf("bom dia!\n");
    jobList jl;
    joblist_init(&jl);
    joblist_insert(&jl, 64, "primeira");
    joblist_insert(&jl, 128, "segundaa");
    joblist_insert(&jl, 256, "terceira");
    joblist_remove(&jl, 128);
    joblist_print(&jl);
    return 0;
}
