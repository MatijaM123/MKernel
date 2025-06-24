// _switch_context.S (или inline ASM ако желиш)
.global _switch_context
.type _switch_context, @function

_switch_context:
    // Argumenti: rdi = &old_regs (може бити NULL)
    //             rsi = &new_regs

    // Ako postoji old_regs, sačuvaj stanje u njega
    test rdi, rdi
    jz 1f

    mov [rdi + 0x00], r15
    mov [rdi + 0x08], r14
    mov [rdi + 0x10], r13
    mov [rdi + 0x18], r12
    mov [rdi + 0x20], r11
    mov [rdi + 0x28], r10
    mov [rdi + 0x30], r9
    mov [rdi + 0x38], r8
    mov [rdi + 0x40], rsi     // пријемни регистри
    mov [rdi + 0x48], rdi
    mov [rdi + 0x50], rbp
    mov [rdi + 0x58], rdx
    mov [rdi + 0x60], rcx
    mov [rdi + 0x68], rbx
    mov [rdi + 0x70], rax

    pushfq
    popq rax
    mov [rdi + 0x78], rax

    mov rax, rsp
    mov [rdi + 0x80], rax

    lea rax, [rip + 1f]   // адреса наставка после switch-a
    mov [rdi + 0x88], rax

1:
    // učitaj nove registre
    mov r15, [rsi + 0x00]
    mov r14, [rsi + 0x08]
    mov r13, [rsi + 0x10]
    mov r12, [rsi + 0x18]
    mov r11, [rsi + 0x20]
    mov r10, [rsi + 0x28]
    mov r9,  [rsi + 0x30]
    mov r8,  [rsi + 0x38]
    mov rsi, [rsi + 0x40]
    mov rdi, [rsi + 0x48]
    mov rbp, [rsi + 0x50]
    mov rdx, [rsi + 0x58]
    mov rcx, [rsi + 0x60]
    mov rbx, [rsi + 0x68]
    mov rax, [rsi + 0x70]

    mov rsp, [rsi + 0x80]
    mov rax, [rsi + 0x78]
    push rax
    popfq

    jmp [rsi + 0x88]   // skok na novi RIP
