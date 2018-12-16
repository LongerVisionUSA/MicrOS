#ifndef VIRTUAL_MEMORY_MANAGER_H
#define VIRTUAL_MEMORY_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "../../Paging/paging.h"
#include "../Physic/physical_memory_manager.h"

uint32_t virtual_memory_alloc_page(bool supervisor);
bool virtual_memory_dealloc_page(uint32_t index);
bool virtual_memory_dealloc_last_page(bool supervisor);

void virtual_memory_set_kernel_base_page_index(uint32_t index);
void virtual_memory_set_user_base_page_index(uint32_t index);

uint32_t virtual_memory_get_allocated_pages_count(bool supervisor);

#endif