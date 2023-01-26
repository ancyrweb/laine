// Created by Ancyr

#include "parser.h"
#include "ast.h"
#include "memory.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define THROW_IF_NULL(value) \
  if (value == NULL) { \
    return NULL; \
  }
  
Parser parser;

// headers
static ASTExprNode* expr();

// allocation
static void add_node(ASTStatementNode *node) {
  if (parser.nodes.size == parser.nodes.capacity) {
    if (parser.nodes.size == 0) {
      parser.nodes.capacity = 8;
      parser.nodes.nodes = ALLOCATE(ASTStatementNode, parser.nodes.capacity);
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

static TokenType current_type() {
  return parser.tokens->tokens[parser.current]->type;
}

static void syntax_error(const char *error_msg) {
  Token *curr = parser.tokens->tokens[parser.current];

  fprintf(stderr, "Syntax error : %s\n", error_msg);
  fprintf(stderr, "At line %d, (%d)\n", curr->line, curr->start);

  parser.is_panic = true;
}

static bool consume(TokenType t, const char *error_msg) {
  if (!match(t)) {
    syntax_error(error_msg);
    return false;
  }

  parser.current++;
  return true;
}

static Token* advance() {
  return parser.tokens->tokens[parser.current++];
}

static Token* expect(TokenType type) {
  if (!match(type)) {
    char buff[256];
    sprintf(
      buff, 
      "Expected %s but got %s", 
      ln_debug_toktostr(type), 
      ln_debug_toktostr(current_type())
    );

    syntax_error(buff);
    return NULL;
  }

  return advance();
}

static void synchronize_statement() {
  while(!is_eof() && !match(T_EOF) && !match(T_SEMICOLON)) {
    advance();
  }

  if (match(T_SEMICOLON)) {
    advance();
  }

  parser.is_panic = false;
}

// recursive descent parser

static ASTExprBinop* alloc_binop(ASTExprNode *left, ASTExprNode *right, Token *operand) {
    ASTExprBinop *binop = ALLOCATE(ASTExprBinop, 1);
    binop->node.type = AST_EXPR_BINOP;
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
static ASTExprNode* primary() {
  if (match(T_INTEGER_LITERAL) || match(T_IDENTIFIER) || match(T_FLOATING_LITERAL) || match(T_STRING_LITERAL)) {
    Token* t = advance();

    ASTExprValue *v = ALLOCATE(ASTExprValue, 1);
    v->node.type = AST_EXPR_VALUE;

    ASTExprNode *output = (ASTExprNode*) v;

    switch (t->type) {
      case T_INTEGER_LITERAL: {
        char buff[t->length + 1];
        memcpy(&buff, parser.tokens->source + t->start, t->length);
        buff[t->length] = '\0'; 

        v->as.int_val = atoi(buff);
        v->type = AST_EXPRVAL_INT;
        break;
      }
      case T_FLOATING_LITERAL: {
        char buff[t->length + 1];
        memcpy(&buff, parser.tokens->source + t->start, t->length);
        buff[t->length] = '\0'; 

        v->as.float_val = atof(buff);
        v->type = AST_EXPRVAL_FLOAT;
        break;
      }
      case T_STRING_LITERAL: {
        v->as.string_val = extract_string_literal(t);
        v->type = AST_EXPRVAL_STRING;
        break;
      }
      case T_IDENTIFIER: {
        v->as.string_val = extract_lexeme(t);
        v->type = AST_EXPRVAL_IDENTIFIER;

        if (match(T_INCREMENT) || match(T_DECREMENT)) {
          Token *postfix = advance();
          ASTExprPostfixOp *postfix_node = ALLOCATE(ASTExprPostfixOp, 1);
          postfix_node->node.type = AST_EXPR_POSTFIX;
          postfix_node->left = (ASTExprNode*) v;
          postfix_node->operand = postfix;

          output = (ASTExprNode*) postfix;
        }

        break;
      }
      default: {
        free(v);
        syntax_error("Expected type.");
        return NULL;
        break;
      }
    }
       
    return output;
  } else {
    syntax_error("Expected primary.");
    return NULL;
  }
}

static ASTExprNode* prefixUnary() {
  if (match(T_MINUS) || match(T_INCREMENT) || match(T_DECREMENT) || match(T_BANG)) {
    Token *operand = advance();
    ASTExprNode *right = primary();
    
    THROW_IF_NULL(right)

    ASTExprPrefixOp *v = ALLOCATE(ASTExprPrefixOp, 1);
    v->node.type = AST_EXPR_PREFIX;
    v->operand = operand;
    v->right = right;
    return (ASTExprNode*) v;
  }

  return primary();
}

static ASTExprNode* grouping() {
  if (match(T_PAREN_LEFT)) {
    advance();
    ASTExprNode *e = expr();

    THROW_IF_NULL(e)

    if (!consume(T_PAREN_RIGHT, "Expected right parenthesis.")) {
      ln_ast_free_expr(e);
      return NULL;
    }

    ASTExprGroup *v = ALLOCATE(ASTExprGroup, 1);
    v->node.type = AST_EXPR_GROUPING;
    v->expr = e;
    return (ASTExprNode*) v;
  }

  return prefixUnary();
}



static ASTExprNode* factor_expr() {
  ASTExprNode *left = grouping();
  THROW_IF_NULL(left)

  while (match(T_STAR) || match(T_SLASH) || match(T_MODULO)) {
    Token *operand = advance();
    ASTExprNode *right = grouping();

    if (right == NULL) {
      ln_ast_free_expr(left);
      return NULL;
    }

    left = (ASTExprNode*) alloc_binop(left, right, operand);
  }

  return left;
}

static ASTExprNode* terms_expr() {
  ASTExprNode *left = factor_expr();
  while (match(T_PLUS) || match(T_MINUS)) {
    Token *operand = advance();
    ASTExprNode *right = factor_expr();

    if (right == NULL) {
      ln_ast_free_expr(left);
      return NULL;
    }

    left = (ASTExprNode*) alloc_binop(left, right, operand);
  }

  return left;
}

static ASTExprNode* comparison_expr() {
  ASTExprNode *left = terms_expr();
  while (match(T_LOWER) || match(T_LOWER_EQUAL) || match(T_GREATER) || match(T_GREATER_EQUAL)) {
    Token *operand = advance();
    ASTExprNode *right = terms_expr();

    if (right == NULL) {
      ln_ast_free_expr(left);
      return NULL;
    }

    left = (ASTExprNode*) alloc_binop(left, right, operand);
  }

  return left;
}

static ASTExprNode* equality_expr() {
  ASTExprNode *left = comparison_expr();
  while (match(T_EQUAL_EQUAL) || match(T_BANG_EQUAL)) {
    Token *operand = advance();
    ASTExprNode *right = comparison_expr();

    if (right == NULL) {
      ln_ast_free_expr(left);
      return NULL;
    }

    left = (ASTExprNode*) alloc_binop(left, right, operand);
  }

  return left;
}

static ASTExprNode* and_expr() {
  ASTExprNode *left = equality_expr();
  while (match(T_AND)) {
    Token *operand = advance();
    ASTExprNode *right = equality_expr();

    if (right == NULL) {
      ln_ast_free_expr(left);
      return NULL;
    }

    left = (ASTExprNode*) alloc_binop(left, right, operand);
  }

  return left;
}

static ASTExprNode* or_expr() {
  ASTExprNode *left = and_expr();
  while (match(T_OR)) {
    Token *operand = advance();
    ASTExprNode *right = and_expr();

    if (right == NULL) {
      ln_ast_free_expr(left);
      return NULL;
    }

    left = (ASTExprNode*) alloc_binop(left, right, operand);
  }

  return left;
}

static ASTExprNode* expr() {
  return or_expr();
}

static ASTStatementNode* expression_statement() {
  ASTExprNode *n = expr();

  if (parser.is_panic) {
    synchronize_statement();
    return NULL;
  }

  if(!consume(T_SEMICOLON, "Expected semi-colon")) {
    synchronize_statement();
    return NULL;
  }

  ASTStatementNode *stmt = ALLOCATE(ASTStatementNode, 1);
  stmt->type = AST_STMT_EXPR;
  stmt->stmt = n;
  add_node(stmt);
  
  return stmt;
}

static ASTStatementNode *variable_definition() {
  Token *type = advance();
  Token *identifier = expect(T_IDENTIFIER);

  if (parser.is_panic) {
    synchronize_statement();
    return NULL;
  }

  ASTVariableDefinitionNode *n = ALLOCATE(ASTVariableDefinitionNode, 1);
  n->type = type;
  n->identifier = identifier;

  if (match(T_EQUAL)) {
    advance();
    ASTExprNode *expr_node = expr();
    n->expr = expr_node;
  }

  consume(T_SEMICOLON, "Expected semicolon.");

  ASTStatementNode *stmt = ALLOCATE(ASTStatementNode, 1);
  stmt->type = AST_VARIABLE_DEFINITION;
  stmt->stmt = n;
  add_node(stmt);
  
  return stmt;
}

// interface
void ln_parser_start(TokenList *tokens) {
  parser.nodes.size = 0;
  parser.nodes.capacity = 0;
  parser.nodes.nodes = NULL;
  parser.tokens = tokens;
  parser.current = 0;
  parser.is_panic = false;

  for (;;) {
    ASTStatementNode *node;
    if (match(T_INT) || match(T_FLOAT) || match(T_BOOL)) {
      node = variable_definition();
    } else {
      node = expression_statement();
    }

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