#include "port_io.h"
#include <stdint.h>

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_FREQUENCY 1193182

void timer_init(uint32_t freq_hz) {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / freq_hz);

    outb(PIT_COMMAND, 0x36); 

    outb(PIT_CHANNEL0, divisor & 0xFF);  
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF); 
}


volatile uint64_t system_ticks = 0;

uint64_t get_ticks() {
    return system_ticks;
}

void increment_ticks() {
    system_ticks++;
}