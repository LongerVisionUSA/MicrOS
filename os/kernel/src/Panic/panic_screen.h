#ifndef PANIC_SCREEN_H
#define PANIC_SCREEN_H

#define STACK_POINTER_OFFSET 128

#include <stdlib.h>
#include <stdint.h>
#include "Drivers/VGA/vga.h"
#include "Drivers/VGA/vga_gmode.h"
#include "Drivers/Keyboard/keyboard.h"
#include "Library/kvector.h"
#include "Memory/GDT/gdt_entry.h"
#include "Interrupts/IDT/idt_entry.h"
#include "Timer/timer.h"

void panic_screen_show(exception_state *state, uint32_t code, const char *optString);
void panic_screen_display_intro(exception_state *state, uint32_t code, const char *optString);
void panic_screen_wait_for_key_press();
void panic_screen_display_diagnostic_view(exception_state *state, uint32_t system_clock);
bool panic_screen_is_privilege_level_changed(exception_state *state);
char *panic_screen_value_to_string(char *buffer, unsigned int value);
void panic_screen_display_register_state(char *register_name, unsigned int value, bool new_line);

void panic_screen_display_eflags(uint32_t eflags);
void panic_screen_display_cr0(uint32_t cr0);
void panic_screen_display_cr4(uint32_t cr4);

void panic_screen_display_stack(uint32_t esp, bool user_stack);
void panic_screen_display_descriptor_table(char *name, uint64_t value, uint32_t size_of_entry);
void panic_screen_display_system_clock(uint32_t system_clock);
void panic_screen_display_fpu_control_word(uint32_t control_word);
void panic_screen_display_fpu_status_word(uint32_t status_word);

void panic_screen_display_flag_tags(uint32_t value, char **tags);

#endif