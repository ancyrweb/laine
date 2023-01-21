#ifndef LAINE_SCANNER_H
#define LAINE_SCANNER_H

#include "token.h"
#include "../common.h"

typedef struct {
  const char *source;
  int source_length;
  TokenList *tokens;

  int line;
  int start;
  int current;
} Scanner;

extern Scanner scanner;

void ln_scan_init(const char *source);
void ln_scan_start();
void ln_scan_free();

#endif //LAINE_SCANNER_H
