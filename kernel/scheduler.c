#include "scheduler.h"
#include "process.h"
#include "scheduler_lock.h"

static KBP* ready_head = NULL;
static KBP* ready_tail = NULL;
static KBP* blocked_head = NULL;
static KBP* blocked_tail = NULL;
static KBP* current_process = NULL;
static spinlock_t scheduler_lock;
static spinlock_t add_process_lock;
static spinlock_t block_process_lock;


int get_process_by_pid(int pid){
    KBP* p = ready_tail;
    while (p) {
        if(p->pid == pid)
            return p;
        p = p->next;
    }
    return 0;
}
void scheduler_init() {
    ready_head = NULL;
    ready_tail = NULL;
    current_process = NULL;
    scheduler_lock = spinlock_init();
    add_process_lock = spinlock_init();
    block_process_lock = spinlock_init();
}

void scheduler_add(KBP* process) {
    uint64_t flags;
    spinlock_acquire_irqsave(&add_process_lock, &flags);

    process->state = PROCESS_READY;
    process->next = NULL;

    if (!ready_head) {
        ready_head = ready_tail = process;
    } else {
        ready_tail->next = process;
        ready_tail = process;
    }

    spinlock_release_irqrestore(&add_process_lock, flags);
}

void scheduler_remove(KBP* process) {
    // kill operacija -> treba implementirati
}

void scheduler_block_on(KBP* process, WaitReason reason, int object_id) {
    uint64_t flags;
    spinlock_acquire_irqsave(&block_process_lock, &flags);

    process->state = PROCESS_BLOCKED;
    process->wait_reason = reason;
    process->wait_object_id = object_id;

    // Dodavanje na kraj liste blokiranih procesa
    process->next = NULL;
    if (!blocked_head) {
        blocked_head = blocked_tail = process;
    } else {
        blocked_tail->next = process;
        blocked_tail = process;
    }

    spinlock_release_irqrestore(&block_process_lock, flags);
}
void scheduler_unblock_all_on(WaitReason reason, int object_id) {
    uint64_t flags;
    spinlock_acquire_irqsave(&block_process_lock, &flags);

    KBP* prev = NULL;
    KBP* curr = blocked_head;

    while (curr) {
        KBP* next = curr->next;

        if (curr->wait_reason == reason && curr->wait_object_id == object_id) {
            // Ukloni iz liste blokiranih
            if (prev) {
                prev->next = next;
            } else {
                blocked_head = next;
            }
            if (curr == blocked_tail) {
                blocked_tail = prev;
            }

            curr->next = NULL;
            curr->wait_reason = WAIT_NONE;
            curr->wait_object_id = -1;

            // Dodaj u listu spremnih
            scheduler_add(curr);
        } else {
            prev = curr;
        }

        curr = next;
    }

    spinlock_release_irqrestore(&block_process_lock, flags);
}

void scheduler_unblock(KBP* process) {
    scheduler_add(process); // isto kao dodavanje u red spremnih procesa
}

KBP* get_current_process() {
    return current_process;
}

void scheduler_tick() {
    uint64_t flags;
    spinlock_acquire_irqsave(&scheduler_lock, &flags);

    if (!ready_head) {
        spinlock_release_irqrestore(&scheduler_lock, flags);
        return;
    }

    KBP* prev = current_process;
    current_process = ready_head;
    ready_head = ready_head->next;
    if (!ready_head)
        ready_tail = NULL;

    if (prev && prev->state == PROCESS_RUNNING) {
        prev->state = PROCESS_READY;
        prev->next = NULL;
        scheduler_add(prev); // sigurno zbog spinlocka
    }

    current_process->state = PROCESS_RUNNING;
    switch_context(prev, current_process);
    spinlock_release_irqrestore(&scheduler_lock, flags);
}

extern void _switch_context(Registers* old_regs, Registers* new_regs);


void switch_context(KBP* old_proc, KBP* new_proc) {
    if (!new_proc) return;

    if (old_proc) {
        _switch_context(&old_proc->regs, &new_proc->regs);
    } else {
        // prvi proces - ništa se ne čuva
        _switch_context(NULL, &new_proc->regs);
    }
}