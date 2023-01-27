#include <stdio.h>

#include "src/compiler.h"

int main() {
  CompilerOptions options = { 
    .debug_ast = false, 
    .debug_tokens = false 
  };

  ln_compile("int j = 123; ++j; j = j + 1;", options);
  return 0;
}
