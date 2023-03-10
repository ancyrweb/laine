#include "compiler.h"
#include "core/parser.h"
#include "core/scanner.h"

#include <stdio.h>

void ln_compile(const char *source, CompilerOptions options) {
  compiler_options = options;

  ln_scan_init(source);
  ln_scan_start();

  ln_parser_start(scanner.tokens);

  ln_parser_free();
  ln_scan_free();  
}