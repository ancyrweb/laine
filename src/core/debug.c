#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ln_debug_tokens(const char *source, TokenList *list) {
  #define T(name) case name: printf(#name); printf("\n"); break
  #define TT(name) \
      case name: { \
        memcpy(&buff, source + t->start, t->length); \
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
      T(T_DOUBLE);
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

void ln_debug_ast(Parser parser) {
  for (int i = 0; i < parser.nodes.size; i++) {
    AST_Node *node = parser.nodes.nodes[i];
    switch (node->type) {
      case ASTNODE_LITERAL: {
        AST_Value *value = (AST_Value*) node;
        printf("AST Value : %d\n", value->as.int_val);
        break;
      }
    }
  }
}