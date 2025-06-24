#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

void scheduler_init();
void scheduler_add(KBP* process);
void scheduler_remove(KBP* process);
void scheduler_block(KBP* process);
void scheduler_unblock(KBP* process);
void scheduler_tick(); // позива се периодично (нпр. од тајмера)

KBP* get_current_process();

#endif
