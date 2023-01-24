#include "ast.h"

#include <stdlib.h>

void ln_ast_free(AST_Node *node) {
  switch (node->type) {
    case ASTNODE_VALUE: {
      AST_Value *v = (AST_Value*) node;
      free(v);
      break;
    }
    case ASTNODE_BINOP: {
      AST_Binop *v = (AST_Binop*) node;
      ln_ast_free(v->left);
      ln_ast_free(v->right);
      free(v);
      break;
    }
    case ASTNODE_POSTFIX: {
      AST_PostfixOp *v = (AST_PostfixOp*) node;
      ln_ast_free(v->left);
      free(v);
    }
    case ASTNODE_PREFIX: {
      AST_PrefixOp *v = (AST_PrefixOp*) node;
      ln_ast_free(v->right);
      free(v);
    }
    default: {
      break;
    }
  }
}