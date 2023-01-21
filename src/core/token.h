// Created by Ancyr

#ifndef LAINE_TOKEN_H
#define LAINE_TOKEN_H

typedef enum TokenType {
    T_BYTE, T_SHORT, T_INT, T_LONG, T_FLOAT, T_DOUBLE, T_BOOL,

    T_DO, T_WHILE, T_FOR,
    T_IF, T_ELSE, T_ELSEIF,
    T_FUN, T_RETURN,
    T_CLASS, T_EXTENDS, T_ABSTRACT, T_PRIVATE, T_PUBLIC, T_PROTECTED,

    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_MODULO,
    T_PLUS_EQUAL, T_MINUS_EQUAL, T_STAR_EQUAL, T_SLASH_EQUAL, T_MODULO_EQUAL,
    T_INCREMENT, T_DECREMENT,
    T_GREATER, T_GREATER_EQUAL, T_LOWER, T_LOWER_EQUAL,
    T_EQUAL_EQUAL, T_BANG_EQUAL,
    T_BANG,
    T_EQUAL,

    T_PAREN_LEFT, T_PAREN_RIGHT,
    T_BRACKET_LEFT, T_BRACKET_RIGHT,
    T_BRACE_LEFT, T_BRACE_RIGHT,

    T_SEMICOLON, T_DOT,

    T_STRING_LITERAL, T_INTEGER_LITERAL, T_FLOATING_LITERAL,
    T_IDENTIFIER,

    T_EOF,
} TokenType;

typedef struct Token {
    TokenType type;
    unsigned int line;
    unsigned int start;
    unsigned int length;
} Token;

typedef struct TokenList {
  unsigned int size;
  unsigned int capacity;
  Token **tokens;
} TokenList;

TokenList* ln_tokenlist_init();
void ln_tokenlist_grow(TokenList *tokenlist);
void ln_tokenlist_add(TokenList *list, Token *token);
void ln_tokenlist_free(TokenList *tokenlist);

#endif //LAINE_TOKEN_H
