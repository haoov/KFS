#include "klib.h"
#include "idt.h"
#include "intf.h"
#include "kernel.h"
#include "ports.h"

struct idt_entry	idt[256];
struct idt_ptr		idtp;

char	*except_msg[32] = {
	"Division by zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

uint64_t int_subs[48] = {
	(uint64_t)&isr0,
	(uint64_t)&isr1,
	(uint64_t)&isr2,
	(uint64_t)&isr3,
	(uint64_t)&isr4,
	(uint64_t)&isr5,
	(uint64_t)&isr6,
	(uint64_t)&isr7,
	(uint64_t)&isr8,
	(uint64_t)&isr9,
	(uint64_t)&isr10,
	(uint64_t)&isr11,
	(uint64_t)&isr12,
	(uint64_t)&isr13,
	(uint64_t)&isr14,
	(uint64_t)&isr15,
	(uint64_t)&isr16,
	(uint64_t)&isr17,
	(uint64_t)&isr18,
	(uint64_t)&isr19,
	(uint64_t)&isr20,
	(uint64_t)&isr21,
	(uint64_t)&isr22,
	(uint64_t)&isr23,
	(uint64_t)&isr24,
	(uint64_t)&isr25,
	(uint64_t)&isr26,
	(uint64_t)&isr27,
	(uint64_t)&isr28,
	(uint64_t)&isr29,
	(uint64_t)&isr30,
	(uint64_t)&isr31,
	(uint64_t)&irq0,
	(uint64_t)&irq1,
	(uint64_t)&irq2,
	(uint64_t)&irq3,
	(uint64_t)&irq4,
	(uint64_t)&irq5,
	(uint64_t)&irq6,
	(uint64_t)&irq7,
	(uint64_t)&irq8,
	(uint64_t)&irq9,
	(uint64_t)&irq10,
	(uint64_t)&irq11,
	(uint64_t)&irq12,
	(uint64_t)&irq13,
	(uint64_t)&irq14,
	(uint64_t)&irq15
};

void *irq_handlers[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0 ,0 ,0, 0, 0 ,0 ,0, 0
};

void idt_set_entry(int index, uint64_t handler, uint16_t selector, uint8_t attr) {
	idt[index].offset_low = handler & 0xFFFF;
	idt[index].offset_mid = (handler >> 16) & 0xFFFF;
	idt[index].offset_high = (handler >> 32) & 0xFFFFFFFF;
	idt[index].selector = selector;
	idt[index].ist = 0;
	idt[index].type_attr = attr;
	idt[index].reserved = 0;
}

void irq_install(int index, void (*handler)(struct interrupt_frame *frame)) {
	irq_handlers[index] = handler;
}

void irq_uninstall(int index) {
	irq_handlers[index] = 0;
}

void interrupt_handler(struct interrupt_frame *frame) {
	void (*handler)(struct interrupt_frame *fram);
	switch (frame->int_no) {
		case 0 ... 31:
			kernel_panic(except_msg[frame->int_no], frame);
			break;
		case 32 ... 47:
			handler = irq_handlers[frame->int_no - 32];
			if (handler) {
				handler(frame);
			}
			if (frame->int_no >= 40) {
				outb(0xA0, 0x20);
			}
			outb(0x20, 0x20);
			break;
		default:
			break;
	}
}

void remap_pic() {
	// Start init sequence
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	// Set vector offsets
	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	// Tell master about slave
	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	// 8086 mode
	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	// Enabled all interrupts
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

void idt_init() {
	idtp.limit = sizeof(idt) - 1;
	idtp.base = (uint64_t)&idt;

	memset(&idt, 0, sizeof(idt));

	remap_pic();

	for (int i = 0; i < 48; ++i) {
		idt_set_entry(i, int_subs[i], CODE_SEG, INTERRUPT_GATE | PRESENT);
	}

	asm volatile("lidt %0" : :  "m"(idtp));

	asm volatile("sti");
}
