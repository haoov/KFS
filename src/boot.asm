section .multiboot ;                    multiboot header that marks the program as a kernel and define the data required for multiboot startup.
    align 4 ;                           to align the data in a border of 4 octets (each data will be stored at an adress who is a x4).
    MAGIC equ 0x1BADB002 ;              Magic number as defined in the multiboot specification.
    FLAGS equ 0x0 ;                     0 means no memory Or modules are required.
    CHECKSUM equ -(MAGIC + FLAGS) ;     Ensure the sum of MAGIC + FLAGS + CHECKSUM is 0.

    dd MAGIC
    dd FLAGS
    dd CHECKSUM

    dw 0 ;                               Type
    dw 0 ;                               Flags
    dd 8 ;                               Size

section .bss ;                           Contains uninitialised variables (Block Started by Symbol)
    align 16
    stack_bottom:
        resb  1048576 ;                  Reserv 1MB for the stack
    stack_top: ;                         Label for stack adress

section .text ;                          Define a code section where the program is stored. Program's intruction go there.
    bits 32
    global _start ;                      Specify _start as the entry point to the kernel and the bootloader will jump to this position once loaded.
    extern k_main                        Declare that a k_main exist in an other file.

_start:
    cli ;                                Disable interrupts
    mov esp, stack_top ;                 Set the esp register to point the top of the stack (it grows downwards on x86 system).
    
    call check_multiboot ;               Check that multiboot is conform.

    call enable_long_mode ;              Enable long mode (64 bits)
    jmp gdt64_code_segment:init_64 ;     Jump to 64 bits mode 

    call k_main ;                        Call to the kernel main function defined in the C in our kernel.

    hlt ;                                Stops the execution of the processor while waiting for new events or instructions.

check_multiboot:
    cmp eax, 0x36d76289 ;                Compare the value in eax and magic number
    jne .no_multiboot   ;                If it do not correspond, then jump to no_multiboot
    ret

.no_multiboot:
    move al, "0"
    jmp error

error:
    mov dword [0xb8000], 0x4f524f45 ;    "OORE" (O) et "R" pour "ERROR"
    mov dword [0xb8004], 0x4f3a4f52 ;    "OR: " et "ER" pour "ROR"
    mov dword [0xb8008], 0x4f204f20 ;    "O " (O pour error)
    mov byte  [0xb800a], al
    hlt

enable_long_mode:
    lgdt [gdt64_pointer] ;                Load the long mode in the archi x86

    mov eax, cr4 ;                        Load Control register 4
    or eax, 1 << 5 ;                      Enable PAE (physical address extension) because we want max adress up to 64 Go and 64 bits depend on it
    mov cr4, eax;                         
 
    mov ecx, 0xC0000080 ;                 MSR EFER (use to activate long mode)
    rdmsr
    or eax, 1 << 8 ;                      Enable LM
    wrmsr ;                               re-write eax in EFER (extended feature enable register)

    move ecx, cr0
    or eax, 1 << 31 | 1 ;                 Enable protected mode and paging
    mov cr0, eax

    ret

section .data
gdt64:
    dq 0                              ; NULL Descriptor
gdt64_code:
    dq 0x0020980000000000             ; Code Segment: Execute/Read, Long Mode
gdt64_data:
    dq 0x0000920000000000             ; Data Segment: Read/Write

gdt64_pointer:
    dw gdt64_end - gdt64 - 1
    dq gdt64

gdt64_end:
    
gdt64_code_segment equ gdt64_code - gdt64
gdt64_data_segment equ gdt64_data - gdt64

[bits 64]
init_64:
    mov ax, gdt64_data_segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call k_main64                    ; Exécuter le kernel en 64 bits
    hlt