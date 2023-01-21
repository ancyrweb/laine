#ifndef LAINE_DEBUG_H
#define LAINE_DEBUG_H

#include "token.h"
#include "ast.h"
#include "parser.h"

void ln_debug_tokens(const char *source, TokenList *list);
void ln_debug_ast(Parser parser);

#endif