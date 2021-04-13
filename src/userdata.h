#ifndef USERDATA_H
#define USERDATA_H

#include "types.h" // For PATH_MAX

struct user_data_s {
    //! User's... username
    char * username;
    //! User's hostname
    char hostname[64];
    //! Current working directory
    char cwd[PATH_MAX];
    //! User's $HOME directory
    char * home_dir;
    //! Same as cwd, but with $HOME replaced by '~'.
    char * pretty_cwd;
};

typedef struct user_data_s user_data_t;

user_data_t get_user_data();

#endif // USERDATA_H
