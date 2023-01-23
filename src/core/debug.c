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

static void debug_node(AST_Node *node, unsigned int depth) {
  switch (node->type) {
    case ASTNODE_VALUE: {
      AST_Value *value = (AST_Value*) node;
      printf("AST Value : %d\n", value->as.int_val);
      break;
    }
    case ASTNODE_BINOP: {
      AST_Binop *value = (AST_Binop*) node;
      AST_Node *left = value->left;
      AST_Node *right = value->right;

      char buff[1024];
      sprintf(
        &buff, 
        "(%d) | AST Binop\n      Operand : %s\n", 
        depth, 
        ln_debug_toktostr(value->operand->type)
      );
     
      printf("%s", buff);

      debug_node(left, depth + 1);
      debug_node(right, depth + 1);
    }
  }
}

void ln_debug_ast(Parser parser) {
  for (int i = 0; i < parser.nodes.size; i++) {
    AST_Node *node = parser.nodes.nodes[i];
    debug_node(node, 0);
  }
}