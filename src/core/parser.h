// Created by Ancyr

#ifndef LAINE_PARSER_H
#define LAINE_PARSER_H

#include "token.h"
#include "ast.h"

typedef struct {
  int size;
  int capacity;
  AST_Node **nodes;
} ParserNodes;

typedef struct {
  struct {
    int size;
    int capacity;
    AST_Node **nodes;
  } nodes;
} Parser;

void ln_parser_start(TokenList *tokens);
void ln_parser_free();

#endif //LAINE_PARSER_H
