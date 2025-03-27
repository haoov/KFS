#include "ksh.h"
#include "intf.h"
#include "kb.h"

struct cmd	cmds[] =
{
	{"help", ksh_help}, {"pstack", print_stack},
	{"clear", kclear}, {NULL, NULL}
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
		kprint("Command not found ");
		kprint(line);
		kprint_char(NEWLINE);
	}
}

void	ksh_help(void)
{
	kprint("Commands:\n");
	kprint("- help          print this help message\n");
//	kprint("- hlt\t\thalt the system\n");
//	kprint("- reboot\t\treboot the system\n");
	kprint("- pstack        print 20 entries from top of the stack\n");
	kprint("- clear         clear the screen\n");
//	kprint("- sysinfos\t\tprint system informations\n");
}

