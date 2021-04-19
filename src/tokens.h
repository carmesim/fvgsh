#ifndef TOKENS_H
#define TOKENS_H

#include "basictypes.h"   //

typedef struct tokens_s tokens_t;

struct tokens_s {
    //! The actual array of tokens that the struct contains
    char ** data;

    //! How much memory was allocated for `data`
    usize_t capacity;

    //! Current number of elements in `data`
    usize_t size;
};

tokens_t parse_tokens(char *);
tokens_t tok_free(tokens_t);

#endif // TOKENS_H
