#ifndef LAINE_COMPILER_H
#define LAINE_COMPILER_H

#include "core/token.h"
#include <stdbool.h>

typedef struct CompilerOptions {
  bool debug_tokens;
  bool debug_ast;
} CompilerOptions;

CompilerOptions compiler_options;

void ln_compile(const char *source, CompilerOptions options);

#endif //LAINE_COMPILER_H
