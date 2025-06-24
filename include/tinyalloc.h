#ifndef TINYALLOC_H
#define TINYALLOC_H

#include <stdint.h>
#include <stddef.h>

void ta_init(void* heap_start, void* heap_end);
void* ta_alloc(size_t size);
void ta_free(void* ptr);

#endif
