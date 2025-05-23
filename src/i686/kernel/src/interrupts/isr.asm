extern kernel_panic
global isr_common_subr
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

;  0: Divide By Zero Exception
isr0:
    cli
    push dword 0
    push dword 0
    jmp isr_common_subr

;  1: Debug Exception
isr1:
    cli
    push dword 0
    push dword 1
    jmp isr_common_subr

;  2: Non Maskable Interrupt Exception
isr2:
    cli
    push dword 0
    push dword 2
    jmp isr_common_subr

;  3: Int 3 Exception
isr3:
    cli
    push dword 0
    push dword 3
    jmp isr_common_subr

;  4: INTO Exception
isr4:
    cli
    push dword 0
    push dword 4
    jmp isr_common_subr

;  5: Out of Bounds Exception
isr5:
    cli
    push dword 0
    push dword 5
    jmp isr_common_subr

;  6: Invalid Opcode Exception
isr6:
    cli
    push dword 0
    push dword 6
    jmp isr_common_subr

;  7: Coprocessor Not Available Exception
isr7:
    cli
    push dword 0
    push dword 7
    jmp isr_common_subr

;  8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push dword 8
    jmp isr_common_subr

;  9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push dword 0
    push dword 9
    jmp isr_common_subr

; 10: Bad TSS Exception (With Error Code!)
isr10:
    cli
    push dword 10
    jmp isr_common_subr

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push dword 11
    jmp isr_common_subr

; 12: Stack Fault Exception (With Error Code!)
isr12:
    cli
    push dword 12
    jmp isr_common_subr

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push dword 13
    jmp isr_common_subr

; 14: Page Fault Exception (With Error Code!)
isr14:
    cli
    push dword 14
    jmp isr_common_subr

; 15: Reserved Exception
isr15:
    cli
    push dword 0
    push dword 15
    jmp isr_common_subr

; 16: Floating Point Exception
isr16:
    cli
    push dword 0
    push dword 16
    jmp isr_common_subr

; 17: Alignment Check Exception
isr17:
    cli
    push dword 0
    push dword 17
    jmp isr_common_subr

; 18: Machine Check Exception
isr18:
    cli
    push dword 0
    push dword 18
    jmp isr_common_subr

; 19: Reserved
isr19:
    cli
    push dword 0
    push dword 19
    jmp isr_common_subr

; 20: Reserved
isr20:
    cli
    push dword 0
    push dword 20
    jmp isr_common_subr

; 21: Reserved
isr21:
    cli
    push dword 0
    push dword 21
    jmp isr_common_subr

; 22: Reserved
isr22:
    cli
    push dword 0
    push dword 22
    jmp isr_common_subr

; 23: Reserved
isr23:
    cli
    push dword 0
    push dword 23
    jmp isr_common_subr

; 24: Reserved
isr24:
    cli
    push dword 0
    push dword 24
    jmp isr_common_subr

; 25: Reserved
isr25:
    cli
    push dword 0
    push dword 25
    jmp isr_common_subr

; 26: Reserved
isr26:
    cli
    push dword 0
    push dword 26
    jmp isr_common_subr

; 27: Reserved
isr27:
    cli
    push dword 0
    push dword 27
    jmp isr_common_subr

; 28: Reserved
isr28:
    cli
    push dword 0
    push dword 28
    jmp isr_common_subr

; 29: Reserved
isr29:
    cli
    push dword 0
    push dword 29
    jmp isr_common_subr

; 30: Reserved
isr30:
    cli
    push dword 0
    push dword 30
    jmp isr_common_subr

; 31: Reserved
isr31:
    cli
    push dword 0
    push dword 31
    jmp isr_common_subr

isr_common_subr:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, kernel_panic
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

