global start

section .text
bits 32

; =============================================================================
; Main entry point
; =============================================================================

start:

    move esp, stack_top         ; Set stack pointer to the top of the stack

    call check_multiboot        ; Performs compatibility

	; Initialize pagination
	call setup_paging32
    call enable_paging

	mov dword [0xb8000], 0x2f4b2f4f
	hlt

; =============================================================================
; Check subroutines
; =============================================================================
; Verify the code has been loaded by a multiboot-compliant bootloader (e. grub)
; The bootloader is expected to set eax to 0x36d76289 (multiboot1 magic number 32bit)

check_multiboot:
    cmp eax, 0x36d76289         ; Compare the value in eax and magic number
    jne .no_multiboot           ; If it do not correspond, jump to no_multiboot
    ret                         ; else

    .no_multiboot:
    mov al, "M"                 ; Set the error code to multiboot
    jmp error

; ==============================================================================
; Paging Setup
; ==============================================================================
; Set up page tables for 32-bit paging
; Map the first 4 MiB of physical memory (1024 pages of 4 KiB each)

setup_paging32:

    ; Initialize page directory with the address of the first page table
    mov eax, page_table         ; Load address of page table
    or eax, 0b11                ; Set bits: present (bit 0), writable (bit 1)
    mov [page_directory], eax   ; Insert address into the page directory

    ; Initialize all page table entries
    mov ecx, 0                  ; Counter for page table entries

    .map:
        mov eax, 0x200000                   ; Load counter value into EAX, which is the page number
        shl eax, 12                         ; Multiply by 2^12 = 4096 (4 KiB pages)
        or eax, 0b10000011                  ; Set bits: present (bit 0), writable (bit 1)
        mov [page_table + ecx * 4], eax     ; Add entry to the page table, each entry is 4 bytes (32 bits)

        inc ecx                     ; Move to the next page (increment counter)
        cmp ecx, 1024               ; 1024 entries per page table (4 MiB), compare with counter
        jne .map                    ; Repeat until all entries are mapped (until counter is 1024)

        ret                         ; Return all entries are map

enable_paging:
        ; Load the address of the page directory into CR3
        mov eax, page_directory     ; Load address of page directory
        mov cr3, eax                ; Load address into CR3, which is the page directory base register

        ; Enable paging by setting the PG bit in CR0
        mov eax, cr0                ; Load CR0 into EAX
        or eax, 1 << 31             ; Set the PG bit (bit 31 in CR0)
        mov cr0, eax                ; Load EAX back into CR0

        ret                         ; Return after enabling paging

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

    hlt                              ; HALT THE CPU

; ==============================================================================
; Data Sections
; ==============================================================================

section .bss
align 4096                      ; Align to 4 KiB (page size)

; Page Directory (1024 entries of 4 bytes each)
page_directory:
    resb 4096                   ; Reserve 4 KiB for the page directory

; Page Table (1024 entries of 4 bytes each)
page_table:
    resb 4096                   ; Reserve 4 KiB for a single page table

stack_bottom:
    resb 4096 * 4               ; 16K stack
stack_top:

section .rotada                 ; RO data-section