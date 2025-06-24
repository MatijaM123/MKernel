#include "memory.h"
#include "tinyalloc.h"

static uint8_t heap[HEAP_SIZE];

void memory_init() {
    ta_init(heap, heap + HEAP_SIZE);
}

void* memory_alloc(size_t size) {
    return ta_alloc(size);
}

void memory_free(void* ptr, size_t size) {
    (void)size; // nije potrebno u tinyalloc
    ta_free(ptr);
}
