#ifndef TOKENS_H
#define TOKENS_H

#include "basictypes.h"   //

typedef struct str_vec_s str_vec_t;

struct str_vec_s {
    //! The actual array of tokens that the struct contains
    char ** data;

    //! How much memory was allocated for `data`
    usize_t capacity;

    //! Current number of elements in `data`
    usize_t size;
};

str_vec_t tokenize(char *, const char *);
void vec_free(str_vec_t *);
void append_null(str_vec_t *);
int reserve_more(str_vec_t *, const usize_t);

#endif // TOKENS_H
