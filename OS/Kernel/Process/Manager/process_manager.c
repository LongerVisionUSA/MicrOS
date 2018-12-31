#include "process_manager.h"

kvector processes;
volatile uint32_t current_process_id = 0;
volatile uint32_t next_process_id = 0;
volatile uint32_t last_task_switch = 0;
volatile bool run_scheduler_on_next_interrupt = false;

extern void enter_user_space(interrupt_state *address);

void process_manager_init()
{
    last_task_switch = timer_get_system_clock();

    kvector_init(&processes);
    idt_attach_process_manager(process_manager_interrupt_handler);
}

uint32_t process_manager_create_process(char *path, char *parameters)
{
    process_info *process = (process_info *)heap_kernel_alloc(sizeof(process_info), 0);
    process->id = next_process_id++;
    process->cpu_usage = 0;

    process->page_directory = heap_kernel_alloc(1024 * 4, 1024 * 4);

    paging_table_entry *page_directory = paging_get_page_directory();
    memcpy(process->page_directory, (void *)page_directory, 1024 * 4);

    paging_set_page_directory(process->page_directory);

    filesystem_file_info process_file_info;
    fat_generic_get_file_info(path, &process_file_info);

    uint8_t *process_content = heap_kernel_alloc(process_file_info.size, 0);
    fat_read_file(path, process_content, 0, process_file_info.size);

    elf_header *app_header = elf_get_header(process_content);
    uint32_t initial_page = elf_loader_load(process_content);
    process->size_in_memory = elf_get_total_size_in_memory(process_content);

    process->heap = (void *)(initial_page * 1024 * 1024 * 4 + process->size_in_memory);
    heap_set_user_heap((void *)(process->heap));
    heap_init_user_heap();

    uint32_t path_length = strlen(path) + 1;
    uint32_t parameters_length = strlen(parameters) + 1;

    void *path_in_user_heap = heap_user_alloc(path_length, 0);
    void *parameters_in_user_heap = heap_user_alloc(path_length, 0);

    memcpy(path_in_user_heap, path, path_length);
    memcpy(parameters_in_user_heap, parameters, parameters_length);

    process->user_stack = heap_user_alloc(1024 * 1024, 32) + (1024 * 1024);

    //*((uint32_t *)process->user_stack - 1) = (uint32_t)parameters_in_user_heap;
    //*((uint32_t *)process->user_stack - 2) = (uint32_t)path_in_user_heap;

    process->state.eip = app_header->entry_position;
    process->state.esp = (uint32_t)process->user_stack - 64;
    process->state.interrupt_number = 0;
    process->state.eflags = 0x200;
    process->state.cs = 0x1B;
    process->state.ss = 0x23;

    process->state.registers.eax = 0;
    process->state.registers.ebx = 0;
    process->state.registers.ecx = 0;
    process->state.registers.edx = 0;
    process->state.registers.esp_unused = (uint32_t)process->state.esp;
    process->state.registers.ebp = 0;
    process->state.registers.esi = 0;
    process->state.registers.edi = 0;

    if (processes.count == 0)
    {
        current_process_id = process->id;
    }

    kvector_add(&processes, process);
    heap_kernel_dealloc(process_content);

    paging_set_page_directory(page_directory);
    return process->id;
}

process_info *process_manager_get_process(uint32_t process_id)
{
    for (uint32_t i = 0; i < processes.count; i++)
    {
        if (((process_info *)processes.data[i])->id == process_id)
        {
            return (process_info *)processes.data[i];
        }
    }

    return NULL;
}

void process_manager_save_current_process_state(interrupt_state *state, uint32_t delta)
{
    process_info *old_process = processes.data[current_process_id];
    memcpy(&old_process->state, state, sizeof(interrupt_state));

    old_process->cpu_usage = (old_process->cpu_usage + delta) / 2;
}

void process_manager_switch_to_next_process()
{
    uint32_t new_process_id = (current_process_id + 1) % processes.count;
    process_info *new_process = processes.data[new_process_id];
    current_process_id = new_process_id;

    paging_set_page_directory(new_process->page_directory);
    heap_set_user_heap((void *)(new_process->heap));

    pic_confirm_master_and_slave();
    __asm__("mov %0, %%eax\n"
            "mov %%eax, %%esp"
            :
            : "g"(new_process->state.esp)
            : "eax");

    enter_user_space(&new_process->state);
}

void process_manager_close_current_process()
{
    // TODO: release process memory
    kvector_remove(&processes, current_process_id);

    if (processes.count > 0)
    {
        current_process_id--;
        process_manager_switch_to_next_process();
    }
    else
    {
        // TODO: do something better
        io_disable_interrupts();
        __asm__("hlt");
    }
}

uint32_t process_manager_get_processes_count()
{
    return processes.count;
}

process_info *process_manager_get_process_info(uint32_t id)
{
    for (uint32_t i = 0; i < processes.count; i++)
    {
        process_info *process = processes.data[i];
        if (process->id == id)
        {
            return process;
        }
    }

    return NULL;
}

void process_manager_get_current_process_user_info(process_user_info *user_info)
{
    process_manager_get_process_user_info(current_process_id, user_info);
}

bool process_manager_get_process_user_info(uint32_t id, process_user_info *user_info)
{
    process_info *process = process_manager_get_process_info(id);
    if (process == NULL)
    {
        return false;
    }

    process_manager_convert_process_info_to_user_info(process, user_info);
    return true;
}

void process_manager_get_all_processes_user_info(process_user_info *user_info)
{
    for (uint32_t i = 0; i < processes.count; i++)
    {
        process_info *process = processes.data[i];
        process_manager_convert_process_info_to_user_info(process, &user_info[i]);
    }
}

bool process_manager_set_current_process_name(char *name)
{
    process_info *process = process_manager_get_process_info(current_process_id);
    if (process != NULL)
    {
        memcpy(process->name, name, 32);
        return true;
    }

    return false;
}

void process_manager_convert_process_info_to_user_info(process_info *process, process_user_info *user_info)
{
    user_info->id = process->id;
    memcpy(user_info->name, process->name, 32);

    user_info->cpu_usage = process->cpu_usage;
    user_info->memory_usage = process_manager_get_process_memory_usage(process);
}

uint32_t process_manager_get_process_memory_usage(process_info *process)
{
    void *old_page_directory = paging_get_page_directory();
    void *old_heap = heap_get_user_heap();

    paging_set_page_directory(process->page_directory);
    heap_set_user_heap((void *)process->heap);
    uint32_t allocated_pages = virtual_memory_get_allocated_pages_count(false);

    heap_set_user_heap(old_heap);
    paging_set_page_directory(old_page_directory);

    return allocated_pages * 4;
}

void process_manager_interrupt_handler(interrupt_state *state)
{
    uint32_t delta = timer_get_system_clock() - last_task_switch;

    // TODO: processes.count > 0 is temporary here, idle process will be always present
    if ((run_scheduler_on_next_interrupt || state->cs == 0x1B) && processes.count > 0 && delta >= 10)
    {
        run_scheduler_on_next_interrupt = false;
        last_task_switch = timer_get_system_clock();

        if (state->cs == 0x1B)
        {
            process_manager_save_current_process_state(state, delta);
        }

        process_manager_switch_to_next_process(state);
    }
}

void process_manager_run()
{
    run_scheduler_on_next_interrupt = true;
}