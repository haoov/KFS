; =============================================================================
; 32bit to 64bit switch
; =============================================================================
; This code makes the switch from a 32bit protected mode the 64bit long mode
; It perform the necessary checks, set up and enable paging (required)
; then jumps to 64bit code

global start				; Export 'start' symbol for linker
extern long_mode_start		; Reference to 64bit code (main64.asm)

section .text				; Code section
bits 32						; Specify 32bit instructions

; =============================================================================
; Main entry point
; =============================================================================
start:
	mov esp, stack_top		; Set stack pointer to top of the stack

	; Performs compatibility checks before switching
	call check_multiboot
	call check_cpuid
	call check_long_mode
    
	; Set up and enable paging
	call setup_page_tables
	call enable_paging

	; Load the 64bit global descriptor table
	lgdt [gdt64.pointer]

	; Far jump to 64bit code
	; This load the code segment then jumps to the 64bit entry point
	jmp gdt64.code_segment:long_mode_start

	; Halt the cpu (should never be reached)
	hlt

; =============================================================================
; Check subroutines
; =============================================================================
; Verify the code has been loaded by a multiboot-compliant bootloader (e. grub)
; The bootloader is expected to set eax to 0x2BADB002 (multiboot1 magic number)
check_multiboot:
	cmp eax, 0x2BADB002		; Magic number comparaison
	jne .no_multiboot		; If not equal, multiboot is not present
	ret						; Return if multiboot confirmed
	.no_multiboot:
		mov al, "M"			; Set the error code to M (multiboot)
		jmp error			; Jump to error handler

; Verify CPUID instruction is available
; This is done by attempting to flip bit 21 in EFLAGS register
check_cpuid:
    pushfd                 ; Push EFLAGS to stack
    pop eax                ; Pop EFLAGS into EAX
    mov ecx, eax           ; Store original EFLAGS in ECX for comparison
    xor eax, 1 << 21       ; Flip bit 21 (ID bit)
    push eax               ; Push modified value
    popfd                  ; Pop into EFLAGS
    pushfd                 ; Push EFLAGS again
    pop eax                ; Pop EFLAGS into EAX
    push ecx               ; Restore original EFLAGS
    popfd                  ; from the saved ECX value
    cmp eax, ecx           ; If bit 21 not changed, CPUID isn't available
    je .no_cpuid           ; Jump if equal (CPUID not available)
    ret                    ; Return if CPUID is available
    .no_cpuid:
        mov al, "C"        ; Set error code to 'C' (CPUID)
        jmp error          ; Jump to error handler

; Verify the CPU supports 64-bit long mode
check_long_mode:
    mov eax, 0x80000000    ; Set EAX to query highest extended function
    cpuid                  ; Call CPUID
    cmp eax, 0x80000001    ; Compare result - must be at least 0x80000001
    jb .no_long_mode       ; Jump if below (long mode not supported)

    mov eax, 0x80000001    ; Set EAX to query extended processor info
    cpuid                  ; Call CPUID
    test edx, 1 << 29      ; Test bit 29 in EDX (LM bit)
    jz .no_long_mode       ; Jump if zero (long mode not supported)

    ret                    ; Return if long mode is supported
    .no_long_mode:
        mov al, "L"        ; Set error code to 'L' (Long Mode)
        jmp error          ; Jump to error handler

; ==============================================================================
; Paging Setup
; ==============================================================================

; Set up page tables for 64-bit long mode
; This creates a 4-level paging hierarchy required for 64-bit mode
setup_page_tables:
    ; Map first PML4 entry to P3 table
    mov eax, page_table_l3
    or eax, 0b11           ; Set bits: present (bit 0), writable (bit 1)
    mov [page_table_l4], eax

    ; Map first P3 entry to P2 table
    mov eax, page_table_l2
    or eax, 0b11           ; Set bits: present (bit 0), writable (bit 1)
    mov [page_table_l3], eax

    mov ecx, 0             ; Initialize counter to 0

    ; Map each P2 entry to a 2MiB page
    .map:
        mov eax, 0x200000  ; 2MiB page size
        mul ecx            ; Multiply by counter to get physical address
        or eax, 0b10000011 ; Set bits: present, writable, huge page (bit 7)
        mov [page_table_l2 + ecx * 8], eax  ; Store entry (entry is 8 bytes)

        inc ecx            ; Increment counter
        cmp ecx, 512       ; Check if we've mapped all entries
        jne .map           ; If not, continue mapping

        ret                ; Return when all entries are mapped

; Enable paging and long mode in the CPU
enable_paging:
    ; Load page table address into CR3 register
    mov eax, page_table_l4
    mov cr3, eax

    ; Enable Physical Address Extension (PAE)
    mov eax, cr4
    or eax, 1 << 5         ; Set PAE bit (bit 5)
    mov cr4, eax

    ; Enable long mode in EFER MSR
    mov ecx, 0xC0000080    ; Set ECX to EFER MSR number
    rdmsr                  ; Read the MSR into EDX:EAX
    or eax, 1 << 8         ; Set LME bit (bit 8)
    wrmsr                  ; Write back to the MSR

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31        ; Set PG bit (bit 31)
    mov cr0, eax

    ret                    ; Return with paging enabled

; ==============================================================================
; Error Handling
; ==============================================================================

; Error handler - displays error code on screen and halts
error:
    ; Print "ERR: X" where X is the error code
    ; Write directly to VGA text buffer at 0xb8000
    mov dword [0xb8000], 0x4f524f45  ; 'ER' in red on white
    mov dword [0xb8004], 0x4f3a4f52  ; 'R:' in red on white
    mov dword [0xb8008], 0x4f204f20  ; '  ' in red on white
    mov byte  [0xb800a], al          ; Error code character in red on white

    hlt                              ; Halt the CPU

; ==============================================================================
; Data Sections
; ==============================================================================

section .bss             ; Uninitialized data section
align 4096               ; Align to 4K boundary (page size)

; Page tables - 4K each (512 * 8)
page_table_l4:
    resb 4096            ; PML4 Table (Level 4)
page_table_l3:
    resb 4096            ; PDPT Table (Level 3)
page_table_l2:
    resb 4096            ; PD Table (Level 2)

; Stack - 16K total size
stack_bottom:
    resb 4096 * 4        ; 16K stack
stack_top:               ; Top of stack (grows downward)

section .rodata          ; Read-only data section

; Global Descriptor Table for 64-bit mode
gdt64:
    dq 0                 ; Null descriptor (required)
    .code_segment: equ $ - gdt64
        ; Code segment descriptor bits:
        ; bit 43: executable
        ; bit 44: descriptor type (code/data)
        ; bit 47: present
        ; bit 53: 64-bit
        dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)

    .pointer:            ; GDT pointer structure
        dw $ - gdt64 - 1 ; Size of GDT minus 1
        dq gdt64         ; Address of GDT

