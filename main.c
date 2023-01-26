#include <stdio.h>

#include "src/compiler.h"

int main() {
  ln_compile("1 + 2; \"This is a string\"; v + 3; 10 * 3.5 + 10 - 90;");
  return 0;
}
