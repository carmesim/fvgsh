#include "userdata.h"

#include "strutils.h" // For get_pretty_cwd
#include <signal.h>   // For struct sigaction, sigemptyset, SIGINT, SIGHUP
#include <stdio.h>    // For fprintf, stderr
#include <string.h>   // For strdup
#include <aio.h>      // For uid_t
#include <unistd.h>   // For gethostname
#include <pwd.h>      // For struct passwd

user_data_t get_user_data() {
    user_data_t ud;
    if (!getcwd(ud.cwd, sizeof ud.cwd))
    {
        fprintf(stderr,  "error: could not get current working directory.\n");
    }

    //! Gets the effective ID of the user that started the shell
    uid_t uid = geteuid();
    struct passwd pwent;
    struct passwd *pwent_ptr;
    char buffer[1024];
    //! Looks for the UDI on the password databank and saves the result on pwent
    getpwuid_r(uid, &pwent, buffer, sizeof buffer, &pwent_ptr);
    ud.username = strdup(pwent.pw_name);
    //! Saves home directory
    ud.home_dir = strdup(pwent.pw_dir);
    printf("HOMEDIR = %s\n", ud.home_dir);
    gethostname(ud.hostname, sizeof ud.hostname);

    // pretty_cwd must be freed before exit.
    ud.pretty_cwd = strdup(ud.cwd);

    get_pretty_cwd(ud.pretty_cwd, ud.home_dir);
    return ud;
}
