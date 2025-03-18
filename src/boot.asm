section .multiboot ;                multiboot header that marks the program as a kernel and define the data required for multiboot startup.
    align 4 ;                       to align the data in a border of 4 octets (each data will be stored at an adress who is a x4).
    MAGIC equ 0x1BADB002 ;          Magic number as defined in the multiboot specification.
    FLAGS equ 0x0 ;                 0 means no memory Or modules are required.
    CHECKSUM equ -(MAGIC + FLAGS) ; Ensure the sum of MAGIC + FLAGS + CHECKSUM is 0.

    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss ;                      Contains uninitialised variables (Block Started by Symbol)
    stack_bottom:
        resb  1048576 ;             Reserv 8 KB for the stack (8192 octets)
    stack_top: ;                    Label for stack adress

section .text ;                     Define a code section where the program is stored. Program's intruction go there.
    bits 32
    global _start ;                 Specify _start as the entry point to the kernel and the bootloader will jump to this position once loaded.
    extern k_main                   Declare that a k_main exist in an other file.

_start:
    mov esp, stack_top ;             Set the esp register to point the top of the stack (it grows downwards on x86 system).
    
    call check_multiboot ;          Check that multiboot is conform.

    call k_main ;                   Call to the kernel main function defined in the C in our kernel.

    hlt ;                           Stops the execution of the processor while waiting for new events or instructions.

check_multiboot:
    cmp eax, 0x36d76289             Compare the value in eax and magic number
    jne .no_multiboot               If it do not correspond, then jump to no_multiboot
    ret

.no_multiboot:
    move al, "0"
    jmp error

error:
    mov dword [0xb8000], 0x4f524f45  ; "OORE" (O) et "R" pour "ERROR"
    mov dword [0xb8004], 0x4f3a4f52  ; "OR: " et "ER" pour "ROR"
    mov dword [0xb8008], 0x4f204f20  ; "O " (O pour error)
    mov byte  [0xb800a], al
    hlt