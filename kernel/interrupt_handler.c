
#define MAX_IRQS 16

KBP* irq_handlers[MAX_IRQS]; // Niz drajvera registrovanih za IRQ-ove

int register_irq_handler(int irq_num, KBP* handler_proc) {
    if (irq_num < 0 || irq_num >= MAX_IRQS) return -1;

    irq_handlers[irq_num] = handler_proc;
    return 0;
}

void handle_interrupt(int irq_num) {
    KBP* handler = irq_handlers[irq_num];
    if (!handler) return;

    Message msg = {
        .sender_pid = -1,
        .receiver_pid = handler->pid,
        .is_reply = 0,
        .reply_expected = 0,
    };
    strncpy(msg.data, "INTERRUPT", MAX_MSG_DATA);

    enqueue_message(&handler->msg_queue, &msg);
    scheduler_unblock_all_on(WAIT_DEVICE, irq_num);
}


//inb/outb sistemski taskovi za usmeravanje drajvera
uint8_t sys_inb(uint16_t port) {
    uint8_t val;
    asm volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void sys_outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}