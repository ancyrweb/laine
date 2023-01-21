// Created by Ancyr
#include "token.h"
#include "memory.h"

#include <stdlib.h>
#include <stdio.h>

#define TOKEN_LIST_DEFAULT_SIZE 32

TokenList* ln_tokenlist_init() {
    TokenList *t = ALLOCATE(TokenList, 1);
    t->size = 0;
    t->capacity = TOKEN_LIST_DEFAULT_SIZE;
    t->tokens = ALLOCATE(Token, TOKEN_LIST_DEFAULT_SIZE);
    return t;
}

void ln_tokenlist_grow(TokenList *t) {
    unsigned int next_capacity = t->capacity * 2;
    t->tokens = ln_reallocate(t->tokens, t->capacity, next_capacity);
    t->capacity = next_capacity;
}

void ln_tokenlist_add(TokenList* list, Token *token) {
    if (list->size == list->capacity) {
        ln_tokenlist_grow(list);
    }

    list->tokens[list->size] = token;
    list->size++;
}

void ln_tokenlist_free(TokenList *t) {
    for (int i = 0; i < t->size; i++) {
        free(t->tokens[i]);
    }

    free(t->tokens);
    free(t);
}