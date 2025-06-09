#include "ksh.h"
#include "intf.h"
#include "kb.h"
#include "ports.h"
#include "kernel.h"

extern uint64_t kernel_start;
extern uint64_t kernel_end;

struct cmd	cmds[] = {
	{"help", ksh_help},
	{"clear", kclear}, {"sysinfos", system_infos},
	{"reboot", kernel_reboot}, {"shutdown", shutdown},
	{"hlt", halt}, {"meminfos", meminfos},
	{NULL, NULL}
};

void	ksh_prompt(void) {
	kprint_str("$ ");
}

void	ksh_cmd(char *line) {
	int i;

	for (i = 0; cmds[i].name; ++i) {
		if (!strncmp(line, cmds[i].name, strlen(cmds[i].name))) {
			cmds[i].action();
			break;
		}
	}
	if (!cmds[i].name && line[0]) {
		kprint_str("command not found: ");
		kprint_str(line);
		kprint_char('\n');
	}
}

void	ksh_help(void) {
	kprint_str("Commands:\n");
	kprint_str("- help       print this help message\n");
	kprint_str("- hlt        halt the system\n");
	kprint_str("- reboot     reboot the system\n");
	kprint_str("- shutdown   completely shutdown the system\n");
	kprint_str("- clear      clear the screen\n");
	kprint_str("- sysinfos   print system informations\n");
	kprint_str("- meminfos   print system memory layout\n");
}

void	system_infos(void) {
	kprint_str("JuraOS x86_64 1.0\n");
}

void	kernel_reboot(void) {
	__asm__ volatile ("cli");

	while (inb(KB_STATUS_PORT) & KB_INPUT_BUFFER_FULL) {}

	outb(KB_CTRL_PORT, KB_CMD_RESET);
}

void	shutdown(void) {
	__asm__ volatile ("cli");
	outw(0x604, 0x2000);
}

void	halt(void) {
	halt_cpu();
}

void	meminfos(void) {
	kprint_str("kernel start address: ");
	kprint_hex((uint64_t)&kernel_start);
	kprint_char('\n');
	kprint_str("kernel end address: ");
	kprint_hex((uint64_t)&kernel_end);
	kprint_char('\n');
}
