#include "memory.h"

#include <stdlib.h>
#include <stdint.h>

void *ln_reallocate(void *ptr, int old_size, int new_size) {
  if (new_size == 0) {
    free(ptr);
    return NULL;
  }

  void *next = realloc(ptr, new_size);
  if (next == NULL) {
    exit(1);
  }

  return next;
}

void *ln_allocate(int size) {
  void *next = malloc(size);
  if (next == NULL) {
    exit(1);
  }

  return next;
}