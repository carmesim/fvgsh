#include "strvec.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

static str_vec_t vec_new() {
    str_vec_t tokens;
    tokens.data = malloc(5 * sizeof (char *));
    if (!tokens.data) {
        fprintf(stderr, "fatal error: malloc failed in vec_new\n");
        exit(1);
    }
    tokens.size = 0;
    tokens.capacity = 5;
    return tokens;
}

void append_null(str_vec_t * vec) {
    if (vec->capacity == vec->size) {
        reserve_more(vec, 1);
    }
    vec->data[vec->size] = NULL;
}

void vec_free(str_vec_t tokens) {
    for (usize_t i = 0; i < tokens.size; i++) {
        free(tokens.data[i]);
    }
    free(tokens.data);
    tokens.capacity = 0;
}

//! reserve_more allocates an additional amount of bytes to a pre-existing str_vec_t
int reserve_more(str_vec_t * tokens, const usize_t additional_strs) {
    tokens->capacity += additional_strs;
    char ** tmp = realloc(tokens->data, tokens->capacity * sizeof (char *));
    if (!tmp) {
        fprintf(stderr, "realloc failed in reserve_more\n");
        tokens->capacity -= additional_strs;
        return 1;
    }
    tokens->data = tmp;
    return 0;
}

int vec_insert(str_vec_t * tokens, const char * token) {
    if (tokens->capacity > tokens->size) {
        tokens->data[tokens->size++] = strdup(token);
        return 0;
    }

    if (reserve_more(tokens, 1)) {
        fprintf(stderr, "reserve_more failed in vec_insert\n");
        return 1;
    }

    tokens->data[tokens->size++] = strdup(token);
    return 0;
}

str_vec_t tokenize(char * line) {

    str_vec_t tokens = vec_new();

    // Context pointer for strtok_r
    char * ctx;

    for (
        char * token = strtok_r(line, " ", &ctx);
        token;
        token = strtok_r(NULL, " ", &ctx)
        ) {
        vec_insert(&tokens, token);
    }

    return tokens;
}
