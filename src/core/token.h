// Created by Ancyr

#ifndef LAINE_TOKEN_H
#define LAINE_TOKEN_H

typedef enum TokenType {
    BYTE, SHORT, INT, LONG, FLOAT, DOUBLE, BOOL,

    DO, WHILE, FOR,
    IF, ELSE, ELSEIF,
    FUN, RETURN,
    CLASS, EXTENDS, ABSTRACT, PRIVATE, PUBLIC, PROTECTED,

    PLUS, MINUS, STAR, SLASH, MODULO,
    GREATER, GREATER_EQUAL, LOWER, LOWER_EQUAL,
    EQUAL_EQUAL, BANG_EQUAL,
    BANG,

    PAREN_LEFT, PAREN_RIGHT,
    BRACKET_LEFT, BRACKET_RIGHT,
    BRACE_LEFT, BRACE_RIGHT,

    SEMICOLON,

    STRING_LITERAL, INTEGER_LITERAL, FLOATING_LITERAL, BOOLEAN_LITERAL,
    IDENTIFIER
} TokenType;

typedef struct Token {
    TokenType type;
    unsigned int line;
    unsigned int start;
    unsigned int length;
    char* lexeme;
} Token;

typedef struct TokenList {
  Token *tokens;
  unsigned int size;
  unsigned int capacity;
} TokenList;

TokenList* ln_tokenlist_init();
void ln_tokenlist_grow(TokenList *tokenlist);
void ln_tokenlist_free(TokenList *tokenlist);

#endif //LAINE_TOKEN_H
