#ifndef LAINE_AST_H
#define LAINE_AST_H

#include "token.h"

typedef enum ASTNodeType {
  ASTNODE_LITERAL,
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
  AST_Node node;
  AST_Value left;
  AST_Value right;
  Token operand;
} AST_Binop;

#endif