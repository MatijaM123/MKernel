#include "tinyalloc.h"

typedef struct ta_header {
    size_t size;
    struct ta_header* next;
    int is_free;
} ta_header;

static ta_header* ta_head = 0;
static void* ta_end = 0;

void ta_init(void* heap_start, void* heap_end) {
    ta_head = (ta_header*)heap_start;
    ta_head->size = (uintptr_t)heap_end - (uintptr_t)heap_start - sizeof(ta_header);
    ta_head->next = 0;
    ta_head->is_free = 1;

    ta_end = heap_end;
}

void* ta_alloc(size_t size) {
    ta_header* curr = ta_head;

    while (curr) {
        if (curr->is_free && curr->size >= size) {
            if (curr->size > size + sizeof(ta_header)) {
                // Podeli blok
                ta_header* new_block = (ta_header*)((uintptr_t)curr + sizeof(ta_header) + size);
                new_block->size = curr->size - size - sizeof(ta_header);
                new_block->next = curr->next;
                new_block->is_free = 1;

                curr->next = new_block;
                curr->size = size;
            }

            curr->is_free = 0;
            return (void*)((uintptr_t)curr + sizeof(ta_header));
        }
        curr = curr->next;
    }

    return 0;
}

void ta_free(void* ptr) {
    if (!ptr) return;

    ta_header* header = (ta_header*)((uintptr_t)ptr - sizeof(ta_header));
    header->is_free = 1;

    // Spajanje slobodnih blokova (coalescing)
    ta_header* curr = ta_head;
    while (curr && curr->next) {
        if (curr->is_free && curr->next->is_free) {
            curr->size += sizeof(ta_header) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}
