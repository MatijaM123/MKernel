#include "idt.h"

#define IDT_SIZE 256
idt_entry_t idt[IDT_SIZE];
idt_ptr_t idtp;

extern void idt_flush(); //asemblerska funkcija

void idt_set_gate(int n, void (*handler)(), uint8_t ist, uint8_t flags) {
    uint64_t addr = (uint64_t)handler;
    idt[n].offset_low = addr & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].ist = ist;
    idt[n].type_attr = flags;
    idt[n].offset_mid = (addr >> 16) & 0xFFFF;
    idt[n].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[n].zero = 0;
}

void idt_load() {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;
    idt_flush(); // asm: lidt [idtp]
}
