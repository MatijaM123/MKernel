#include "scheduler_lock.h"

static inline uint8_t xchg8(volatile uint8_t* addr, uint8_t val) {
    uint8_t old;
    asm volatile (
        "lock xchg %0, %1"
        : "=r"(old), "+m"(*addr)
        : "0"(val)
        : "memory"
    );
    return old;
}

static inline void cli() {
    asm volatile("cli" ::: "memory");
}

static inline void sti() {
    asm volatile("sti" ::: "memory");
}

static inline uint64_t read_flags() {
    uint64_t rflags;
    asm volatile("pushfq; popq %0" : "=r"(rflags));
    return rflags;
}

static inline void write_flags(uint64_t rflags) {
    asm volatile("pushq %0; popfq" :: "r"(rflags));
}

void spinlock_init(spinlock_t* lock) {
    lock->locked = 0;
}

void spinlock_acquire_irqsave(spinlock_t* lock, uint64_t* flags) {
    *flags = read_flags();
    cli();
    while (xchg8(&lock->locked, 1)) {
        while (lock->locked); 
    }
}

void spinlock_release_irqrestore(spinlock_t* lock, uint64_t flags) {
    lock->locked = 0;
    write_flags(flags);
}
