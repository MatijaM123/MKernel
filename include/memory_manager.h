#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

#define HEAP_SIZE 1024 * 1024 * 8  // 8 MiB

void memory_init();
void* memory_alloc(size_t size);
void memory_free(void* ptr, size_t size);

#endif
