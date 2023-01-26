#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ln_debug_toktostr(TokenType type) {
  #define T(name) case name: { return #name; }
  switch (type) {
    T(T_BYTE);
    T(T_SHORT);
    T(T_INT);
    T(T_LONG);
    T(T_FLOAT);
    T(T_PLUS);
    T(T_MINUS);
    T(T_STAR);
    T(T_SLASH);
    T(T_MODULO);
    T(T_PLUS_EQUAL);
    T(T_MINUS_EQUAL);
    T(T_STAR_EQUAL);
    T(T_SLASH_EQUAL);
    T(T_MODULO_EQUAL);
    T(T_INCREMENT);
    T(T_DECREMENT);
    T(T_GREATER);
    T(T_GREATER_EQUAL);
    T(T_LOWER);
    T(T_LOWER_EQUAL);
    T(T_EQUAL_EQUAL);
    T(T_BANG_EQUAL);
    T(T_BANG);
    T(T_EQUAL);
    T(T_PAREN_LEFT);
    T(T_PAREN_RIGHT);
    T(T_BRACKET_LEFT);
    T(T_BRACKET_RIGHT);
    T(T_BRACE_LEFT);
    T(T_BRACE_RIGHT);
    T(T_SEMICOLON);
    T(T_DOT);
    T(T_INTEGER_LITERAL)
    T(T_IDENTIFIER)
    T(T_STRING_LITERAL)
    T(T_FLOATING_LITERAL)
    T(T_EOF);
    default: {
      return "T_UNKNOWN";
    }
  }

  #undef T
}

void ln_debug_tokens(TokenList *list) {
  #define T(name) case name: printf("%s", ln_debug_toktostr(name)); printf("\n"); break
  #define TT(name) \
      case name: { \
        memcpy(&buff, list->source + t->start, t->length); \
        buff[t->length] = '\0'; \
        printf(#name); \
        printf(" (%s)\n", buff); \
        break; } \
      

  char buff[1024];

  for (unsigned int i = 0; i < list->size; i++) {
    Token *t = list->tokens[i];
    switch (t->type) {
      T(T_BYTE);
      T(T_SHORT);
      T(T_INT);
      T(T_LONG);
      T(T_FLOAT);
      T(T_PLUS);
      T(T_MINUS);
      T(T_STAR);
      T(T_SLASH);
      T(T_MODULO);
      T(T_PLUS_EQUAL);
      T(T_MINUS_EQUAL);
      T(T_STAR_EQUAL);
      T(T_SLASH_EQUAL);
      T(T_MODULO_EQUAL);
      T(T_INCREMENT);
      T(T_DECREMENT);
      T(T_GREATER);
      T(T_GREATER_EQUAL);
      T(T_LOWER);
      T(T_LOWER_EQUAL);
      T(T_EQUAL_EQUAL);
      T(T_BANG_EQUAL);
      T(T_BANG);
      T(T_EQUAL);
      T(T_PAREN_LEFT);
      T(T_PAREN_RIGHT);
      T(T_BRACKET_LEFT);
      T(T_BRACKET_RIGHT);
      T(T_BRACE_LEFT);
      T(T_BRACE_RIGHT);
      T(T_SEMICOLON);
      T(T_DOT);
      TT(T_INTEGER_LITERAL)
      TT(T_IDENTIFIER)
      TT(T_STRING_LITERAL)
      TT(T_FLOATING_LITERAL)
      T(T_EOF);
      default: {
        printf("T_UNKNOWN\n");
      }
    }
  }

  #undef T
  #undef TT
}

 void ln_debug_expr_node(ASTExprNode *node, unsigned int depth) {
  #define PRINT_BUFF() \
    p += sprintf(p, "%c", '\0'); \
    printf("%s\n", printf_buffer); \
    free(printf_buffer);

  char *printf_buffer = malloc(sizeof(char) * 1024);
  char *p = printf_buffer;

  for (int i = 0; i < depth; i++) {
    p += sprintf(p, "--");
  }
  
  switch (node->type) {
    case AST_EXPR_VALUE: {
      ASTExprValue *value = (ASTExprValue*) node;
      if (value->type == AST_EXPRVAL_INT) {
        p += sprintf(p, "- Integer : %d", value->as.int_val);
      } else if (value->type == AST_EXPRVAL_FLOAT) {
        p += sprintf(p, "- Float : %lf", value->as.float_val);
      } else if (value->type == AST_EXPRVAL_STRING) {
        p += sprintf(p, "- String : %s", value->as.string_val);
      } else if (value->type == AST_EXPRVAL_IDENTIFIER) {
        p += sprintf(p, "- Identifier : %s", value->as.string_val);
      }

      PRINT_BUFF();
      break;
    }
    case AST_EXPR_BINOP: {
      ASTExprBinop *value = (ASTExprBinop*) node;
      ASTExprNode *left = value->left;
      ASTExprNode *right = value->right;

      p += sprintf(p, "- Binop - Operand : %s", ln_debug_toktostr(value->operand->type));
      PRINT_BUFF();

      ln_debug_expr_node(left, depth + 1);
      ln_debug_expr_node(right, depth + 1);
      break;
    }
    case AST_EXPR_POSTFIX: {
      ASTExprPostfixOp *value = (ASTExprPostfixOp*) node;

      p += sprintf(p, "- PostfixOp - Operand : %s", ln_debug_toktostr(value->operand->type));
      PRINT_BUFF();

      ln_debug_expr_node(value->left, depth + 1);
      break;
    }
    case AST_EXPR_PREFIX: {
      ASTExprPrefixOp *value = (ASTExprPrefixOp*) node;
      
      p += sprintf(p, "- PrefixOp - Operand : %s", ln_debug_toktostr(value->operand->type));
      PRINT_BUFF();

      ln_debug_expr_node(value->right, depth + 1);
      break;
    }
    case AST_EXPR_GROUPING: {
      ASTExprGroup *value = (ASTExprGroup*) node;

      p += sprintf(p, "- Grouping");
      PRINT_BUFF();
   
      ln_debug_expr_node(value->expr, depth + 1);
      break;
    }
  }

  #undef PRINT_BUFF
}

 void ln_debug_stmt_node(ASTStatementNode *node, int depth) {
  switch(node->type) {
    case AST_STMT_EXPR: {
      printf("- STATEMENT EXPRESSION\n");
      ASTExprNode *expr = (ASTExprNode*) node->stmt;
      ln_debug_expr_node(expr, 0);
    }
  }
}

void ln_debug_ast(Parser parser) {
  printf("--- AST ---\n");
  for (int i = 0; i < parser.nodes.size; i++) {
    ASTStatementNode *node = parser.nodes.nodes[i];
    ln_debug_stmt_node(node, 0);
  }
}