#include <stdio.h>

#include "src/compiler.h"

int main() {
  ln_compile("int a = 0; int b = 1; int c = a + b;");
  return 0;
}
