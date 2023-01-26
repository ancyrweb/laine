#ifndef LAINE_DEBUG_H
#define LAINE_DEBUG_H

#include "token.h"
#include "ast.h"
#include "parser.h"

char* ln_debug_toktostr(TokenType type);
void ln_debug_tokens(TokenList *list);
void ln_debug_ast(Parser parser);
void ln_debug_expr_node(ASTExprNode *node, unsigned int depth);
void ln_debug_stmt_node(ASTStatementNode *node, int depth);

#endif