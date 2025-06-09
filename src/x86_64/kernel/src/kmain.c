#include "kernel.h"
#include "idt.h"
#include "intf.h"
#include "kb.h"
#include "ksh.h"

void	entry_msg() {
	set_scr_color((PCOLOR_BLACK << 4) | PCOLOR_GREEN);
	kclear();
	kprint_str("      _                    ____   _____ \n");
	kprint_str("     | |                  / __ \\ / ____|\n");
	kprint_str("     | |_   _ _ __ __ _  | |  | | (___  \n");
	kprint_str(" _   | | | | | '__/ _` | | |  | |\\___ \\ \n");
	kprint_str("| |__| | |_| | | | (_| | | |__| |____) |\n");
	kprint_str(" \\____/ \\__,_|_|  \\__,_|  \\____/|_____/ \n");
	system_infos();
	kprint_char(NEWLINE);
}

void kmain() {
	idt_init();
	keyboard_setup();
	entry_msg();
	ksh_prompt();
}
