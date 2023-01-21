#include "compiler.h"
#include "core/scanner.h"

Compiler compiler;

void ln_compile(const char *source) {
  ln_scan_init(source);
  ln_scan_start();

  ln_scan_free();  
}