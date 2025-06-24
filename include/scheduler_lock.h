#ifndef SCHEDULER_LOCK_H
#define SCHEDULER_LOCK_H

#include <stdint.h>

typedef struct {
    volatile uint8_t locked;
} spinlock_t;

void spinlock_init(spinlock_t* lock);
void spinlock_acquire_irqsave(spinlock_t* lock, uint64_t* flags);
void spinlock_release_irqrestore(spinlock_t* lock, uint64_t flags);

#endif