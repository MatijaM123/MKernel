#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>
#include <message.h>

#define MAX_PROCESSES 512
#define MAX_OPEN_DEVICES 128

// Moguća stanja procesa
typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

// Stanje u registrima procesora (koristi se za context switch)
typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
    uint64_t rip, rsp, rflags;
} Registers;

//Resurs koji proces čeka
typedef enum {
    WAIT_NONE,
    WAIT_DEVICE,
    WAIT_TIMER,
    WAIT_SEMAPHORE,
    WAIT_MESSAGE,
    WAIT_IOPORT,
} WaitReason;


// IO uređaj (npr. fajl, port, stream)
typedef struct io_device {
    int device_id;
    int type;           // npr. 0: file, 1: port, 2: stream
    void* device_data;  // custom data za uređaj
} io_device_t;

// Kontrolni blok procesa
typedef struct KBP {
    int pid;                        // Jedinstveni identifikator procesa
    Registers regs;                 // Trenutno stanje procesa
    
    void* mem_start;               // Početak dodeljenog memorijskog segmenta
    size_t mem_size;               // Veličina memorije

    io_device_t open_devices[MAX_OPEN_DEVICES]; // Lista otvorenih uređaja

    struct KBP* next;              // Pokazivač na sledeći proces u scheduler listi

    MessageQueue msg_queue;  // Red dolaznih poruka
    int waiting_for_reply;   // 1 ako proces čeka odgovor
    Message reply_buffer;    // Gde će se smeštati odgovor


    WaitReason wait_reason;
    int wait_object_id;
} KBP;

#endif
