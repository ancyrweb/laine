#ifndef LAINE_AST_H
#define LAINE_AST_H

#include "token.h"



// Expressions
typedef enum ASTExprType {
  AST_EXPR_VALUE,
  AST_EXPR_BINOP,
  AST_EXPR_PREFIX,
  AST_EXPR_POSTFIX,
  AST_EXPR_GROUPING
} ASTExprType;

typedef enum ASTExprValueType {
  AST_EXPRVAL_INT,
  AST_EXPRVAL_FLOAT,
  AST_EXPRVAL_STRING,
  AST_EXPRVAL_IDENTIFIER,
} ASTExprValueType;

typedef struct {
  ASTExprType type;
} ASTExprNode;

typedef struct {
  ASTExprNode node;
  ASTExprValueType type;
  union {
    int int_val; 
    double float_val;
    char *string_val;
  } as;
} ASTExprValue;

typedef struct {
  ASTExprNode node; // parent
  ASTExprNode *left; // can be any of sub nodes
  ASTExprNode *right; // can be any of subnodes too
  Token *operand;
} ASTExprBinop;

typedef struct {
  ASTExprNode node;
  ASTExprNode *expr;
} ASTExprGroup;

typedef struct {
  ASTExprNode node; // parent
  ASTExprNode *right; // can be any of subnodes too
  Token *operand;
} ASTExprPrefixOp;

typedef struct {
  ASTExprNode node; // parent
  ASTExprNode *left; // can be any of subnodes too
  Token *operand;
} ASTExprPostfixOp;


typedef enum ASTStatementType {
  AST_STMT_EXPR,
  AST_VARIABLE_DEFINITION
} ASTStatementType;

typedef struct ASTStatementNode {
  ASTStatementType type;
  void *stmt;
} ASTStatementNode;

typedef struct ASTVariableDefinitionNode {
  Token *type;
  Token *identifier;
  ASTExprNode *expr;
} ASTVariableDefinitionNode;

void ln_ast_free(ASTStatementNode *node);
void ln_ast_free_expr(ASTExprNode *node);

#endif