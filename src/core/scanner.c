#include "scanner.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

Scanner scanner;

static bool is_eof() {
  return scanner.start >= scanner.source_length;
}

static char advance() {
  return scanner.source[scanner.current++];
}

static char peek() {
  return scanner.source[scanner.current];
}

static void add_token() {
  
}

static void skip_whitespaces() {
  while(is_eof() == false) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t': {
        advance();
        break;
      }
      case '\n': {
        scanner.line++;
        advance();
        break;
      }
      default: {
        return;
      }
    }
  }
}

// Core

void ln_scan_init(const char *source) {
  scanner.source = source;
  scanner.source_length = strlen(source);
  scanner.tokens = ln_tokenlist_init();
  scanner.start = 0;
  scanner.current = 0;
  scanner.line = 0;
}

void ln_scan_start() {
  while (!is_eof()) {
    scanner.start = scanner.current;
    skip_whitespaces();

    if (is_eof()) {
      add_token(T_EOF)
      break;
    }

    const c = peek();
    switch (c) {
      default: {
        printf("Unrecognized token %c\n", c);
        advance();
      }
    }
  }
}

void ln_scan_free() {
  scanner.source = NULL;
  ln_tokenlist_free(scanner.tokens);
  scanner.tokens = NULL;
}