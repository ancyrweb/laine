// Created by Ancyr

#include "parser.h"
#include "ast.h"
#include "memory.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

Parser parser;

// allocation
void add_node(AST_Node *node) {
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

// interface
void ln_parser_start(TokenList *tokens) {
  parser.nodes.size = 0;
  parser.nodes.capacity = 0;
  parser.nodes.nodes = NULL;

  AST_Value *v = ALLOCATE(AST_Value, 1);
  v->as.int_val = 10;
  v->node.type = ASTNODE_LITERAL;
  v->type = ASTVAL_INT;
  
  add_node((AST_Node*) v);
  ln_debug_ast(parser);
}

void ln_parser_free() {
  for (int i = 0; i < parser.nodes.size; i++) {
    free(parser.nodes.nodes[i]);
  }
}