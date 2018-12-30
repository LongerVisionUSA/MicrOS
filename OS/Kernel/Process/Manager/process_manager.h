#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdlib.h>
#include <kvector.h>
#include "process_info.h"
#include "process_user_info.h"
#include "../../Memory/Paging/paging.h"
#include "../ELF/Parser/elf_header.h"
#include "../ELF/Loader/elf_loader.h"
#include "../../FileSystems/FAT/fat.h"
#include "../../Assembly/io.h"
#include "../../Interrupts/IDT/idt.h"

void process_manager_init();
uint32_t process_manager_create_process(char *path, char *parameters);
process_info *process_manager_get_process(uint32_t process_id);

void process_manager_save_current_process_state(interrupt_state *state);
void process_manager_switch_to_next_process();
void process_manager_close_current_process();
uint32_t process_manager_get_processes_count();
process_info *process_manager_get_process_info(uint32_t id);
bool process_manager_get_process_user_info(uint32_t id, process_user_info *user_info);
bool process_manager_set_current_process_name(char *name);

void process_manager_interrupt_handler(interrupt_state *state);
void process_manager_run();

#endif