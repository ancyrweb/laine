#ifndef LAINE_AST_H
#define LAINE_AST_H

#include "token.h"

typedef enum ASTNodeType {
  ASTNODE_VALUE,
  ASTNODE_BINOP,
} ASTNodeType;

typedef enum ASTValueType {
  ASTVAL_INT,
} ASTValueType;

typedef struct {
  ASTNodeType type;
} AST_Node;

typedef struct {
  AST_Node node;
  ASTValueType type;
  union {
    int int_val; 
  } as;
} AST_Value;

typedef struct {
  AST_Node node; // parent
  AST_Node *left; // can be any of sub nodes
  AST_Node *right; // can be any of subnodes too
  Token *operand;
} AST_Binop;

void ln_ast_free(AST_Node *node);

#endif