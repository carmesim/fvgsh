#include "tokens.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

static tokens_t tok_new() {
    tokens_t tokens;
    tokens.data = malloc(5 * sizeof (char *));
    tokens.size = 0;
    tokens.capacity = 5;
    return tokens;
}

tokens_t tok_free(tokens_t tokens) {
    for (usize_t i = 0; i < tokens.size; i++) {
        free(tokens.data[i]);
    }
    free(tokens.data);
    tokens.capacity = 0;
    return tokens;
}

int tok_insert(tokens_t * tokens, const char * token) {
    if (tokens->capacity > tokens->size) {
        tokens->data[tokens->size++] = strdup(token);
        return 0;
    }

    tokens->capacity += sizeof (char *);
    char ** tmp = realloc(tokens->data, tokens->capacity);
    if (!tmp) {
        fprintf(stderr, "realloc failed in tok_insert");
        tokens->capacity -= sizeof (char *);
        return 1;
    }
    tokens->data = tmp;
    tokens->data[tokens->size++] = strdup(token);
    return 0;
}

tokens_t parse_tokens(char * line) {

    tokens_t tokens = tok_new();

    // Context pointer for strtok_r
    char * ctx;

    char * token = strtok_r(line, " ", &ctx);
    tok_insert(&tokens, token);

    while ((token = strtok_r(NULL, " ", &ctx)))
    {
        tok_insert(&tokens, token);
    }

    return tokens;
}
