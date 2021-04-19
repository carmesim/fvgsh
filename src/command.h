#ifndef COMMAND_H
#define COMMAND_H

#include "basictypes.h" // For bool

typedef struct command_s command_t;
typedef struct fd_s fd_t;
typedef struct output_s output_t;

// Um 'file descriptor' abstrato, pode conter um número, como 0 (representando stdin)
// ou um arquivo
//! An abstract
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
    //! True when >> is used
    //! False otherwise
    bool should_append;
};

struct command_s {
    // Caminho pro binário, ex.: /bin/ls
    char * bin_path;
    // Argumentos passados, como ["-li", NUL]
    char ** args;
    fd_t input;
    fd_t output;
};


#endif // COMMAND_H
