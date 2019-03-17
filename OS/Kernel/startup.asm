;                                   MicrOS virtual memory layout
; |---------------|------------|------------|-------------|------------|-------------------------|
; |     1 MiB     |   15 MiB   |   1 Mib    |    4 MiB    |   3 MiB    |        1017 MiB         |
; | BIOS & Kernel |  Reserved  |   Stack    | Page tables |  Reserved  | Dynamic allocation area |
; |               |            |            |             |            |                         | 
; |  0xC0000000   | 0xC0100000 | 0xC1000000 | 0xC1100000  | 0xC1500000 |       0xC1800000        |
; |  0xC00FFFFF   | 0xC0FFFFFF | 0xC10FFFFF | 0xC14FFFFF  | 0xC17FFFFF |       0xFFFFFFFF        |
; |---------------|------------|------------|-------------|--------------------------------------|
; {     Page 0       }{1}{2}{3}{        Page 4       }{     Page 5     }{       Page 6 - n       }

[BITS 16]

%define PAGE_DIRECTORY_BASE 0x00006000
%define PAGE_TABLES_BASE    0x01100000
%define PAGES_COUNT         6

; %define PAGE_DIRECTORY_BASE 0x09000000
; %define PAGE_TABLES_BASE    0x0A000000
; %define PAGES_COUNT         6

jmp main

; Entry frame: https://wiki.osdev.org/GDT
gdt_begin:
; Null segment, reserved by CPU
gdt_null:
    dd 0x00000000
    dd 0x00000000

; Code segment
gdt_code:
    ; Segment limit (4 GiB)
    dw 0xFFFF

    ; Segment base address (16 bits)
    dw 0x0000

    ; Segment base address (8 bits)
    db 0x00

    ; 1 - present bit (1 for all valid sectors)
    ; 00 - privilege (ring level), 00 is the higest
    ; 1 - reserved
    ; 1 - excebutable bit, code can be excecuted here
    ; 0 - conforming bit, only kernel can execute code
    ; 1 - segment can be read
    ; 0 - access bit, default is zero
    db 10011010b

    ; 1 - granularity (0 = 1B block, 1 = 4 KiB block)
    ; 1 - size bit (0 = 16b protected mode, 1 = 32b protected mode)
    ; 00 - reserved
    ; 1111 - segment base address (4 bits)
    db 11001111b

    ; Segment base address (8 bits)
    db 0x00

; Data segment
gdt_data:
    ; Segment limit (4 GiB)
    dw 0xFFFF

    ; Segment base address (16 bits)
    dw 0x0000

    ; Segment base address (8 bits)
    db 0x00

    ; 1 - present bit (1 for all valid sectors)
    ; 00 - privilege (ring level), 00 is the higest
    ; 1 - reserved
    ; 0 - excebutable bit, code can't be excecuted here (because it's just data)
    ; 0 - conforming bit, only kernel can execute code
    ; 1 - segment can be read
    ; 0 - access bit, default is zero
    db 10010010b

    ; 1 - granularity (0 = 1B block, 1 = 4 KiB block)
    ; 1 - size bit (0 = 16b protected mode, 1 = 32b protected mode)
    ; 00 - reserved
    ; 1111 - segment base address (4 bits)
    db 11001111b

    ; Segment base address (8 bits)
    db 0x00
gdt_end:
gdt_description:
    dw gdt_end - gdt_begin - 1
    dd gdt_begin

main:
    ; Load memory map before we will switch to the protected mode
    ; Buffer segment
    mov bx, 0x0000
    mov es, bx

    ; Buffer offset
    mov bx, 0x5C00
    mov di, bx

    call load_memory_map

    ; Load GDT table
    lgdt [dword gdt_description]

    ; Set protected mode flag
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Jump to protected area 
    jmp dword 0x08:main_protected_area

; Input:
;   es - buffer segment
;   di - buffer offset
load_memory_map:
    xor ebx, ebx

    ; Push initial buffer address
    push di

    ; Push initial entries count
    mov eax, 0
    push eax

    load_memory_map_loop:
    ; Increment pointer in buffer
    add di, 24

    ; Increment entries count
    pop eax
    inc eax
    push eax

    ; Set magic number
    mov edx, 0x534d4150

    ; Set number of bytes to read
    mov ecx, 24

    ; Read memory map
    mov eax, 0xe820
    int 0x15
    
    ; Disable interrupts (can be enabled by int 0x15)
    cli

    ; Exit if ebx is equal to zero (reading has ended)
    cmp ebx, 0
    jne load_memory_map_loop

    ; Store entries count at the begin of the buffer
    pop eax
    pop di
    mov [di], eax
    
    ret
    
[BITS 32]
main_protected_area:
    ; Set data and stack segments to the third GDI descriptor
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    
    call clear_page_directory
    call clear_page_tables

    call create_page_directory
    call create_identity_page_table
    call create_kernel_page_table
    call enable_paging

    ; Set new stack with virtual address
    mov eax, 0xC1100000
    mov esp, eax

    ; Init FPU
    finit
    
    ; Jump to kmain and start kernel work
    extern kmain
    call kmain

    ; Something went wrong, but no problem!
    JMP $
    
; Output: nothing
clear_page_directory:
    ; Index
    mov eax, 0

    clear_page_directory_loop:
    ; Clear entry
    mov [PAGE_DIRECTORY_BASE + eax], byte 0

    ; Increment index
    inc eax

    ; Leave loop if we cleared all entries
    cmp eax, 0x1000
    jl clear_page_directory_loop

    ret

; Input: nothing
; Output: nothing
clear_page_tables:
    ; Index
    mov eax, 0

    clear_page_tables_loop:
    ; Clear entry
    mov [PAGE_TABLES_BASE + eax], byte 0

    ; Increment index
    inc eax

    ; Leave loop if we cleared all entries
    cmp eax, 0x400000
    jl clear_page_tables_loop

    ret

; Input: nothing
; Output: nothing
create_page_directory:
    ; Page definition macro
    %macro page_directory_definition 3
    mov eax, %1 + 3
    mov [%2 + %3], eax
    %endmacro
    
    ; Add temporary identity entry (physical address: 0x00000000, virtual address: 0x00000000, 24 MB)
    %assign i 0 
    %rep    PAGES_COUNT 
    page_directory_definition PAGE_TABLES_BASE + (i * 0x1000), PAGE_DIRECTORY_BASE, i * 4
    %assign i i+1 
    %endrep

    ; Add kernel entry (physical address: 0x00000000, virtual address: 0xC0000000, 24 MB)
    %assign i 0 
    %rep    PAGES_COUNT 
    page_directory_definition PAGE_TABLES_BASE + 0x00300000 + (i * 0x1000), PAGE_DIRECTORY_BASE + 0xC00, i * 4
    %assign i i+1 
    %endrep

    ret

; Input: nothing
; Output: nothing
create_identity_page_table:
    ; Entry index
    mov eax, 0

    ; Entry physical address
    mov ebx, 0

    fill_identity_page_table_loop:
    ; Set flags: read/write, present
    mov ecx, ebx
    or ecx, 3
    
    ; Set entry
    mov [PAGE_TABLES_BASE + eax*4], ecx

    ; Go to the next entry
    add ebx, 0x1000
    inc eax

    ; Leave loop if we filled all entries for the first 6 megabytes
    cmp eax, PAGES_COUNT * 0x1000
    jl fill_identity_page_table_loop

    ret

; Input: nothing
; Output: nothing
create_kernel_page_table:
    ; Entry index
    mov eax, 0

    ; Entry physical address
    mov ebx, 0

    fill_kernel_page_table_loop:
    ; Set flags: read/write, present
    mov ecx, ebx
    or ecx, 3
    
    ; Set entry
    mov [PAGE_TABLES_BASE + 0x00300000 + eax*4], ecx

    ; Go to the next entry
    add ebx, 0x1000
    inc eax

    ; Leave loop if we filled all entries for the first megabyte
    cmp eax, PAGES_COUNT * 0x1000
    jl fill_kernel_page_table_loop

    ret

; Input: nothing
; Output: nothing
enable_paging:
    ; Set address of the directory table
    mov eax, PAGE_DIRECTORY_BASE
    mov cr3, eax

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    jmp .branch
    nop
    nop
    nop
    nop
    nop
    .branch:

    ret