global enter_user_space

; Input: interrupt number on stack
; Output: nothing
enter_user_space:
    ; Set data segments (0x20 + 0x03 (ring 3))
    mov ax, 0x23
    mov ds, ax
    mov es, ax 
    mov fs, ax 
    mov gs, ax

    ; Set stack pointer to the interrupt state structure
    mov eax, [esp + 4]
    mov esp, eax
    
    ; Load FPU state
    fwait
    frstor [esp]
    
    ; Move stack pointer (frstor won't do this itself)
    add esp, 108
    
    ; Restore registers
    popa
    
    ; Skip interrupt number
    add esp, 4
    
    ; Bye!
    iret