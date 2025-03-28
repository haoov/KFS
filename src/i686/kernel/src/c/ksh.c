#include "ksh.h"
#include "intf.h"
#include "kb.h"
#include "ports.h"
#include "gdt.h"
#include "idt.h"

extern struct gdt_ptr	gdtp;
extern struct idt_ptr	idtp;
extern uint32_t			kernel_start;

struct cmd	cmds[] =
{
	{"help", ksh_help}, {"pstack", print_stack},
	{"clear", kclear}, {"sysinfos", system_infos},
	{"reboot", kernel_reboot}, {"shutdown", shutdown},
	{"hlt", halt}, {"meminfos", meminfos},
	{NULL, NULL}
};

void	ksh_prompt(void)
{
	kprint("$ ");
}

void	ksh_cmd(char *line)
{
	int i;

	for (i = 0; cmds[i].name; ++i)
	{
		if (!strncmp(line, cmds[i].name, strlen(cmds[i].name)))
		{
			cmds[i].action();
			break;
		}
	}
	if (!cmds[i].name && line[0])
	{
		kprint("command not found: %s\n", line);
	}
}

void	ksh_help(void)
{
	kprint("Commands:\n");
	kprint("- help       print this help message\n");
	kprint("- hlt        halt the system\n");
	kprint("- reboot     reboot the system\n");
	kprint("- shutdown   completely shutdown the system\n");
	kprint("- pstack     print 20 entries from top of the stack\n");
	kprint("- clear      clear the screen\n");
	kprint("- sysinfos   print system informations\n");
	kprint("- meminfos   print system memory layout\n");
}

void	system_infos(void)
{
	kprint("JuraOS i686 1.0\n");
}

void	kernel_reboot(void)
{
	__asm__ volatile ("cli");

	while (inb(KB_STATUS_PORT) & KB_INPUT_BUFFER_FULL) {}

	outb(KB_CTRL_PORT, KB_CMD_RESET);
}

void	shutdown(void)
{
	__asm__ volatile ("cli");
	outw(0x604, 0x2000);
}

void	halt(void)
{
	__asm__ volatile
	(
		"cli\n\t"
		"hlt"
	);
}

void	meminfos(void)
{
	kprint("gdt physical address: %x\n", gdtp.base);
	kprint("idt physical address: %x\n", idtp.base);
	kprint("kernel start address: %x\n", &kernel_start);
}
