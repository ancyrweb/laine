#include "ast.h"

#include <stdlib.h>

void ln_ast_free(AST_Node *node) {
  switch (node->type) {
    case ASTNODE_VALUE: {
      AST_Value *v = (AST_Value*) node;
      free(v);
    }
    case ASTNODE_BINOP: {
      AST_Binop *v = (AST_Binop*) node;
      free(v);
    }
  }
}