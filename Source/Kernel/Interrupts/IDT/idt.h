#ifndef IDT_H
#define IDT_H

#define IDT_INTERRUPT_DESCRIPTOR_TABLE_LENGTH 64
#define IDT_MAX_INTERRUPT_HANDLERS 64

#include "idt_entry.h"
#include "idt_info.h"
#include "interrupt_state.h"
#include "interrupt_handler_definition.h"
#include "../PIC/pic.h"

void idt_init();
void idt_set(uint8_t index, uint32_t (*handler)());
void idt_unset(uint8_t index);
void idt_attach_interrupt_handler(uint8_t interrupt_number, void (*handler)());
void idt_detach_interrupt_handler(uint8_t interrupt_number, void (*handler)());

void global_int_handler();

// Hardware interrupts
extern uint32_t int0();
extern uint32_t int1();
extern uint32_t int2();
extern uint32_t int3();
extern uint32_t int4();
extern uint32_t int5();
extern uint32_t int6();
extern uint32_t int7();
extern uint32_t int8();
extern uint32_t int9();
extern uint32_t int10();
extern uint32_t int11();
extern uint32_t int12();
extern uint32_t int13();
extern uint32_t int14();
extern uint32_t int15();

// Software interrupts
extern uint32_t int48();
extern uint32_t int49();
extern uint32_t int50();
extern uint32_t int51();
extern uint32_t int52();

#endif