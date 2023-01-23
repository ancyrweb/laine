#include <stdio.h>

#include "src/compiler.h"

int main() {
  ln_compile("125 + 100 * 50 - 100 / 50 + 25;");
  return 0;
}
