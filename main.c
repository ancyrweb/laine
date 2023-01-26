#include <stdio.h>

#include "src/compiler.h"

int main() {
  ln_compile("int = 123; 123 + 3 / 4 * 10 - 3 int 456; 1;");
  return 0;
}
