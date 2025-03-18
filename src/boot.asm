section .multiboot ;            multiboot header that marks the program as a kernel and define the data required for multiboot startup.
align 8 ;                       to align the data in a border of 4 octets (each data will be stored at an adress who is a x4).

MAGIC equ 0x1BADB002 ;          Magic number as defined in the multiboot specification.
FLAGS equ 0x0 ;                 0 means no memory Or modules are required.
CHECKSUM equ -(MAGIC + FLAGS) ; Ensure the sum of MAGIC + FLAGS + CHECKSUM is 0.

long MAGIC
long FLAGS
long CHECKSUM

section .bss
resb 8192 ;                     Reserv 8 KB for the stack
stack_top: ;                    Label for stack adress

section .text ;                 Define a code section where the program is stored. Program's intruction go there.
global _start ;                 Specify _start as the entry point to the kernel and the bootloader will jump to this position once loaded.

_start:
    mov esp, stack_top ;        Set the esp register to point the top of the stack (it grows downwards on x86 system).
    
    call k_main ;               Call to the kernel main function defined in the C in our kernel.

    cli ;                       Disable interrupts on the processor. To do before manipulating hardware or starting a task in the kernel.
    
    hlt ;                       Stops the execution of the processor while waiting for new events or instructions.
