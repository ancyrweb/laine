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

// headers
static AST_Node* expr();

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

static AST_Binop* alloc_binop(AST_Node *left, AST_Node *right, Token *operand) {
    AST_Binop *binop = ALLOCATE(AST_Binop, 1);
    binop->node.type = ASTNODE_BINOP;
    binop->left = left;
    binop->right = right;
    binop->operand = operand;
    return binop;
}

static char* extract_string_literal(Token* t) {
  int size = t->length - 2; // take the " out
  char *buff = malloc(sizeof(char) * size + 1); // leave space for \0
  const char *p = parser.tokens->source + t->start + 1;

  strncpy(buff, p, size);
  buff[size] = '\0';

  return buff;
}

static char* extract_lexeme(Token* t) {
  int size = t->length; // take the " out
  char *buff = malloc(sizeof(char) * size + 1); // leave space for \0
  const char *p = parser.tokens->source + t->start;

  strncpy(buff, p, size);
  buff[size] = '\0';

  return buff;
}

// parsing
static AST_Node* primary() {
  if (match(T_INTEGER_LITERAL) || match(T_IDENTIFIER) || match(T_FLOATING_LITERAL) || match(T_STRING_LITERAL)) {
    Token* t = advance();

    AST_Value *v = ALLOCATE(AST_Value, 1);
    v->node.type = ASTNODE_VALUE;

    AST_Node *output = (AST_Node*) v;

    switch (t->type) {
      case T_INTEGER_LITERAL: {
        char buff[t->length + 1];
        memcpy(&buff, parser.tokens->source + t->start, t->length);
        buff[t->length] = '\0'; 

        v->as.int_val = atoi(buff);
        v->type = ASTVAL_INT;
        break;
      }
      case T_FLOATING_LITERAL: {
        char buff[t->length + 1];
        memcpy(&buff, parser.tokens->source + t->start, t->length);
        buff[t->length] = '\0'; 

        v->as.float_val = atof(buff);
        v->type = ASTVAL_FLOAT;
        break;
      }
      case T_STRING_LITERAL: {
        v->as.string_val = extract_string_literal(t);
        v->type = ASTVAL_STRING;
        break;
      }
      case T_IDENTIFIER: {
        v->as.string_val = extract_lexeme(t);
        v->type = ASTVAL_IDENTIFIER;

        if (match(T_INCREMENT) || match(T_DECREMENT)) {
          Token *postfix = advance();
          AST_PostfixOp *postfix_node = ALLOCATE(AST_PostfixOp, 1);
          postfix_node->node.type = ASTNODE_POSTFIX;
          postfix_node->left = (AST_Node*) v;
          postfix_node->operand = postfix;

          output = (AST_Node*) postfix;
        }

        break;
      }
      default: {
        syntax_error("Expected type.");
        break;
      }
    }
       
    return output;
  } else {
    syntax_error("Expected primary.");
  }
}

static AST_Node* prefixUnary() {
  if (match(T_MINUS) || match(T_INCREMENT) || match(T_DECREMENT) || match(T_BANG)) {
    Token *operand = advance();
    AST_Node *right = primary();
    

    AST_PrefixOp *v = ALLOCATE(AST_PrefixOp, 1);
    v->node.type = ASTNODE_PREFIX;
    v->operand = operand;
    v->right = right;
    return (AST_Node*) v;
  }

  return primary();
}

static AST_Node* grouping() {
  if (match(T_PAREN_LEFT)) {
    advance();
    AST_Node *e = expr();
    consume(T_PAREN_RIGHT, "Expected right parenthesis.");

    AST_Grouping *v = ALLOCATE(AST_Grouping, 1);
    v->node.type = ASTNODE_GROUPING;
    v->expr = e;
    return (AST_Node*) v;
  }

  return prefixUnary();
}

static AST_Node* factor_expr() {
  AST_Node *left = grouping();
  while (match(T_STAR) || match(T_SLASH) || match(T_MODULO)) {
    Token *operand = advance();
    AST_Node *right = grouping();
    left = (AST_Node*) alloc_binop(left, right, operand);
  }

  return left;
}

static AST_Node* terms_expr() {
  AST_Node *left = factor_expr();
  while (match(T_PLUS) || match(T_MINUS)) {
    Token *operand = advance();
    AST_Node *right = factor_expr();
    left = (AST_Node*) alloc_binop(left, right, operand);
  }

  return left;
}

static AST_Node* comparison_expr() {
  AST_Node *left = terms_expr();
  while (match(T_LOWER) || match(T_LOWER_EQUAL) || match(T_GREATER) || match(T_GREATER_EQUAL)) {
    Token *operand = advance();
    AST_Node *right = terms_expr();
    left = (AST_Node*) alloc_binop(left, right, operand);
  }

  return left;
}

static AST_Node* equality_expr() {
  AST_Node *left = comparison_expr();
  while (match(T_EQUAL_EQUAL) || match(T_BANG_EQUAL)) {
    Token *operand = advance();
    AST_Node *right = comparison_expr();
    left = (AST_Node*) alloc_binop(left, right, operand);
  }

  return left;
}

static AST_Node* and_expr() {
  AST_Node *left = equality_expr();
  while (match(T_AND)) {
    Token *operand = advance();
    AST_Node *right = equality_expr();
    left = (AST_Node*) alloc_binop(left, right, operand);
  }

  return left;
}

static AST_Node* or_expr() {
  AST_Node *left = and_expr();
  while (match(T_OR)) {
    Token *operand = advance();
    AST_Node *right = and_expr();
    left = (AST_Node*) alloc_binop(left, right, operand);
  }

  return left;
}

static AST_Node* expr() {
  return or_expr();
}

static AST_Node* expression_statement() {
  AST_Node *n = expr();
  consume(T_SEMICOLON, "Expected semi-colon");

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

  for (;;) {
    expression_statement();
    if (match(T_EOF)) {
      advance();
      break;
    }
  }
  
  ln_debug_ast(parser);
}

void ln_parser_free() {
  for (int i = 0; i < parser.nodes.size; i++) {
    ln_ast_free(parser.nodes.nodes[i]);
  }
}