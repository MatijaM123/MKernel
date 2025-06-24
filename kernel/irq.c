#include "idt.h"
#include "pic.h"
#include "interrupts.h" // your handle_interrupt()

extern void irq0();
extern void irq1();
// ...
extern void irq15();

void irq_install() {
    pic_remap();

    idt_set_gate(32, irq0, 0, 0x8E);
    idt_set_gate(33, irq1, 0, 0x8E);
    // ...
    idt_set_gate(47, irq15, 0, 0x8E);
}
