#ifndef KSH_H
#define KSH_H

#define PROMPT	"$ "

typedef void	(*cmd_action)(void);

struct cmd
{
	const char	*name;
	cmd_action	action;
};

void	ksh_help(void);
void	ksh_cmd(char *line);
void	ksh_prompt(void);
void	system_infos();
void	kernel_reboot();
void	shutdown(void);
void	halt(void);

#endif
