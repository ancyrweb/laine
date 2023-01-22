// Created by Ancyr

#include "parser.h"
#include "ast.h"
#include "memory.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

Parser parser;

// allocation
static void add_node(AST_Node *node) {
  if (parser.nodes.size == parser.nodes.capacity) {
    if (parser.nodes.size == 0) {
      parser.nodes.capacity = 8;
      parser.nodes.nodes = ALLOCATE(AST_Node, parser.nodes.capacity);
    } else {
      int next_capacity = parser.nodes.capacity * 2;
      parser.nodes.nodes = ln_reallocate(parser.nodes.nodes, parser.nodes.capacity, next_capacity);
      parser.nodes.capacity = next_capacity;
    }
  }

  parser.nodes.nodes[parser.nodes.size++] = node;
}

// utils
static bool is_eof() {
  return parser.current >= parser.tokens->size;
}

static bool match(TokenType t) {
  return parser.tokens->tokens[parser.current]->type == t;
}

static void syntax_error(const char *error_msg) {
  fprintf(stderr, "Error : %s", error_msg);
  exit(1);
}

static void consume(TokenType t, const char *error_msg) {
  if (!match(t)) {
    syntax_error(error_msg);
    return;
  }

  parser.current++;
}

static Token* advance() {
  return parser.tokens->tokens[parser.current++];
}

static AST_Node* primary() {
  if (match(T_INTEGER_LITERAL)) {
    Token* t = advance();

    char buff[1024];
    memcpy(&buff, parser.tokens->source + t->start, t->length);
    buff[t->length] = '\0'; 

    AST_Value *v = ALLOCATE(AST_Value, 1);
    v->as.int_val = atoi(buff);
    v->node.type = ASTNODE_VALUE;
    v->type = ASTVAL_INT;
    
    return (AST_Node*) v;
  } else {
    syntax_error("Expected primary.");
  }
}

static AST_Node* expression_statement() {
  AST_Node *n = primary();
  consume(T_SEMICOLON, "Expected semi-colon;");

  if (n != NULL) {
    add_node(n);
  }
  return n;
}

// interface
void ln_parser_start(TokenList *tokens) {
  parser.nodes.size = 0;
  parser.nodes.capacity = 0;
  parser.nodes.nodes = NULL;
  parser.tokens = tokens;
  parser.current = 0;

  expression_statement();
  
  ln_debug_ast(parser);
}

void ln_parser_free() {
  for (int i = 0; i < parser.nodes.size; i++) {
    ln_ast_free(parser.nodes.nodes[i]);
  }
}