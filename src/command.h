#ifndef COMMAND_H
#define COMMAND_H

#include "basictypes.h" // For bool

typedef struct command_s command_t;
typedef struct fd_s fd_t;
typedef struct output_s output_t;
typedef enum command_type command_type_t;

//! An abstract "file descriptor"
//! May represent a Unix file descriptor or a file path
struct fd_s {
    //! The kind of data this struct will hold
    enum kind{Stdin, Stdout, Filename} kind;
    union {
        //! Represents the Unix file descriptor used
        //! Only used when kind == Stdin or kind == Stdout
        int fd;
        //! Represents the file from which stdin or stdout should be piped to/from.
        //! Only used when kind == Stdin or kind == Stdout
        char * file_path;
    } data;

    //! When saving to a file, `should_append` defines if the contents should be appended to the file
    //! (if it exists) of the existing files should be discarted
    //!
    //! True when >> is used, false otherwise
    bool should_append;
};

struct command_s {
    //! The binary to be executed
    char * bin;

    //! NULL-terminated sequence of arguments to the specified binary
    char ** args;

    //! What the binary will read from
    fd_t input;

    //! What the binary will write to
    fd_t output;
};

enum command_type {
    Piped,
    Logical,
    Sequential,
    Regular
};

command_type_t parse_command_type(const char *);

#endif // COMMAND_H
