#ifndef DEF_MEMORY
#define DEF_MEMORY

#define ALLOCATE(type, count) ln_reallocate(NULL, 0, sizeof(type) * (count))
#define DEALLOCATE(ptr) ln_reallocate(ptr, 0, 0)

void *ln_reallocate(void *ptr, int old_size, int new_size);

#endif