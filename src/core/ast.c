#include "ast.h"

#include <stdlib.h>

void ln_ast_free(ASTStatementNode *node) {
  switch(node->type) {
    case AST_STMT_EXPR: {
      ASTExprNode *expr = (ASTExprNode*) node->stmt;
      ln_ast_free_expr(expr);
      free(node);
      break;
    }
  }
}

void ln_ast_free_expr(ASTExprNode *node) {
  switch (node->type) {
    case AST_EXPR_VALUE: {
      ASTExprValue *v = (ASTExprValue*) node;
      if (v->type == AST_EXPRVAL_STRING || v->type == AST_EXPRVAL_IDENTIFIER) {
        free(v->as.string_val);
      }

      free(v);
      break;
    }
    case AST_EXPR_BINOP: {
      ASTExprBinop *v = (ASTExprBinop*) node;
      ln_ast_free_expr(v->left);
      ln_ast_free_expr(v->right);
      free(v);
      break;
    }
    case AST_EXPR_POSTFIX: {
      ASTExprPostfixOp *v = (ASTExprPostfixOp*) node;
      ln_ast_free_expr(v->left);
      free(v);
    }
    case AST_EXPR_PREFIX: {
      ASTExprPrefixOp *v = (ASTExprPrefixOp*) node;
      ln_ast_free_expr(v->right);
      free(v);
    }
    default: {
      break;
    }
  }
}