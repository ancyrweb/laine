#include "scanner.h"
#include "memory.h"
#include "debug.h"
#include "../compiler.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

static char peek_next() {
  return scanner.source[scanner.current + 1];
}

static void scan_err(const char *message) {
  fprintf(stderr, "%s", message);
  exit(1);
}

static void add_token(TokenType type) {
  Token *t = ALLOCATE(Token, 1);
  t->start = scanner.start;
  t->length = scanner.current - scanner.start;
  t->line = scanner.line;
  t->type = type;

  ln_tokenlist_add(scanner.tokens, t);
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

static bool match(char c) {
  if (is_eof() == false && peek() == c) {
    advance();
    return true;
  }

  return false;
}

static bool is_digit(c) {
  return c >= '0' && c <= '9';
}

static bool is_alpha(c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static void digits() {
  while(!is_eof() && is_digit(peek())) {
    advance();
  }

  if (match('.')) {
    if (!is_digit(peek())) {
      // a floating point cannot end with a "."
      // TODO error handling
      scan_err("A floating point cannot end with a .");
    }

    while(!is_eof() && is_digit(peek())) {
      advance();
    }

    add_token(T_FLOATING_LITERAL);
    return;
  }

  add_token(T_INTEGER_LITERAL);
}

static void identifier_or_keyword() {
  while(!is_eof() && is_alpha(peek())) {
    advance();
  }

  size_t size = scanner.current - scanner.start;
  char buff[size + 1];
  
  memcpy(&buff, scanner.source + scanner.start, size);
  buff[size] = '\0';

  // Here goes nothing
  if (memcmp(buff, "byte", 4) == 0) {
    add_token(T_BYTE);
  } else if (memcmp(buff, "short", 5) == 0) {
    add_token(T_SHORT);
  } else if (memcmp(buff, "int", 3) == 0) {
    add_token(T_INT);
  } else if (memcmp(buff, "long", 4) == 0) {
    add_token(T_LONG);
  } else if (memcmp(buff, "float", 5) == 0) {
    add_token(T_FLOAT);
  } else if (memcmp(buff, "bool", 4) == 0) {
    add_token(T_BOOL);
  } else if (memcmp(buff, "do", 2) == 0) {
    add_token(T_DO);
  } else if (memcmp(buff, "while", 5) == 0) {
    add_token(T_WHILE);
  } else if (memcmp(buff, "for", 3) == 0) {
    add_token(T_FOR);
  } else if (memcmp(buff, "if", 2) == 0) {
    add_token(T_IF);
  } else if (memcmp(buff, "else", 4) == 0) {
    add_token(T_ELSE);
  } else if (memcmp(buff, "elseif", 6) == 0) {
    add_token(T_ELSEIF);
  } else if (memcmp(buff, "true", 4) == 0) {
    add_token(T_TRUE);
  } else if (memcmp(buff, "false", 5) == 0) {
    add_token(T_FALSE);
  } else {
    add_token(T_IDENTIFIER);
  }
  
}

static void string_literal() {
  char c = advance();
  while (!is_eof() && c != '"') {
    c = advance();
  }

  if (is_eof()) {
    scan_err("Unterminated literal string.");
  }

  add_token(T_STRING_LITERAL);
}
// Core

void ln_scan_init(const char *source) {
  scanner.source = source;
  scanner.source_length = strlen(source);
  scanner.tokens = ln_tokenlist_init();
  scanner.tokens->source = source;
  scanner.start = 0;
  scanner.current = 0;
  scanner.line = 0;
}

void ln_scan_start() {
  #define MATCH(symbol, token) \
    case symbol: add_token((token)); break

  while (!is_eof()) {
    skip_whitespaces();
    scanner.start = scanner.current;

    if (is_eof()) {
      add_token(T_EOF);
      break;
    }

    char c = advance();
    switch (c) {
      MATCH(';', T_SEMICOLON);
      MATCH('{', T_BRACE_LEFT);
      MATCH('}', T_BRACE_RIGHT);
      MATCH('[', T_BRACKET_LEFT);
      MATCH(']', T_BRACKET_RIGHT);
      MATCH('(', T_PAREN_LEFT);
      MATCH(')', T_PAREN_RIGHT);
      MATCH('.', T_DOT);
      case '=': {
        if (peek() == '=') {
          add_token(T_EQUAL_EQUAL);
          advance();
        } else {
          add_token(T_EQUAL);
        }

        break;
      }
      case '+': {
        if (match('=')) {
          add_token(T_PLUS_EQUAL);
        } else if (match('+')) {
          add_token(T_INCREMENT);
        } else {
          add_token(T_PLUS);
        }
        break;
      }
      case '-': {
        if (match('=')) {
          add_token(T_MINUS_EQUAL);
        } else if (match('+')) {
          add_token(T_DECREMENT);
        } else {
          add_token(T_MINUS);
        }
        break;
      }
      case '*': {
        if (match('=')) {
          add_token(T_STAR_EQUAL);
        } else {
          add_token(T_STAR);
        }
        break;
      }
      case '/': {
        if (match('=')) {
          add_token(T_SLASH_EQUAL);
        } else {
          add_token(T_SLASH);
        }
        break;
      }
      case '%': {
        if (match('=')) {
          add_token(T_MODULO_EQUAL);
        } else {
          add_token(T_MODULO);
        }
        break;
      }
      case '>': {
        if (match('=')) {
          add_token(T_GREATER_EQUAL);
        } else {
          add_token(T_GREATER);
        }
        break;
      }
      case '<': {
        if (match('=')) {
          add_token(T_LOWER_EQUAL);
        } else {
          add_token(T_LOWER);
        }
        break;
      }
      case '"': {
        string_literal();
        break;
      }
      default: {
        if (is_digit(c)) {
          digits();
          break;
        } else if (is_alpha(c)) {
          identifier_or_keyword();
          break;
        }

        printf("Unrecognized token %c\n", c);
      }
    }
  }

  if (compiler_options.debug_tokens) {
    ln_debug_tokens(scanner.tokens);
  }
  #undef MATCH
}

void ln_scan_free() {
  scanner.source = NULL;
  ln_tokenlist_free(scanner.tokens);
  scanner.tokens = NULL;
}