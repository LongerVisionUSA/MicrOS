#ifndef SIGNALS_MANAGER_H
#define SIGNALS_MANAGER_H

#include "Interrupts/IDT/idt.h"
#include "Interrupts/IDT/interrupt_state.h"
#include "Process/Manager/process_manager.h"
#include "signal_params.h"

void signals_manager_init();
void signals_page_fault(exception_state *state);

#endif