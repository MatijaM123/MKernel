#include "idt.h"
#include "pic.h"
#include "interrupts.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void irq_install() {
    pic_remap();

    idt_set_gate(32, irq0, 0, 0x8E);
    idt_set_gate(33, irq1, 0, 0x8E);
    idt_set_gate(34,irq2,0,0x8E);
    idt_set_gate(35,irq3,0,0x8E);
    idt_set_gate(36,irq4,0,0x8E);
    idt_set_gate(37,irq5,0,0x8E);
    idt_set_gate(38,irq6,0,0x8E);
    idt_set_gate(39,irq7,0,0x8E);
    idt_set_gate(40,irq8,0,0x8E);
    idt_set_gate(41,irq9,0,0x8E);
    idt_set_gate(42,irq10,0,0x8E);
    idt_set_gate(43,irq11,0,0x8E);
    idt_set_gate(44,irq12,0,0x8E);
    idt_set_gate(45,irq13,0,0x8E);
    idt_set_gate(46,irq14,0,0x8E);
    idt_set_gate(47, irq15, 0, 0x8E);
}

void irq_common_stub(uint64_t irq_num) {
    handle_interrupt(irq_num); // ovo je tvoja funkcija
    pic_send_eoi(irq_num);
}